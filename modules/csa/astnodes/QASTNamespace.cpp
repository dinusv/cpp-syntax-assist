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


#include "QASTNamespace.hpp"
#include "QAnnotatedTokenSet.hpp"
#include "QSourceLocation.hpp"
#include "QSourceLocation_p.hpp"
#include "clang-c/Index.h"

#include <QDebug>

namespace csa{ namespace ast{

QASTNamespace::QASTNamespace(
        QAnnotatedTokenSet *tokenSet,
        QSourceLocation* cursorLocation,
        QSourceLocation* rangeStartLocation,
        QSourceLocation* rangeEndLocation,
        QASTNode *parent)
    : QASTNode("namespace", tokenSet, cursorLocation, rangeStartLocation, rangeEndLocation, parent ){

    // Set default offsets
    m_bodyStart = new QSourceLocation(*rangeStartLocation);
    m_bodyEnd   = new QSourceLocation(*rangeEndLocation);
//    m_offsets[QASTNamespace::CURSOR] = 0;

    // Find '{' and '}' in token set
    for ( QAnnotatedTokenSet::Iterator it = tokenSet->begin(); it != tokenSet->end(); ++it ){
        CXTokenKind tokenKind = clang_getTokenKind((*it)->token());

        if ( tokenKind == CXToken_Punctuation ){
            CXString tokenString           = clang_getTokenSpelling(tokenSet->translationUnit(), (*it)->token());
            std::string tokenStdString     = clang_getCString(tokenString);
            CXSourceLocation tokenLocation = clang_getTokenLocation(tokenSet->translationUnit(), (*it)->token());
            if ( tokenStdString == "{" ){
                QSourceLocation* sl = m_bodyStart;
                sl->assign(createSourceLocation(tokenLocation));
                sl->assign(createSourceLocation(sl->filePath(), sl->offset() + 1, tokenSet->translationUnit()));
            } else if ( tokenStdString == "}" ){
                QSourceLocation* sl = m_bodyEnd;
                sl->assign(createSourceLocation(tokenLocation));
            }
            clang_disposeString(tokenString);
        }
    }

    CXString name = clang_getCursorDisplayName(tokenSet->cursor());
    const char* cName = clang_getCString(name);
    setIdentifier(cName);
    clang_disposeString(name);
}

QASTNamespace::~QASTNamespace(){
}


QASTNode *QASTNamespace::propagateUserCursor(const QSourceLocation &location){
    QASTNode* base = this;
    if ( rangeStartLocation()->offset() <= location.offset() && rangeEndLocation()->offset() >= location.offset() ){
        QASTNode* result = QASTNode::propagateUserCursor(location);
        if ( result != 0 )
            base = result;
    }
    return base;
}

}}
