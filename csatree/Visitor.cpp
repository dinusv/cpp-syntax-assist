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


#include "Visitor.hpp"
#include "TokenClassifier.hpp"
#include "AnnotatedTokenSet.hpp"
#include "SourceLocation.hpp"
#include "CSANode.hpp"
#include "CSANamespace.hpp"
#include "CSAClass.hpp"
#include "CSAAccessSpecifier.hpp"
#include "CSAField.hpp"
#include "CSAMethod.hpp"
#include "CSAConstructor.hpp"
#include "CSADestructor.hpp"

#include <QDebug>

namespace csa{
using namespace ast;

class VisitorClientData{

public:
    VisitorClientData( TokenClassifier* classifier, CSANode* node )
        : m_node(node)
        , m_classifier(classifier){
    }

    void     setNode(CSANode* node){ m_node = node; }
    CSANode* node(){ return m_node; }
    TokenClassifier* classifier(){ return m_classifier; }

private:
    CSANode*         m_node;
    TokenClassifier* m_classifier;

};

Visitor::Visitor(){
}

void Visitor::createCSANodeTree(const CXCursor &rootCursor, CSANode *root, TokenClassifier *classifier){
    VisitorClientData* data = new VisitorClientData(classifier, root);
    clang_visitChildren(rootCursor, Visitor::callback, (CXClientData)data);
    delete data;
}

CXChildVisitResult Visitor::callback(CXCursor cursor, CXCursor, CXClientData data){
    CSANode* csanode            = static_cast<VisitorClientData*>(data)->node();
    TokenClassifier* classifier = static_cast<VisitorClientData*>(data)->classifier();

    CXSourceLocation loc = clang_getCursorLocation(cursor);
    CXSourceRange range  = clang_getCursorExtent(cursor);

    CXSourceLocation locStart = clang_getRangeStart(range);
    CXSourceLocation locEnd   = clang_getRangeEnd(range);

    // Skip cursors that are not in the same file
    CXFile cursorFile;
    clang_getSpellingLocation(loc, &cursorFile, 0, 0, 0);
    if ( cursorFile != clang_getFile(classifier->translatinoUnit(), classifier->file().c_str()) )
        return CXChildVisit_Continue;

    AnnotatedTokenSet* cursorTokenSet = classifier->findTokenSet(cursor);
    if ( cursorTokenSet == 0 )
        cursorTokenSet = new AnnotatedTokenSet(cursor, classifier->translatinoUnit());

    switch(cursor.kind){
    case CXCursor_TypedefDecl :
        if ( csanode->parent() == 0 ){ //we're at root
        }
        break;
    case CXCursor_Namespace :
        csanode = new CSANamespace(cursorTokenSet, new SourceLocation(loc), new SourceLocation(locStart), new SourceLocation(locEnd), csanode);
        break;
    case CXCursor_ClassDecl :
        csanode = new CSAClass(cursorTokenSet, new SourceLocation(loc), new SourceLocation(locStart), new SourceLocation(locEnd), csanode);
        break;
    case CXCursor_CXXAccessSpecifier :
        csanode = new CSAAccessSpecifier(cursorTokenSet, new SourceLocation(loc), new SourceLocation(locStart), new SourceLocation(locEnd), csanode);
        break;
    case CXCursor_FieldDecl :
        csanode = new CSAField(cursorTokenSet, new SourceLocation(loc), new SourceLocation(locStart), new SourceLocation(locEnd), csanode);
        break;
    case CXCursor_Constructor :
        csanode = new CSAConstructor(cursorTokenSet, classifier, new SourceLocation(loc), new SourceLocation(locStart), new SourceLocation(locEnd), csanode);
        break;
    case CXCursor_Destructor :
        csanode = new CSADestructor(cursorTokenSet, new SourceLocation(loc), new SourceLocation(locStart), new SourceLocation(locEnd), csanode);
        break;
    case CXCursor_CXXMethod :
        csanode = new CSAMethod(cursorTokenSet, classifier, new SourceLocation(loc), new SourceLocation(locStart), new SourceLocation(locEnd), csanode);
        break;
    default:
        break;
    }

    VisitorClientData* visitorData = new VisitorClientData(classifier, csanode);
    clang_visitChildren(cursor, Visitor::callback, visitorData);
    delete visitorData;

    return CXChildVisit_Continue;
}

}
