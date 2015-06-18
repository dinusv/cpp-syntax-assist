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


#include "QASTField.hpp"
#include "QASTClass.hpp"
#include "QAnnotatedTokenSet.hpp"
#include "QSourceLocation.hpp"
#include "clang-c/Index.h"

#include <QDebug>

namespace csa{ namespace ast{

QASTField::QASTField(
        QAnnotatedTokenSet *tokenSet,
        QSourceLocation* cursorLocation,
        QSourceLocation* rangeStartLocation,
        QSourceLocation* rangeEndLocation,
        QASTNode* parent)

    : QASTNode("field", tokenSet, cursorLocation, rangeStartLocation, rangeEndLocation, parent){

    // Get Identifier
    // --------------

    CXString id = clang_getCursorSpelling(tokenSet->cursor());
    setIdentifier(clang_getCString(id));

    // Get Field Type
    // ---------------

    CXType type           = clang_getCursorType(tokenSet->cursor());
    CXString typeSpelling = clang_getTypeSpelling(type);
    m_fieldType           = clang_getCString(typeSpelling);
    clang_disposeString(typeSpelling);

    // Determine field type
    // --------------------

    if ( type.kind == CXType_Unexposed || type.kind == CXType_Invalid || m_fieldType.contains("int") ){
        m_fieldType = "";

        bool doubleColonFlag = false;
        for ( QAnnotatedTokenSet::Iterator it = tokenSet->begin(); it != tokenSet->end(); ++it ){

            CXToken t              = (*it)->token();
            CXString tSpelling     = clang_getTokenSpelling(tokenSet->translationUnit(), t);
            const char* tCSpelling = clang_getCString(tSpelling);
            CXTokenKind tKind      = clang_getTokenKind(t);

            if ( tKind == CXToken_Identifier && std::string(clang_getCString(id)) == tCSpelling ){
                break;
            } else if ( tKind == CXToken_Punctuation && std::string("::") == tCSpelling ){
                doubleColonFlag = true;
                m_fieldType    += tCSpelling;
            } else if ( ( tKind == CXToken_Identifier || tKind == CXToken_Keyword ) &&
                        !m_fieldType.isEmpty() && !doubleColonFlag ){
                m_fieldType    += QString(" ") + tCSpelling;
            } else {
                doubleColonFlag = false;
                m_fieldType    += tCSpelling;
            }
        }
    }

    clang_disposeString(id);
}

QString QASTField::prop(const QString &key) const{
    if ( key == "type" )
        return m_fieldType;
    return QASTNode::prop(key);
}

}}// namespace
