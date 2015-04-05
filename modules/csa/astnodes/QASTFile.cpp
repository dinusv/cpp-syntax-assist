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
#include "QTokenClassifier.hpp"

#include <QFile>

namespace csa{ namespace ast{

class QInsertionElementPrivate{

public:
    QInsertionElementPrivate(const QString& pData, const QSourceLocation& pLocation)
        : data(pData), location(pLocation){}

    QString data;
    QSourceLocation location;

private:
    QInsertionElementPrivate();
};

bool offsetCompare(QInsertionElementPrivate* e1, QInsertionElementPrivate* e2){
    if ( e1->location.filePath() == e2->location.filePath() )
        return (e1->location.offset() < e2->location.offset());

    return (e1->location.filePath() < e2->location.filePath());
}

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

bool QASTFile::hasInsertions(){
    return m_insertions.size() > 0;
}

void QASTFile::saveInsertions(){

    if ( hasInsertions() ){
        std::sort(m_insertions.begin(), m_insertions.end(), offsetCompare);

        QString filePath = identifier();
        QFile file(filePath);
        if ( !file.open(QIODevice::ReadOnly) ){
            qCritical() << "Cannot open file '" << filePath << "' for reading.";
            return;
        }

        QString fileData = QTextStream(&file).readAll();
        file.close();

        if ( !file.open(QIODevice::WriteOnly | QIODevice::Truncate ) ){
            qCritical() << "Cannot open file '" << filePath << "' for writing.";
            return;
        }

        QTextStream writeStream(&file);
        int startCut, endCut = 0;

        for ( QList<QInsertionElementPrivate*>::iterator it = m_insertions.begin(); it != m_insertions.end(); ++it ){
            QInsertionElementPrivate* el = *it;
            startCut = endCut;
            endCut = el->location.offset();
            writeStream << fileData.mid(startCut, endCut - startCut) << el->data;
        }
        writeStream << fileData.mid(endCut);
    }
}

void QASTFile::insert(const QString& value, const QSourceLocation& location){
    m_insertions.append(new QInsertionElementPrivate(value, location));
}

}}//namespace
