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


#include "QAnnotatedTokenSet.hpp"
#include "QAnnotatedToken.hpp"
#include "QASTNode.hpp"

#include <sstream>
#include <iostream>

namespace csa{

class QAnnotatedTokenSetPrivate{

public:
    QAnnotatedTokenSetPrivate(const CXCursor &pCursor, const CXTranslationUnit& pTu)
        : cursor(pCursor)
        , translationUnit(pTu)
    {
    }

    QAnnotatedTokenSet::TokenList tokens;
    CXCursor                      cursor;
    CXTranslationUnit             translationUnit;
};


QAnnotatedTokenSet::QAnnotatedTokenSet(const CXCursor &cursor, const CXTranslationUnit &tu, ast::QASTNode* parent)
    : QObject(parent)
    , d_ptr(new QAnnotatedTokenSetPrivate(cursor, tu))
{
}

QAnnotatedTokenSet::~QAnnotatedTokenSet(){
    for ( QAnnotatedTokenSet::Iterator it = begin(); it != end(); ++it )
        delete *it;
    delete d_ptr;
}

void QAnnotatedTokenSet::append(const CXToken &token){
    Q_D(QAnnotatedTokenSet);
    d->tokens.append(new QAnnotatedToken(token, this));
}

bool QAnnotatedTokenSet::operator ==(const QAnnotatedTokenSet &other){
    Q_D(QAnnotatedTokenSet);
    return clang_equalCursors(d->cursor, other.cursor());
}

bool QAnnotatedTokenSet::operator !=(const QAnnotatedTokenSet &other){
    Q_D(QAnnotatedTokenSet);
    return !clang_equalCursors(d->cursor, other.cursor());
}

const CXTranslationUnit &QAnnotatedTokenSet::translationUnit(){
    Q_D(QAnnotatedTokenSet);
    return d->translationUnit;
}

void QAnnotatedTokenSet::dump(std::string &str){
    Q_D(QAnnotatedTokenSet);
    CXString displayName = clang_getCursorDisplayName(d->cursor);
    str.append("Cursor :");
    str.append(clang_getCString(displayName));
    str.append("\n Tokens :");
    clang_disposeString(displayName);

    for ( QAnnotatedTokenSet::Iterator it = begin(); it != end(); ++it ){
        CXString tokenString           = clang_getTokenSpelling(d->translationUnit, (*it)->token());
        CXSourceLocation tokenLocation = clang_getTokenLocation(d->translationUnit, (*it)->token());
        unsigned int column, line;
        clang_getSpellingLocation(tokenLocation, 0, &line, &column, 0);
        std::stringstream stream;
        stream << " (" << line << "," << column << ") \"";
        str.append(stream.str() + clang_getCString(tokenString) + "\"");
        clang_disposeString(tokenString);
    }
    str.append("\n");
}

const QList<QAnnotatedToken*>& QAnnotatedTokenSet::tokenList() const{
    const Q_D(QAnnotatedTokenSet);
    return d->tokens;
}

ast::QASTNode* QAnnotatedTokenSet::associatedNode(){
    return qobject_cast<ast::QASTNode*>(parent());
}

const CXCursor &QAnnotatedTokenSet::cursor() const{
    const Q_D(QAnnotatedTokenSet);
    return d->cursor;
}

unsigned int QAnnotatedTokenSet::size() const{
    const Q_D(QAnnotatedTokenSet);
    return static_cast<unsigned int>(d->tokens.size());
}

QAnnotatedTokenSet::Iterator QAnnotatedTokenSet::begin(){
    Q_D(QAnnotatedTokenSet);
    return d->tokens.begin();
}

QAnnotatedTokenSet::Iterator QAnnotatedTokenSet::end(){
    Q_D(QAnnotatedTokenSet);
    return d->tokens.end();
}

QAnnotatedTokenSet::ConstIterator QAnnotatedTokenSet::begin() const{
    const Q_D(QAnnotatedTokenSet);
    return d->tokens.begin();
}

QAnnotatedTokenSet::ConstIterator QAnnotatedTokenSet::end() const{
    const Q_D(QAnnotatedTokenSet);
    return d->tokens.end();
}

void QAnnotatedTokenSet::erase(QAnnotatedTokenSet::Iterator it){
    Q_D(QAnnotatedTokenSet);
    d->tokens.erase(it);
}

}// namespace
