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
#include "QSourceLocation.hpp"
#include "QSourceLocation_p.hpp"
#include "QTokenClassifier.hpp"
#include "QAnnotatedTokenSet.hpp"
#include "QASTFile.hpp"
#include "QASTNode.hpp"
#include "QASTVisitor.hpp"
#include "QASTSearch.hpp"
#include "QCSAConsole.hpp"
#include <QMap>
#include <QHash>

namespace csa{

using namespace ast;

class QCodeBasePrivate{
public:
    CXIndex index;
};

QCodeBase::QCodeBase(
        const char* const* translationUnitArgs,
        int                translationUnitNumArgs,
        const QStringList &entries,
        const QString&     searchDir,
        QObject*           parent)

    : QObject(parent)
    , d_ptr(new QCodeBasePrivate)
    , m_projectDir(searchDir != "" ? QDir(searchDir).path() : (entries.size() > 0 ? QFileInfo(entries[0]).path() : ""))
    , m_root(0)
    , m_current(0){

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

    for ( QStringList::const_iterator it = entries.begin(); it != entries.end(); ++it ){
        parsePath(*it);
    }
}

QCodeBase::~QCodeBase(){
    for ( int i = 0; i < m_translationUnitNumArgs; ++i ){
        delete [] m_translationUnitArgs[i];
    }
    delete[] m_translationUnitArgs;
    delete d_ptr;
}

void QCodeBase::save(){
    QString currentSelection     = m_current ? m_current->breadcrumbs() : "*";
    QString currentSelectionType = m_current ? m_current->typeName() : "";

    for ( int i = 0; i < m_files.size(); ++i ){
        QASTFile* root = m_files[i];
        if ( root->hasModifiers() ){
            root->save();
            QCSAConsole::log(QCSAConsole::Info1, "Saved: " + root->identifier());
            reparseIndex(i);
        }
    }

    select(currentSelection, currentSelectionType);
}

void QCodeBase::propagateUserCursor(int offset, const QString &file){
    CXTranslationUnit transUnit = m_classifiers.first()->translationUnit();
    CXFile cfile = clang_getFile(transUnit, file.toStdString().c_str());
    CXSourceLocation sLocation = clang_getLocationForOffset(transUnit, cfile, offset);
    propagateUserCursor(createSourceLocation(sLocation));
}

void QCodeBase::propagateUserCursor(int line, int column, const QString &file){
    CXTranslationUnit transUnit = m_classifiers.first()->translationUnit();
    CXFile cfile = clang_getFile(transUnit, file.toStdString().c_str());
    CXSourceLocation sLocation = clang_getLocation(transUnit, cfile, line, column);
    propagateUserCursor(createSourceLocation(sLocation));
}

void QCodeBase::propagateUserCursor(const QSourceLocation &location){
    QASTNode* deepest = m_current->propagateUserCursor(location);
    if ( deepest != 0 ){
        m_current = deepest;
        emit nodeSelected(m_current);
    }
}

bool QCodeBase::select(const QString &searchData, const QString &type){
    for ( QList<ast::QASTFile*>::iterator it = m_files.begin(); it != m_files.end(); ++it ){
        QASTFile* file = *it;
        QASTNode* foundChild = file->findFirst(searchData, type);
        if ( foundChild ){
            selectNode(foundChild);
            return true;
        }
    }
    return false;
}

bool QCodeBase::selectNode(QASTNode* node){
    if ( node != 0 ){
        m_current = node;
        emit nodeSelected(node);
        return true;
    }
    return false;
}

QList<QObject*> QCodeBase::find(const QString& searchData, const QString& type){
    QList<QObject*> foundNodes;

    for ( QList<ast::QASTFile*>::iterator it = m_files.begin(); it != m_files.end(); ++it ){
        QASTFile* file = *it;
        QList<QObject*> foundFileNodes = file->find(searchData, type);

        foundNodes << foundFileNodes;
    }

    return foundNodes;
}

void QCodeBase::setHeaderSearchPattern(const QStringList &pattern){
    m_headerSearchPatterns = pattern;
}

void QCodeBase::setSourceSearchPattern(const QStringList &pattern){
    m_sourceSearchPatterns = pattern;
}

QASTFile *QCodeBase::findSource(const QString &header){
    QString headerBaseName = QFileInfo(header).baseName();

    QString searchLocation = m_projectDir;
    if ( searchLocation == "" )
        searchLocation = QFileInfo(header).path();

    QDirIterator it(searchLocation, m_sourceSearchPatterns, QDir::Files, QDirIterator::Subdirectories);
    while (it.hasNext()){
        QString file = it.next();
        if ( it.fileInfo().baseName() == headerBaseName ){
            QASTFile* astFile = findFile(file);
            if ( !astFile )
                astFile = parseFile(file);
            return astFile;
        }
    }

    return 0;
}

QASTFile *QCodeBase::findHeader(const QString &source){
    QString sourceBaseName = QFileInfo(source).baseName();

    QString searchLocation = m_projectDir;
    if ( searchLocation == "" )
        searchLocation = QFileInfo(source).path();

    QDirIterator it(searchLocation, m_headerSearchPatterns, QDir::Files, QDirIterator::Subdirectories);
    while (it.hasNext()){
        QString file = it.next();
        if ( it.fileInfo().baseName() == sourceBaseName ){
            QASTFile* astFile = findFile(file);
            if ( !astFile )
                astFile = parseFile(file);
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

void QCodeBase::reparseIndex(int index){

    QASTFile* root               = m_files[index];
    QTokenClassifier* classifier = m_classifiers[index];
    CXTranslationUnit transUnit  = classifier->translationUnit();

    emit fileAboutToBeReparsed(root);

    clang_reparseTranslationUnit(transUnit, 0, 0, clang_defaultReparseOptions(transUnit));
    CXCursor startCursor = clang_getTranslationUnitCursor(transUnit);
    classifier->reparse();

    root->removeChildren();
    QASTVisitor::createCSANodeTree(startCursor, root, classifier);

    emit fileReparsed(root);
}

const QList<QASTFile*>& QCodeBase::astFiles() const{
    return m_files;
}

void QCodeBase::parsePath(const QString& path){
    QFileInfo finfo(path);
    if ( !finfo.exists() || finfo.isRelative() ){
        finfo = QFileInfo(m_projectDir + "/" + path);
        if ( !finfo.exists() ){
            QCSAConsole::logError(
                "Path \'" + path + "\' does not exist. Use createFile() or createDirectory() instead.");
        }
    }
    if ( finfo.isDir()){
        QStringList searchVals = QStringList() << m_headerSearchPatterns << m_sourceSearchPatterns;
        QDirIterator it(finfo.filePath(), searchVals, QDir::Files, QDirIterator::Subdirectories);
        while (it.hasNext()){
            it.next();
            parseFile(it.filePath());
        }
    } else {
        parseFile(finfo.filePath());
    }
}

QASTFile* QCodeBase::parseFile(const QString& file){
    Q_D(QCodeBase);

    QFileInfo finfo(file);
    if (!finfo.exists() || finfo.isRelative()){
        finfo = QFileInfo(m_projectDir + "/" + file);
        if ( !finfo.exists() ){
            QCSAConsole::logError("Path \'" + finfo.filePath() + "\' does not exist. Use createFile() instead.");
            return 0;
        }
    }
    if (finfo.isDir()){
        QCSAConsole::logError("Path \'" + finfo.filePath() + "\' is directory. Use parse() function instead.");
        return 0;
    }

    QString filePath = finfo.filePath();

    CXTranslationUnit transUnit  = clang_parseTranslationUnit(
                d->index,
                filePath.toStdString().c_str(),
                m_translationUnitArgs,
                m_translationUnitNumArgs,
                0,
                0,
                CXTranslationUnit_Incomplete | CXTranslationUnit_CXXChainedPCH);

    CXCursor startCursor  = clang_getTranslationUnitCursor(transUnit);
    CXFile   clangFile    = clang_getFile(transUnit, filePath.toStdString().c_str());

    QTokenClassifier* fileClassifier = new QTokenClassifier(transUnit, filePath.toStdString().c_str());
    unsigned int fileSize = QTokenClassifier::getFileSize(filePath.toStdString().c_str());

    QASTFile* fileRoot = new QASTFile(
        new QAnnotatedTokenSet(startCursor, transUnit),
        filePath,
        new QSourceLocation(createSourceLocation(clang_getLocationForOffset(
                transUnit,
                clangFile,
                fileSize
            ))
        )
    );
    fileRoot->setParent(this);

    m_files.append(fileRoot);
    m_classifiers.append(fileClassifier);

    QASTVisitor::createCSANodeTree(startCursor, fileRoot, fileClassifier);

    QCSAConsole::log(QCSAConsole::Info1, "Parsed: " + fileRoot->identifier());
    emit fileAdded(fileRoot);

    if ( !m_current )
        selectNode(fileRoot);

    return fileRoot;
}

QASTFile* QCodeBase::reparseFile(QASTFile* file){
    for ( int i = 0; i < m_files.size(); ++i ){

        if ( m_files[i] == file ){

            QASTNode* currentFile = m_current;
            while ( currentFile->astParent() )
                currentFile = currentFile->astParent();

            QString currentSelection = "", currentSelectionType = "";
            if ( qobject_cast<QASTFile*>(currentFile) == file ){
                currentSelection     = m_current ? m_current->breadcrumbs() : "*";
                currentSelectionType = m_current ? m_current->typeName() : "";
            }

            reparseIndex(i);

            if ( currentFile == file )
                select(currentSelection, currentSelectionType);

            return file;
        }
    }
    return 0;
}

QASTFile* QCodeBase::createFile(const QString& filePath){
    QFile file(filePath);
    if ( !file.open(QIODevice::WriteOnly) ){
        QCSAConsole::logError("Cannot create file \'" + filePath + "\'.");
        return 0;
    }
    return parseFile(filePath);
}

bool QCodeBase::makePath(const QString& path){
    QDir dir(path);
    if ( dir.isRelative() && m_projectDir == ""){
        QCSAConsole::logError(
            "Failed to create path. There's no project directory configured and path is not absolute.");
        return false;
    }
    if ( !QDir(m_projectDir).mkpath(path) ){
        QCSAConsole::logError("Failed to create path \'" + path + "\'%s.");
        return false;
    }
    return true;
}

QASTFile *QCodeBase::findFile(const QString &fileName){
    for ( QList<ast::QASTFile*>::const_iterator it = m_files.begin(); it != m_files.end(); ++it ){
        ast::QASTFile* file = *it;
        if ( file->identifier() == fileName )
            return file;
    }
    return 0;
}

QSourceLocation* QCodeBase::createLocation(const QString& file, unsigned int lineOrOffset, unsigned int column){
    for ( int i = 0; i < m_files.size(); ++i ){
        if ( m_files[i]->identifier() == file ){
            return m_files[i]->createLocation(lineOrOffset, column);
        }
    }
    return 0;
}

void QCodeBase::setProjectDir(const QString& path){
    QDir dir(path);
    if ( dir.isAbsolute() )
        m_projectDir = dir.path();
    else
        m_projectDir += "/" + dir.path();
}

}// namespace
