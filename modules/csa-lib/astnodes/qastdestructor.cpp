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


#include "qastdestructor.h"
#include "qastclass.h"
#include "qannotatedtoken_p.h"
#include "qannotatedtokenset.h"
#include "clang-c/Index.h"

namespace csa{ namespace ast{

QASTDestructor::QASTDestructor(
        QAnnotatedTokenSet *tokenSet,
        QSourceLocation* cursorLocation,
        QSourceLocation* rangeStartLocation,
        QSourceLocation* rangeEndLocation,
        QASTNode* parent)

    : QASTNode("destructor", tokenSet, cursorLocation, rangeStartLocation, rangeEndLocation, parent){


    if ( tokenSet->size() == 0 ){
        CXType type          = clang_getCursorType(tokenSet->cursor());
        CXString typeStr     = clang_getTypeSpelling(type);
        const char* cTypeStr = clang_getCString(typeStr);
        setIdentifier(cTypeStr);
        clang_disposeString(typeStr);
    }

    bool spaceFlag = false;
    for ( QAnnotatedTokenSet::Iterator it = tokenSet->begin(); it != tokenSet->end(); ++it ){
        CXToken t = (*it)->token().token;
        CXString tSpelling = clang_getTokenSpelling(tokenSet->translationUnit(), t);
        CXTokenKind tKind  = clang_getTokenKind(t);
        if ( spaceFlag ){
            setIdentifier(identifier() + " ");
            spaceFlag = false;
        }
        if ( tKind == CXToken_Punctuation ){
            setIdentifier(identifier() + clang_getCString(tSpelling));
            spaceFlag    = false;
        } else {
            setIdentifier(identifier() + clang_getCString(tSpelling));
            spaceFlag     = true;
        }
        clang_disposeString(tSpelling);
    }

}

}}// namespace
