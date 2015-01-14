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


#include "CSANamespace.hpp"
#include "AnnotatedTokenSet.hpp"
#include "SourceLocation.hpp"
#include "clang-c/Index.h"

#include <QDebug>

namespace csa{ namespace ast{

const QASTNode::OffsetKeyMap QASTNamespace::OFFSET_KEYS = QASTNamespace::createOffsetKeys();

QASTNamespace::QASTNamespace(
        AnnotatedTokenSet *tokenSet,
        SourceLocation* cursorLocation,
        SourceLocation* rangeStartLocation,
        SourceLocation* rangeEndLocation,
        QASTNode *parent)
    : QASTNode("namespace", cursorLocation, rangeStartLocation, rangeEndLocation, parent ){

    // Set default offsets
    m_offsets[QASTNamespace::BEGIN]  = new SourceLocation(*rangeStartLocation);
    m_offsets[QASTNamespace::END]    = new SourceLocation(*rangeEndLocation);
    m_offsets[QASTNamespace::CURSOR] = 0;

    // Find '{' and '}' in token set
    for ( AnnotatedTokenSet::Iterator it = tokenSet->begin(); it != tokenSet->end(); ++it ){
        CXTokenKind tokenKind = clang_getTokenKind(*it);

        if ( tokenKind == CXToken_Punctuation ){
            CXString tokenString           = clang_getTokenSpelling(tokenSet->translationUnit(), *it);
            std::string tokenStdString     = clang_getCString(tokenString);
            CXSourceLocation tokenLocation = clang_getTokenLocation(tokenSet->translationUnit(), *it);
            if ( tokenStdString == "{" ){
                SourceLocation* sl = m_offsets[QASTNamespace::BEGIN];
                sl->assign(tokenLocation);
                sl->assign(sl->offset() + 1, tokenSet->translationUnit());
            } else if ( tokenStdString == "}" ){
                SourceLocation* sl = m_offsets[QASTNamespace::END];
                sl->assign(tokenLocation);
            }
            clang_disposeString(tokenString);
        }
    }

    CXString name = clang_getCursorDisplayName(tokenSet->cursor());
    const char* cName = clang_getCString(name);
    m_identifier = std::string(cName);
    clang_disposeString(name);
}

QASTNamespace::~QASTNamespace(){
    for ( QASTNode::OffsetMap::iterator it = m_offsets.begin(); it != m_offsets.end(); ++it ){
        delete it->second;
    }
    m_offsets.clear();
}

const SourceLocation *QASTNamespace::location(const std::string &id) const{
    QASTNode::OffsetKeyMap::const_iterator it = OFFSET_KEYS.find(id);
    if ( it == OFFSET_KEYS.end() )
        return 0;
    return QASTNamespace::location(it->second);
}

const SourceLocation *QASTNamespace::location(int id) const{
    QASTNode::OffsetMap::const_iterator it = m_offsets.find(id);
    if ( it == m_offsets.end() )
        return 0;
    return it->second;
}

QASTNode *QASTNamespace::propagateUserCursor(const SourceLocation &location){
    QASTNode* base = this;
    if ( rangeStartLocation().offset() <= location.offset() && rangeEndLocation().offset() >= location.offset() ){
        m_offsets[QASTNamespace::CURSOR] = new SourceLocation(location);
        QASTNode* result = QASTNode::propagateUserCursor(location);
        if ( result != 0 )
            base = result;
    }
    return base;
}

const QASTNode::OffsetKeyMap QASTNamespace::createOffsetKeys(){
    QASTNode::OffsetKeyMap base;
    base["BEGIN"]  = QASTNamespace::BEGIN;
    base["END"]    = QASTNamespace::END;
    base["CURSOR"] = QASTNamespace::CURSOR;
    return base;
}

}}
