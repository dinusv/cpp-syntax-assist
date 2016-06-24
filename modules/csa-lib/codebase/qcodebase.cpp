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


#include "qcodebase.h"
#include "qcodebaseconfig.h"
#include "qsourcelocation.h"
#include "qsourcelocation_p.h"
#include "qtokenclassifier.h"
#include "qannotatedtokenset.h"
#include "qastfield.h"
#include "qastnode.h"
#include "qastvisitor.h"
#include "qastsearch.h"
#include "qcsaconsole.h"
#include <QMap>
#include <QHash>

namespace csa{

using namespace ast;

class QCodebasePrivate{
public:
    CXIndex index;
};

QCodebase::QCodebase(
        QCodebaseConfig* config,
        const QStringList &entries,
        const QString&     searchDir,
        QObject*           parent)

    : QObject(parent)
    , d_ptr(new QCodebasePrivate)
    , m_config(config)
    , m_projectDir(searchDir != "" ? QDir(searchDir).path() : "")
{
    Q_D(QCodebase);

    if ( m_projectDir == "" ){
        if ( entries.size() > 0 ){
            QFileInfo entryInfo(entries[0]);
            if ( entryInfo.isAbsolute() )
                m_projectDir = entryInfo.path();
            else
                m_projectDir = QDir::currentPath();
        } else
            m_projectDir = QDir::currentPath();
    }

    d->index = clang_createIndex(0, 0);

    for ( QStringList::const_iterator it = entries.begin(); it != entries.end(); ++it ){
        parsePath(*it);
    }
}

QCodebase::~QCodebase(){
    delete m_config;
    delete d_ptr;
    for ( QList<csa::QTokenClassifier*>::Iterator it = m_classifiers.begin(); it != m_classifiers.end(); ++it )
        delete *it;
    m_classifiers.clear();
}

void QCodebase::save(){
    for ( int i = 0; i < m_files.size(); ++i ){
        QASTFile* root = m_files[i];
        if ( root->hasModifiers() ){
            root->save();
            QCSAConsole::log(QCSAConsole::Info1, "Saved: " + root->identifier());
            reparseIndex(i);
        }
    }
}

QASTNode *QCodebase::nodeAt(const QString &file, int lineOrOffset, int column){
    for ( int i = 0; i < m_files.size(); ++i ){
        QASTFile* root = m_files[i];
        if ( root->identifier() == file ){
            QSourceLocation* location = root->createLocation(lineOrOffset, column);
            QASTNode* node = root->nodeAt(*location);
            delete location;
            return node;
        }
    }
    return 0;
}

QList<QObject*> QCodebase::find(const QString& searchData, const QString& type){
    QList<QObject*> foundNodes;

    for ( QList<ast::QASTFile*>::iterator it = m_files.begin(); it != m_files.end(); ++it ){
        QASTFile* file = *it;
        QList<QObject*> foundFileNodes = file->find(searchData, type);

        foundNodes << foundFileNodes;
    }

    return foundNodes;
}

QASTFile *QCodebase::findSource(const QString &header){
    QString headerBaseName = QFileInfo(header).baseName();

    QString searchLocation = m_projectDir;
    if ( searchLocation == "" )
        searchLocation = QFileInfo(header).path();

    QDirIterator it(searchLocation, m_config->sourceFilePatterns(), QDir::Files, QDirIterator::Subdirectories);
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

QASTFile *QCodebase::findHeader(const QString &source){
    QString sourceBaseName = QFileInfo(source).baseName();

    QString searchLocation = m_projectDir;
    if ( searchLocation == "" )
        searchLocation = QFileInfo(source).path();

    QDirIterator it(searchLocation, m_config->headerFilePatterns(), QDir::Files, QDirIterator::Subdirectories);
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

QTokenClassifier *QCodebase::classifierForFile(const QString &file){
    for ( int i = 0; i < m_files.size(); ++i ){
        if ( m_files[i]->identifier() == file ){
            return m_classifiers[i];
        }
    }
    return 0;
}

void QCodebase::reparseIndex(int index){

    QASTFile* root               = m_files[index];
    QTokenClassifier* classifier = m_classifiers[index];
    CXTranslationUnit transUnit  = classifier->translationUnit();

    emit fileAboutToBeReparsed(root);

    clang_reparseTranslationUnit(transUnit, 0, 0, clang_defaultReparseOptions(transUnit));
    CXCursor startCursor = clang_getTranslationUnitCursor(transUnit);
    classifier->reparse();

    root->removeChildren();
    QASTVisitor::createCSANodeTree(startCursor, root, classifier);
    root->reparseSize();

    emit fileReparsed(root);
}

const QList<QASTFile*>& QCodebase::astFiles() const{
    return m_files;
}

void QCodebase::parsePath(const QString& path){
    QFileInfo finfo(path);
    if ( !finfo.exists() || finfo.isRelative() ){
        finfo = QFileInfo(m_projectDir + "/" + path);
        if ( !finfo.exists() ){
            QCSAConsole::logError(
                "Path \'" + path + "\' does not exist. Use createFile() or createDirectory() instead.");
        }
    }
    if ( finfo.isDir()){
        QStringList searchVals = QStringList() << m_config->headerFilePatterns() << m_config->sourceFilePatterns();
        QDirIterator it(finfo.filePath(), searchVals, QDir::Files, QDirIterator::Subdirectories);
        while (it.hasNext()){
            it.next();
            parseFile(it.filePath());
        }
    } else {
        parseFile(finfo.filePath());
    }
}

QASTFile* QCodebase::parseFile(const QString& file){
    Q_D(QCodebase);

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
                m_config->translationUnitArgs(),
                m_config->translationUnitNumArgs(),
                0,
                0,
                CXTranslationUnit_Incomplete | CXTranslationUnit_CXXChainedPCH);

    if ( transUnit == 0 ){
        QCSAConsole::logError("Failed to parse translation unit on file: " + finfo.filePath());
        return 0;
    }

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

    return fileRoot;
}

QASTFile* QCodebase::reparseFile(QASTFile* file){
    for ( int i = 0; i < m_files.size(); ++i ){
        if ( m_files[i] == file ){
            reparseIndex(i);
            return file;
        }
    }
    return 0;
}

void QCodebase::reparse(){
    for ( int i = 0; i < m_files.size(); ++i )
        reparseIndex(i);
}

QASTFile* QCodebase::createFile(const QString& filePath){
    QFile file(filePath);
    if ( !file.open(QIODevice::WriteOnly) ){
        QCSAConsole::logError("Cannot create file \'" + filePath + "\'.");
        return 0;
    }
    return parseFile(filePath);
}

bool QCodebase::deleteFile(QASTFile *file){
    for ( int i = 0; i < m_files.size(); ++i ){
        if ( m_files[i] == file ){
            emit fileAboutToBeDeleted(file);
            delete m_classifiers[i];
            m_classifiers.removeAt(i);
            m_files.removeAt(i);
            file->deleteLater();
            if ( QFile(file->identifier()).remove() ){
                return true;
            } else {
                return false;
            }
        }
    }
    return false;
}

bool QCodebase::makePath(const QString& path){
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

QASTFile *QCodebase::findFile(const QString &fileName){
    for ( QList<ast::QASTFile*>::const_iterator it = m_files.begin(); it != m_files.end(); ++it ){
        ast::QASTFile* file = *it;
        if ( file->identifier() == fileName )
            return file;
    }
    return 0;
}

QSourceLocation* QCodebase::createLocation(const QString& file, unsigned int lineOrOffset, unsigned int column){
    for ( int i = 0; i < m_files.size(); ++i ){
        if ( m_files[i]->identifier() == file ){
            return m_files[i]->createLocation(lineOrOffset, column);
        }
    }
    return 0;
}

void QCodebase::setProjectDir(const QString& path){
    QDir dir(path);
    if ( dir.isAbsolute() )
        m_projectDir = dir.path();
    else
        m_projectDir += "/" + dir.path();
}

}// namespace
