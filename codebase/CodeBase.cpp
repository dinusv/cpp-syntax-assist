#include "CodeBase.hpp"
#include "SyntaxTreeModel.hpp"
#include "SourceLocation.hpp"
#include "TokenClassifier.hpp"
#include "CSAFileRoot.hpp"
#include "CSANode.hpp"
#include "Visitor.hpp"
#include <QMap>
#include <QHash>
#include <QDebug>

using namespace csa;
using namespace csa::ast;

namespace helpers{

void insertValues( const QHash<QString, QMap<unsigned int, QString> >& files ){
    QHash<QString, QMap<unsigned int, QString> >::const_iterator it;
    for( it = files.constBegin(); it != files.constEnd(); ++it ){ // for each file

        QFile file(it.key());
        if ( !file.open(QIODevice::ReadOnly) ){
            qCritical() << "Cannot open file '" << it.key() << "' for reading.";
            continue;
        }

        QString fileData = QTextStream(&file).readAll();
        file.close();
        if ( !file.open(QIODevice::WriteOnly | QIODevice::Truncate) ){
            qCritical() << "Cannot open file '" << it.key() << "' for writing.";
            continue;
        }
        QTextStream writeStream(&file);
        QMap<unsigned int, QString>::const_iterator fit;
        int startCut, endCut = 0;
        for ( fit = it.value().begin(); fit != it.value().end(); ++fit ){
            startCut = endCut;
            endCut = fit.key();
            writeStream << fileData.mid(startCut, endCut - startCut) << fit.value();
        }
        writeStream << fileData.mid(endCut);
    }
}

}// namespace helpers

CodeBase::CodeBase(
        const char* const* translationUnitArgs,
        int                translationUnitNumArgs,
        const QString&     file,
        SyntaxTreeModel*   treeModel,
        QObject*           parent)

    : QObject(parent)
    , m_root(0)
    , m_current(0)
    , m_treeModel(treeModel)
    , m_classifier(0){

    CXIndex index = clang_createIndex(0, 0);
    CXTranslationUnit transUnit  = clang_parseTranslationUnit(
                index, file.toStdString().c_str(),
                translationUnitArgs,
                translationUnitNumArgs,
                0, 0,
                CXTranslationUnit_Incomplete | CXTranslationUnit_CXXChainedPCH);

    CXCursor startCursor  = clang_getTranslationUnitCursor(transUnit);
    CXFile   clangFile    = clang_getFile(transUnit, file.toStdString().c_str());

    m_classifier = new TokenClassifier(transUnit, file.toStdString().c_str());
    m_root       = new CSAFileRoot(
                file.toStdString(),
                new SourceLocation(clang_getLocationForOffset(transUnit, clangFile, TokenClassifier::getFileSize(file.toStdString().c_str()))));
    m_current    = m_root;

    Visitor::createCSANodeTree(startCursor, m_root, m_classifier);

    if( m_treeModel ){
        m_treeModel->parse(m_root);
        m_treeModel->setSelected(m_current);
    }
}

void CodeBase::reparse(){
    QString selectionTypeString = QString::fromStdString(m_current->typeString());
    QString selectionIdentifier = QString::fromStdString(m_current->identifier());

    CXTranslationUnit transUnit = m_classifier->translatinoUnit();
    clang_reparseTranslationUnit(transUnit, 0, 0, clang_defaultReparseOptions(transUnit));
    CXCursor startCursor        = clang_getTranslationUnitCursor(transUnit);
    m_classifier->reparse();
    if ( m_treeModel )
        m_treeModel->safeClear();
    m_root->removeChildren();
    Visitor::createCSANodeTree(startCursor, m_root, m_classifier);
    m_current = m_root;
    if ( m_treeModel ){
        m_treeModel->parse(m_root);
        m_treeModel->setSelected(m_current);
    }
    select(selectionTypeString, selectionIdentifier);
}

void CodeBase::propagateUserCursor(int offset, const QString &file){
    CXTranslationUnit transUnit = m_classifier->translatinoUnit();
    CXFile cfile = clang_getFile(transUnit, file.toStdString().c_str());
    CXSourceLocation sLocation = clang_getLocationForOffset(transUnit, cfile, offset);
    propagateUserCursor(SourceLocation(sLocation));
}

void CodeBase::propagateUserCursor(int line, int column, const QString &file){
    CXTranslationUnit transUnit = m_classifier->translatinoUnit();
    CXFile cfile = clang_getFile(transUnit, file.toStdString().c_str());
    CXSourceLocation sLocation = clang_getLocation(transUnit, cfile, line, column);
    propagateUserCursor(SourceLocation(sLocation));
}

void CodeBase::propagateUserCursor(const SourceLocation &location){
    CSANode* deepest = m_root->propagateUserCursor(location);
    if ( deepest != 0 ){
        std::string dump;
        deepest->dump(dump);
        m_current = deepest;
        if ( m_treeModel )
            m_treeModel->setSelected(m_current);
    }
}

bool CodeBase::select(const QString &typeString, const QString &name){
    if ( typeString == "root" ){
        m_current = m_root;
        if ( m_treeModel )
            m_treeModel->setSelected(m_current);
        return true;
    } else if ( typeString == "up" ){
        CSANode* result = m_current->parent();
        if ( result ){
            m_current = result;
            if ( m_treeModel )
                m_treeModel->setSelected(m_current);
            return true;
        }
    } else {
        CSANode* result = m_current->find(typeString.toStdString(), name.toStdString());
        if ( result ){
            m_current = result;
            if ( m_treeModel )
                m_treeModel->setSelected(m_current);
            return true;
        }
    }
    return false;
}

/*
  Each CSANode contains a file, and an offset within that file where the positioning of cerain
  elements can be found. Therefore I create the following map of strings I need to modify each
  file :
  QHash< QString(file), QMap< int(offset), QString(insertion_text) > >
 */
void CodeBase::insert(const QScriptValue &value){
    QHash<QString, QMap<unsigned int, QString> > fileHash;
    QScriptValueIterator it(value);
    while ( it.hasNext() ){
        it.next();
        const SourceLocation* itLocation = m_current->location(it.name().toStdString());
        if ( itLocation == 0 ){
            qCritical() << "Location '" << it.name() << "' does not exist. No data will be inserted.";
            continue;
        }
        QString file        = QString::fromStdString(itLocation->filePath());
        unsigned int offset = itLocation->offset();
        fileHash[file][offset] = it.value().toString();
    }
    helpers::insertValues(fileHash);
    reparse();
}

QString CodeBase::nodeIdentifier() const{
    return QString::fromStdString(m_current->identifier());
}
