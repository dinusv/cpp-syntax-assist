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
#include "AnnotatedTokenSet.hpp"
#include "QASTClass.hpp"

namespace csa{ namespace ast{

QASTAccessSpecifier::QASTAccessSpecifier(
        AnnotatedTokenSet* tokenSet,
        SourceLocation* cursorLocation,
        SourceLocation* rangeStartLocation,
        SourceLocation* rangeEndLocation,
        QASTNode *parent)

    : QASTNode("access", cursorLocation, rangeStartLocation, rangeEndLocation, parent){

    switch( clang_getCXXAccessSpecifier(tokenSet->cursor()) ){
    case CX_CXXInvalidAccessSpecifier :
        m_typeValueName = "invalid";
        break;
    case CX_CXXPrivate :
        m_typeValueName = "private";
        break;
    case CX_CXXPublic :
        m_typeValueName = "public";
        break;
    case CX_CXXProtected :
        m_typeValueName = "protected";
        break;
    }

    // set class offsets
    if ( parent != 0 ){
        if ( parent->typeString() == "class" ){
            QASTClass* node = static_cast<QASTClass*>(parent);

            switch( clang_getCXXAccessSpecifier(tokenSet->cursor()) ){
            case CX_CXXInvalidAccessSpecifier :
                break;
            case CX_CXXPrivate :
                node->setChildLocation(QASTClass::PRIVATE,   *rangeStartLocation);
                break;
            case CX_CXXPublic :
                node->setChildLocation(QASTClass::PUBLIC,    *rangeStartLocation);
                break;
            case CX_CXXProtected :
                node->setChildLocation(QASTClass::PROTECTED, *rangeStartLocation);
                break;
            } // switch
        }
    }

}

}}// namespace
