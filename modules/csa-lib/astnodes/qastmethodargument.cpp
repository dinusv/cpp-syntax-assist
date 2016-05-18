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


#include "qastmethodargument.h"
#include "qannotatedtoken.h"
#include "qannotatedtoken_p.h"
#include "qannotatedtokenset.h"
#include "clang-c/Index.h"

namespace csa{ namespace ast{

QASTMethodArgument::QASTMethodArgument(
        QAnnotatedTokenSet* tokenSet,
        QSourceLocation* cursorLocation,
        QSourceLocation* rangeStartLocation,
        QSourceLocation* rangeEndLocation,
        QASTNode* parent)
    : QASTNode("argument", tokenSet, cursorLocation, rangeStartLocation, rangeEndLocation, parent)
{

    // Get Identifier
    // --------------

    CXString id = clang_getCursorSpelling(tokenSet->cursor());
    setIdentifier(clang_getCString(id));

    // Get Field Type
    // ---------------

    CXType type           = clang_getCursorType(tokenSet->cursor());
    CXString typeSpelling = clang_getTypeSpelling(type);
    m_type           = clang_getCString(typeSpelling);
    clang_disposeString(typeSpelling);

    // Determine argument type
    // -----------------------

    if ( type.kind == CXType_Unexposed || type.kind == CXType_Invalid || m_type.contains("int") ){
        m_type = "";

        bool doubleColonFlag = false;
        for ( QAnnotatedTokenSet::Iterator it = tokenSet->begin(); it != tokenSet->end(); ++it ){

            CXToken t              = (*it)->token().token;
            CXString tSpelling     = clang_getTokenSpelling(tokenSet->translationUnit(), t);
            const char* tCSpelling = clang_getCString(tSpelling);
            CXTokenKind tKind      = clang_getTokenKind(t);

            if ( tKind == CXToken_Identifier && std::string(clang_getCString(id)) == tCSpelling ){
                break;
            } else if ( tKind == CXToken_Punctuation && std::string("::") == tCSpelling ){
                doubleColonFlag = true;
                m_type    += tCSpelling;
            } else if ( ( tKind == CXToken_Identifier || tKind == CXToken_Keyword ) &&
                        !m_type.isEmpty() && !doubleColonFlag ){
                m_type    += QString(" ") + tCSpelling;
            } else {
                doubleColonFlag = false;
                m_type    += tCSpelling;
            }
        }
    }

    clang_disposeString(id);
}

QASTMethodArgument::~QASTMethodArgument(){
}

QString QASTMethodArgument::description() const{
    return m_type + " " + identifier();
}

QString QASTMethodArgument::prop(const QString& key) const{
    if ( key == "type" ){
        return m_type;
    }
    return QASTNode::prop(key);
}

}} // namespace ast, csa
