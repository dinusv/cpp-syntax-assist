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


#include "QCodeBase.hpp"
#include "QCodeBaseObserver.hpp"
#include "QSourceLocation.hpp"
#include "QTokenClassifier.hpp"
#include "QAnnotatedTokenSet.hpp"
#include "QASTFile.hpp"
#include "QASTNode.hpp"
#include "QASTVisitor.hpp"
#include <QMap>
#include <QHash>
#include <QDebug>

namespace csa{

using namespace ast;

class QCodeBasePrivate{
public:
    CXIndex index;
};

QCodeBase::QCodeBase(
        const char* const* translationUnitArgs,
        int                translationUnitNumArgs,
        const QStringList &files,
        const QString&     searchDir,
        QCodeBaseObserver* observer,
        QObject*           parent)

    : QObject(parent)
    , d_ptr(new QCodeBasePrivate)
    , m_searchDir(searchDir)
    , m_root(0)
    , m_current(0)
    , m_observer(observer){

    Q_D(QCodeBase);

    m_sourceSearchPatterns << "*.c" << "*.C" << "*.cxx" << "*.cpp" << "*.c++" << "*.cc" << "*.cp";
    m_headerSearchPatterns << "*.h" << "*.H" << "*.hxx" << "*.hpp" << "*.h++" << "*.hh" << "*.hp";

    m_translationUnitNumArgs = translationUnitNumArgs;
    m_translationUnitArgs    = new char*[m_translationUnitNumArgs];

    for ( int i = 0 ;i < m_translationUnitNumArgs; ++i ){
        m_translationUnitArgs[i] = new char[strlen(translationUnitArgs[i]) + 1];
        strcpy(m_translationUnitArgs[i], translationUnitArgs[i]);
    }

    d->index = clang_createIndex(0, 0);

    for ( QStringList::const_iterator it = files.begin(); it != files.end(); ++it ){
        loadFile(*it);
    }

    updateTreeModel();
}

QCodeBase::~QCodeBase(){
    for ( int i = 0; i < m_translationUnitNumArgs; ++i ){
        delete [] m_translationUnitArgs[i];
    }
    delete[] m_translationUnitArgs;
    delete d_ptr;
}

void QCodeBase::save(){
    for ( int i = 0; i < m_files.size(); ++i ){
        QASTFile* root               = m_files[i];
        QTokenClassifier* classifier = m_classifiers[i];

        if ( root->hasInsertions() ){
            root->saveInsertions();

            QString selectionTypeString = m_current->typeName();
            QString selectionIdentifier = m_current->identifier();

            CXTranslationUnit transUnit = classifier->translationUnit();
            clang_reparseTranslationUnit(transUnit, 0, 0, clang_defaultReparseOptions(transUnit));
            CXCursor startCursor        = clang_getTranslationUnitCursor(transUnit);
            classifier->reparse();
            if ( m_observer )
                m_observer->clearAndReset();
            root->removeChildren();
            QASTVisitor::createCSANodeTree(startCursor, root, classifier);

            if ( i == 0 )
                m_current = root;

            updateTreeModel();

            if ( i == 0 )
                select(selectionTypeString, selectionIdentifier);
        }
    }
}

void QCodeBase::propagateUserCursor(int offset, const QString &file){
    CXTranslationUnit transUnit = m_classifiers.first()->translationUnit();
    CXFile cfile = clang_getFile(transUnit, file.toStdString().c_str());
    CXSourceLocation sLocation = clang_getLocationForOffset(transUnit, cfile, offset);
    propagateUserCursor(QSourceLocation(sLocation));
}

void QCodeBase::propagateUserCursor(int line, int column, const QString &file){
    CXTranslationUnit transUnit = m_classifiers.first()->translationUnit();
    CXFile cfile = clang_getFile(transUnit, file.toStdString().c_str());
    CXSourceLocation sLocation = clang_getLocation(transUnit, cfile, line, column);
    propagateUserCursor(QSourceLocation(sLocation));
}

void QCodeBase::propagateUserCursor(const QSourceLocation &location){
    QASTNode* deepest = m_current->propagateUserCursor(location);
    if ( deepest != 0 ){
        m_current = deepest;
        if ( m_observer )
            m_observer->setSelected(m_current);
    }
}

csa::ast::QASTFile* QCodeBase::loadFile(const QString &file){
    Q_D(QCodeBase);

    CXTranslationUnit transUnit  = clang_parseTranslationUnit(
                d->index,
                file.toStdString().c_str(),
                m_translationUnitArgs,
                m_translationUnitNumArgs,
                0,
                0,
                CXTranslationUnit_Incomplete | CXTranslationUnit_CXXChainedPCH);

    CXCursor startCursor  = clang_getTranslationUnitCursor(transUnit);
    CXFile   clangFile    = clang_getFile(transUnit, file.toStdString().c_str());

    QTokenClassifier* fileClassifier = new QTokenClassifier(transUnit, file.toStdString().c_str());
    QASTFile* fileRoot =
        new QASTFile(
            new QAnnotatedTokenSet(startCursor, transUnit),
            file,
            new QSourceLocation(
                clang_getLocationForOffset(
                    transUnit,
                    clangFile,
                    QTokenClassifier::getFileSize(file.toStdString().c_str())
                )
            ));

    m_files.append(fileRoot);
    m_classifiers.append(fileClassifier);

    if ( !m_current )
        m_current = fileRoot;

    QASTVisitor::createCSANodeTree(startCursor, fileRoot, fileClassifier);

    updateTreeModel();

    return fileRoot;
}

void QCodeBase::updateTreeModel(){
    if ( m_observer ){
        m_observer->parse(m_files);
        m_observer->setSelected(m_current);
    }
}

void QCodeBase::setHeaderSearchPattern(const QStringList &pattern){
    m_headerSearchPatterns = pattern;
}

void QCodeBase::setSourceSearchPattern(const QStringList &pattern){
    m_sourceSearchPatterns = pattern;
}

bool QCodeBase::select(const QString &typeString, const QString &name){
    QASTNode* result = qobject_cast<QASTNode*>(m_root->find(typeString, name));
    if ( result ){
        m_current = result;
        if ( m_observer )
            m_observer->setSelected(m_current);
        return true;
    }
    return false;
}

QASTFile *QCodeBase::findSource(const QString &header){
    QString headerBaseName = QFileInfo(header).baseName();

    QString searchLocation = m_searchDir;
    if ( searchLocation == "" )
        searchLocation = QFileInfo(header).path();

    QDirIterator it(searchLocation, m_sourceSearchPatterns, QDir::Files, QDirIterator::Subdirectories);
    while (it.hasNext()){
        QString file = it.next();
        if ( it.fileInfo().baseName() == headerBaseName ){
            QASTFile* astFile = findFile(file);
            if ( !astFile )
                astFile = loadFile(file);
            return astFile;
        }
    }

    return 0;
}

QASTFile *QCodeBase::findHeader(const QString &source){
    QString sourceBaseName = QFileInfo(source).baseName();

    QString searchLocation = m_searchDir;
    if ( searchLocation == "" )
        searchLocation = QFileInfo(source).path();

    QDirIterator it(searchLocation, m_headerSearchPatterns, QDir::Files, QDirIterator::Subdirectories);
    while (it.hasNext()){
        QString file = it.next();
        if ( it.fileInfo().baseName() == sourceBaseName ){
            QASTFile* astFile = findFile(file);
            if ( !astFile )
                astFile = loadFile(file);
            return astFile;
        }
    }

    return 0;
}

QTokenClassifier *QCodeBase::classifierForFile(const QString &file){
    for ( int i = 0; i < m_files.size(); ++i ){
        if ( m_files[i]->identifier() == file ){
            return m_classifiers[i];
        }
    }
    return 0;
}

QList<QASTFile*> QCodeBase::files(){
    return m_files;
}

QASTFile *QCodeBase::findFile(const QString &fileName){
    for ( QList<ast::QASTFile*>::const_iterator it = m_files.begin(); it != m_files.end(); ++it ){
        ast::QASTFile* file = *it;
        if ( file->identifier() == fileName )
            return file;
    }
    return 0;
}

QASTNode* QCodeBase::cursorNode(){
    return m_current;
}

QSourceLocation* QCodeBase::createLocation(const QString& file, unsigned int offset){
    QTokenClassifier* classifier    = classifierForFile(file);
    if ( !classifier )
        return 0;

    CXTranslationUnit transUnit     = classifier->translationUnit();
    CXSourceLocation sourceLocation = clang_getLocationForOffset(
                transUnit,
                clang_getFile(transUnit, file.toStdString().c_str()),
                offset);
    if ( !clang_equalLocations(sourceLocation, clang_getNullLocation()) )
        return new QSourceLocation(sourceLocation);

    return 0;
}

QSourceLocation* QCodeBase::createLocation(const QString& file, unsigned int line, unsigned int column){
    QTokenClassifier* classifier    = classifierForFile(file);
    if ( !classifier )
        return 0;

    CXTranslationUnit transUnit     = classifier->translationUnit();
    CXSourceLocation sourceLocation = clang_getLocation(
                transUnit,
                clang_getFile(transUnit, file.toStdString().c_str()),
                line,
                column);
    if ( !clang_equalLocations(sourceLocation, clang_getNullLocation()) )
        return new QSourceLocation(sourceLocation);
    return 0;
}

}// namespace
