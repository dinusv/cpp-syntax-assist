/****************************************************************************
**
** Copyright (C) 2014-2015 Dinu SV.
** (contact: mail@dinusv.com)
** This file is part of C++ Snippet Assist application.
**
** GNU General Public License Usage
** 
** This file may be used under the terms of the GNU General Public License 
** version 3.0 as published by the Free Software Foundation and appearing 
** in the file LICENSE.GPL included in the packaging of this file.  Please 
** review the following information to ensure the GNU General Public License 
** version 3.0 requirements will be met: http://www.gnu.org/copyleft/gpl.html.
**
****************************************************************************/


#include "QASTFile.hpp"
#include "QSourceLocation.hpp"
#include "QSourceLocation_p.hpp"
#include "QTokenClassifier.hpp"
#include "QAnnotatedTokenSet.hpp"
#include "QCSAConsole.hpp"

#include <QFile>
#include <QFileInfo>
#include <QTextStream>

namespace csa{ namespace ast{

// QInsertionElementPrivate
// ------------------------

class QModifierElementPrivate{

public:
    enum Type{
        Insertion,
        Erasion
    };

public:
    QModifierElementPrivate(
            const QString& pData,
            const QSourceLocation& pLocation)
        : type(Insertion), data(pData), location(pLocation), location2(pLocation){}

    QModifierElementPrivate(const QSourceLocation& pLocation1, const QSourceLocation& pLocation2)
        : type(Erasion), data(""), location(pLocation1), location2(pLocation2){}

    Type    type;
    QString data;
    QSourceLocation location;
    QSourceLocation location2;

private:
    QModifierElementPrivate();
};

bool offsetCompare(QModifierElementPrivate* e1, QModifierElementPrivate* e2){
    if ( e1->location.filePath() == e2->location.filePath() )
        return (e1->location.offset() < e2->location.offset());

    return (e1->location.filePath() < e2->location.filePath());
}

// QASTFile Definitions
// --------------------

QASTFile::QASTFile(QAnnotatedTokenSet* tokenSet, const QString& file, QSourceLocation *endOfFile)
    : QASTNode(
          "file",
          tokenSet,
          new QSourceLocation(file, 0, 0, 0),
          new QSourceLocation(file, 0, 0, 0),
          endOfFile,
          0)
{
    setIdentifier(file);
}

bool QASTFile::hasModifiers(){
    return m_modifiers.size() > 0;
}

void QASTFile::save(){
    if ( hasModifiers() ){
        std::sort(m_modifiers.begin(), m_modifiers.end(), offsetCompare);

        QString filePath = identifier();
        QFile file(filePath);
        if ( !file.open(QIODevice::ReadOnly) ){
            QCSAConsole::logError("Cannot open file \'" + filePath + "\' for reading.");
            return;
        }

        QString fileData = QTextStream(&file).readAll();
        file.close();

        if ( !file.open(QIODevice::WriteOnly | QIODevice::Truncate ) ){
            QCSAConsole::logError("Cannot open file \'" + filePath + "\' for writing.");
            return;
        }

        QTextStream writeStream(&file);
        int startCut, endCut = 0;
        int insertions = 0, deletions = 0;

        for ( QList<QModifierElementPrivate*>::iterator it = m_modifiers.begin(); it != m_modifiers.end(); ++it ){
            QModifierElementPrivate* el = *it;

            if ( el->type == QModifierElementPrivate::Insertion ){
                startCut = endCut;
                endCut   = el->location.offset();
                writeStream << fileData.mid(startCut, endCut - startCut) << el->data;
                ++insertions;
            } else {
                startCut = endCut;
                endCut   = el->location.offset();
                writeStream << fileData.mid(startCut, endCut - startCut);
                endCut   = el->location2.offset();
                ++deletions;
            }
        }
        writeStream << fileData.mid(endCut);

        QCSAConsole::log(QCSAConsole::Info1,
            "Total modifications: " + QString::number(insertions) + " insertions and " + QString::number(deletions) +
            " in file \'" + identifier() +  "\'"
        );
    }
}

bool QASTFile::insert(const QString& value, QSourceLocation* location){
    if ( location->offset() <= size() && location->filePath() == identifier() ){
         m_modifiers.append(new QModifierElementPrivate(value, *location));
        return true;
    } else {
        QCSAConsole::logError("Cannot insert :\'" +  value + "'. Incompatible file location.");
        return false;
    }
}

bool QASTFile::erase(QSourceLocation* from, QSourceLocation* to){
    if ( from->offset() <= size() &&
         to->offset() <= size() &&
         from->offset() <= to->offset() &&
         from->filePath() == identifier() &&
         to->filePath() == identifier() )
    {
        m_modifiers.append(new QModifierElementPrivate(*from, *to));
        return true;
    } else {
        QCSAConsole::logError(
            "Cannot erase between positions: " + from->toString() + " and " + to->toString() +
            ". Incompatible file locations.");
        return false;
    }
}

QString QASTFile::readAll(){
    QFile file(identifier());
    if ( !file.open(QIODevice::ReadOnly) ){
        QCSAConsole::logError("Cannot open file \'" + identifier() + "\' for reading.");
        return "";
    }

    return QTextStream(&file).readAll();
}

QString QASTFile::read(QSourceLocation* start, QSourceLocation* end) const{
    QFile file(identifier());
    if ( !file.open(QIODevice::ReadOnly) ){
        QCSAConsole::logError("Cannot open file \'" + identifier() + "\' for reading.");
        return "";
    }

    QTextStream fileStream(&file);
    fileStream.seek(start->offset());

    return fileStream.read(end->offset() - start->offset());
}

unsigned int QASTFile::size(){
    return rangeEndLocation()->offset() + 1;
}

QSourceLocation* QASTFile::createLocation(unsigned int lineOrOffset, unsigned int column){
    CXFile clangFile = clang_getFile(tokenSet()->translationUnit(), identifier().toLocal8Bit().constData());
    return new QSourceLocation(
        createSourceLocation(
            column == 0 ? clang_getLocationForOffset(tokenSet()->translationUnit(), clangFile, lineOrOffset) :
                          clang_getLocation(tokenSet()->translationUnit(), clangFile, lineOrOffset, column))
    );
}

QString QASTFile::fileName(){
    return QFileInfo(identifier()).fileName();
}

QString QASTFile::fileNameWithouExtension(){
    return QFileInfo(identifier()).baseName();
}

QString QASTFile::extension(){
    return QFileInfo(identifier()).suffix();
}

QString QASTFile::text(QSourceLocation *from, QSourceLocation *to){
    return read(from, to);
}

}}//namespace
