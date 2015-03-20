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


#include "QASTMethod.hpp"
#include "QASTMethodArgument.hpp"
#include "QAnnotatedTokenSet.hpp"
#include "QTokenClassifier.hpp"
#include "QSourceLocation.hpp"
#include "clang-c/Index.h"

namespace csa{ namespace ast{

QASTMethod::QASTMethod(
        QAnnotatedTokenSet *tokenSet,
        QTokenClassifier* classifier,
        QSourceLocation* cursorLocation,
        QSourceLocation* rangeStartLocation,
        QSourceLocation* rangeEndLocation,
        QASTNode* parent)

    : QASTNode("method", tokenSet, cursorLocation, rangeStartLocation, rangeEndLocation, parent)
    , m_isStatic(false)
    , m_isVirtual(false)
{

    // Get Identifier

    CXString id = clang_getCursorSpelling(tokenSet->cursor());
    setIdentifier(clang_getCString(id));

    // Get Return Type

    QAnnotatedTokenSet::Iterator it = tokenSet->begin();
    while ( it != tokenSet->end() ){
        CXToken t = (*it)->token();
        CXString tSpelling = clang_getTokenSpelling(tokenSet->translationUnit(), t);
        const char* tCSpelling = clang_getCString(tSpelling);
        CXTokenKind tKind  = clang_getTokenKind(t);

        if ( tKind == CXToken_Identifier && std::string(clang_getCString(id)) == tCSpelling )
            break;

        if ( tKind == CXToken_Keyword && std::string("virtual") == tCSpelling ){
            m_isVirtual = true;
            ++it;
            continue;
        }

        if ( ( tKind == CXToken_Identifier || tKind == CXToken_Keyword ) && !m_returnType.isEmpty() )
            m_returnType += " ";

        m_returnType += tCSpelling;

        clang_disposeString(tSpelling);
        ++it;
    }

    // Check Static

    m_isStatic = clang_CXXMethod_isStatic(tokenSet->cursor());

    // Get Arguments

    int numArguments = clang_Cursor_getNumArguments(tokenSet->cursor());
    for( int i = 0; i < numArguments; ++i ){
        CXCursor argCursor = clang_Cursor_getArgument(tokenSet->cursor(), i);

        QAnnotatedTokenSet* argTokenSet = classifier->findTokenSet(argCursor);
        if ( argTokenSet == 0 ){
            argTokenSet = new QAnnotatedTokenSet(argCursor, classifier->translationUnit());
            classifier->appendTokenSet(argTokenSet);
        }

        CXSourceLocation loc = clang_getCursorLocation(argCursor);
        CXSourceRange range  = clang_getCursorExtent(argCursor);

        CXSourceLocation locStart = clang_getRangeStart(range);
        CXSourceLocation locEnd   = clang_getRangeEnd(range);

        QASTMethodArgument* argument = new QASTMethodArgument(
            argTokenSet,
            new QSourceLocation(loc),
            new QSourceLocation(locStart),
            new QSourceLocation(locEnd),
            0
        );
        argument->setParent(this);

        m_arguments.append(argument);
    }

    clang_disposeString(id);

    return;
}

QList<QASTNode*> QASTMethod::arguments() const{
    return m_arguments;
}

QString QASTMethod::content() const{
    QString base = "";
    base += m_isStatic ? "static " : "";
    base += m_isVirtual ? "virtual " : "";
    base += m_returnType + " ";
    base += identifier();
    base += "(";
    for ( QList<csa::ast::QASTNode*>::const_iterator it = m_arguments.begin(); it != m_arguments.end(); ++it ){
        if ( base != "" )
            base += " ";
        base += (*it)->content();
    }
    base += ")";
    base += m_isConst ? " const" : "";
    base += m_isPureVirtual ? " = 0" : "";

    return base;
}

}}// namespace
