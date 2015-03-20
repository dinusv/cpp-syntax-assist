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


#include "QAnnotatedTokenSet.hpp"
#include "QASTNode.hpp"
#include <sstream>
#include <iostream>

namespace csa{

QAnnotatedTokenSet::QAnnotatedTokenSet(const CXCursor &cursor, const CXTranslationUnit &tu, ast::QASTNode* parent)
    : QObject(parent)
    , m_cursor(cursor)
    , m_translationUnit(tu)
{
}

QAnnotatedTokenSet::~QAnnotatedTokenSet(){
    for ( QAnnotatedTokenSet::Iterator it = begin(); it != end(); ++it )
        delete *it;
}

void QAnnotatedTokenSet::append(const CXToken &token){
    m_tokens.append(new QAnnotatedToken(token, this));
}

bool QAnnotatedTokenSet::operator ==(const QAnnotatedTokenSet &other){
    return clang_equalCursors(m_cursor, other.cursor());
}

bool QAnnotatedTokenSet::operator !=(const QAnnotatedTokenSet &other){
    return !clang_equalCursors(m_cursor, other.cursor());
}

const CXTranslationUnit &QAnnotatedTokenSet::translationUnit(){
    return m_translationUnit;
}

void QAnnotatedTokenSet::dump(std::string &str){
    CXString displayName = clang_getCursorDisplayName(m_cursor);
    str.append("Cursor :");
    str.append(clang_getCString(displayName));
    str.append("\n Tokens :");
    clang_disposeString(displayName);

    for ( QAnnotatedTokenSet::Iterator it = begin(); it != end(); ++it ){
        CXString tokenString           = clang_getTokenSpelling(m_translationUnit, (*it)->token());
        CXSourceLocation tokenLocation = clang_getTokenLocation(m_translationUnit, (*it)->token());
        unsigned int column, line;
        clang_getSpellingLocation(tokenLocation, 0, &line, &column, 0);
        std::stringstream stream;
        stream << " (" << line << "," << column << ") \"";
        str.append(stream.str() + clang_getCString(tokenString) + "\"");
        clang_disposeString(tokenString);
    }
    str.append("\n");
}

QList<QAnnotatedToken*> QAnnotatedTokenSet::tokenList(){
    return m_tokens;
}

ast::QASTNode* QAnnotatedTokenSet::associatedNode(){
    return qobject_cast<ast::QASTNode*>(parent());
}

}// namespace
