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

    : QASTNode("destructor", tokenSet, cursorLocation, rangeStartLocation, rangeEndLocation, parent)
    , m_isVirtual(clang_CXXMethod_isVirtual(tokenSet->cursor()))
{
    CXString id = clang_getCursorSpelling(tokenSet->cursor());
    setIdentifier(clang_getCString(id));
    clang_disposeString(id);
}

}}// namespace
