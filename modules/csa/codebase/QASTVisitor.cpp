/****************************************************************************
**
** Copyright (C) 2014 Dinu SV.
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


#include "QASTVisitor.hpp"
#include "QTokenClassifier.hpp"
#include "QAnnotatedTokenSet.hpp"
#include "QSourceLocation.hpp"
#include "QASTNode.hpp"
#include "QASTNamespace.hpp"
#include "QASTClass.hpp"
#include "QASTAccessSpecifier.hpp"
#include "QASTField.hpp"
#include "QASTMethod.hpp"
#include "QASTConstructor.hpp"
#include "QASTDestructor.hpp"

#include <QDebug>

namespace csa{
using namespace ast;


class VisitorClientData{

public:
    VisitorClientData( QTokenClassifier* classifier, QASTNode* node )
        : m_node(node)
        , m_classifier(classifier){
    }

    void      setNode(QASTNode* node){ m_node = node; }
    QASTNode* node(){ return m_node; }
    QTokenClassifier* classifier(){ return m_classifier; }

private:
    QASTNode*         m_node;
    QTokenClassifier* m_classifier;

};


QASTVisitor::QASTVisitor(){
}

void QASTVisitor::createCSANodeTree(const CXCursor &rootCursor, QASTNode *root, QTokenClassifier *classifier){
    VisitorClientData* data = new VisitorClientData(classifier, root);
    clang_visitChildren(rootCursor, QASTVisitor::callback, (CXClientData)data);
    delete data;
}

CXChildVisitResult QASTVisitor::callback(CXCursor cursor, CXCursor, CXClientData data){
    QASTNode* csanode            = static_cast<VisitorClientData*>(data)->node();
    QTokenClassifier* classifier = static_cast<VisitorClientData*>(data)->classifier();

    CXSourceLocation loc      = clang_getCursorLocation(cursor);
    CXSourceRange range       = clang_getCursorExtent(cursor);

    CXSourceLocation locStart = clang_getRangeStart(range);
    CXSourceLocation locEnd   = clang_getRangeEnd(range);

    // Skip cursors that are not in the same file
    CXFile cursorFile;
    clang_getSpellingLocation(loc, &cursorFile, 0, 0, 0);
    if ( cursorFile != clang_getFile(classifier->translationUnit(), classifier->file().c_str()) )
        return CXChildVisit_Continue;

    QAnnotatedTokenSet* cursorTokenSet = classifier->findTokenSet(cursor);
    if ( cursorTokenSet == 0 ){
        cursorTokenSet = new QAnnotatedTokenSet(cursor, classifier->translationUnit());
        classifier->appendTokenSet(cursorTokenSet);
    }

    switch(cursor.kind){
    case CXCursor_TypedefDecl :
        if ( csanode->parent() == 0 ){ //we're at root
        }
        break;
    case CXCursor_Namespace :
        csanode = new QASTNamespace(
                    cursorTokenSet,
                    new QSourceLocation(loc),
                    new QSourceLocation(locStart),
                    new QSourceLocation(locEnd),
                    csanode);
        break;
    case CXCursor_ClassDecl :
        csanode = new QASTClass(
                    cursorTokenSet,
                    new QSourceLocation(loc),
                    new QSourceLocation(locStart),
                    new QSourceLocation(locEnd),
                    csanode);
        break;
    case CXCursor_CXXAccessSpecifier :
        csanode = new QASTAccessSpecifier(
                    cursorTokenSet,
                    new QSourceLocation(loc),
                    new QSourceLocation(locStart),
                    new QSourceLocation(locEnd),
                    csanode);
        break;
    case CXCursor_FieldDecl :
        csanode = new QASTField(
                    cursorTokenSet,
                    new QSourceLocation(loc),
                    new QSourceLocation(locStart),
                    new QSourceLocation(locEnd),
                    csanode);
        break;
    case CXCursor_Constructor :
        csanode = new QASTConstructor(
                    cursorTokenSet,
                    classifier,
                    new QSourceLocation(loc),
                    new QSourceLocation(locStart),
                    new QSourceLocation(locEnd),
                    csanode);
        break;
    case CXCursor_Destructor :
        csanode = new QASTDestructor(
                    cursorTokenSet,
                    new QSourceLocation(loc),
                    new QSourceLocation(locStart),
                    new QSourceLocation(locEnd),
                    csanode);
        break;
    case CXCursor_CXXMethod :
        csanode = new QASTMethod(
                    cursorTokenSet,
                    classifier,
                    new QSourceLocation(loc),
                    new QSourceLocation(locStart),
                    new QSourceLocation(locEnd),
                    csanode);
        break;
    default:
        break;
    }

    VisitorClientData* visitorData = new VisitorClientData(classifier, csanode);
    clang_visitChildren(cursor, QASTVisitor::callback, visitorData);
    delete visitorData;

    return CXChildVisit_Continue;
}

}
