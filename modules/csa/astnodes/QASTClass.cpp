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


#include "QASTClass.hpp"
#include "QSourceLocation.hpp"
#include "QAnnotatedTokenSet.hpp"

namespace csa{ namespace ast{

QASTClass::QASTClass(
        QAnnotatedTokenSet* tokenSet,
        QSourceLocation* cursorLocation,
        QSourceLocation* rangeStartLocation,
        QSourceLocation* rangeEndLocation,
        QASTNode* parent)

    : QASTNode("class", tokenSet, cursorLocation, rangeStartLocation, rangeEndLocation, parent)
    , m_bodyStartSet(false)
{

    m_bodyStart = new QSourceLocation(*rangeStartLocation);
    m_bodyEnd   = new QSourceLocation(*rangeEndLocation);

    // Find '{' and '}' in token set
    for ( QAnnotatedTokenSet::Iterator it = tokenSet->begin(); it != tokenSet->end(); ++it ){
        QAnnotatedToken* aToken = *it;
        CXTokenKind tokenKind = clang_getTokenKind(aToken->token());

        if ( tokenKind == CXToken_Punctuation ){
            CXString tokenString           = clang_getTokenSpelling(tokenSet->translationUnit(), aToken->token());
            std::string tokenStdString     = clang_getCString(tokenString);
            CXSourceLocation tokenLocation = clang_getTokenLocation(tokenSet->translationUnit(), aToken->token());
            if ( tokenStdString == "{" && !m_bodyStartSet ){
                QSourceLocation* sl = m_bodyStart;
                sl->assign(tokenLocation);
                sl->assign(sl->offset() + 1, tokenSet->translationUnit());
                m_bodyStartSet = true;

            } else if ( tokenStdString == "}" ){
                QSourceLocation* sl = m_bodyEnd;
                sl->assign(tokenLocation);

            }
            clang_disposeString(tokenString);
        }
    }

    // Get name
    CXString name = clang_getCursorDisplayName(tokenSet->cursor());
    const char* cName = clang_getCString(name);
    setIdentifier(cName);
    clang_disposeString(name);

}

QASTClass::~QASTClass(){
}

QASTNode *QASTClass::propagateUserCursor(const QSourceLocation &location){
    QASTNode* base = this;
    if ( rangeStartLocation().offset() <= location.offset() && rangeEndLocation().offset() >= location.offset() ){
        //m_offsets[QASTClass::CURSOR] = new SourceLocation(location);
        QASTNode* result = QASTNode::propagateUserCursor(location);
        if ( result != 0 )
            base = result;
    }
    return base;
}

}}// namespace
