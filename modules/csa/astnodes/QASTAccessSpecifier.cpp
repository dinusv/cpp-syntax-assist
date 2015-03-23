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


#include "QASTAccessSpecifier.hpp"
#include "QAnnotatedTokenSet.hpp"
#include "QASTClass.hpp"

namespace csa{ namespace ast{

QASTAccessSpecifier::QASTAccessSpecifier(
        QAnnotatedTokenSet* tokenSet,
        QSourceLocation* cursorLocation,
        QSourceLocation* rangeStartLocation,
        QSourceLocation* rangeEndLocation,
        QASTNode *parent)

    : QASTNode("access", tokenSet, cursorLocation, rangeStartLocation, rangeEndLocation, parent){

    switch( clang_getCXXAccessSpecifier(tokenSet->cursor()) ){
    case CX_CXXInvalidAccessSpecifier :
        setIdentifier("invalid");
        break;
    case CX_CXXPrivate :
        setIdentifier("private");
        break;
    case CX_CXXPublic :
        setIdentifier("public");
        break;
    case CX_CXXProtected :
        setIdentifier("protected");
        break;
    }

}

}}// namespace
