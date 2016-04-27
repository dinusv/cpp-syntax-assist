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


#include "qannotatedtoken.h"
#include "qannotatedtoken_p.h"
#include "qannotatedtokenset.h"
#include "qsourcelocation.h"
#include "qsourcelocation_p.h"
#include "qastnode.h"
#include "clang-c/Index.h"

namespace csa{

class QAnnotatedTokenPrivate{

public:
    QAnnotatedTokenPrivate(const QCXTokenWrapper& tokenWrapper)
        : tokenWrap(tokenWrapper){}

    QAnnotatedTokenSet* parent;
    QCXTokenWrapper     tokenWrap;
    CXSourceRange       tokenRange;
    QString             tokenName;
};

QAnnotatedToken::QAnnotatedToken(const QCXTokenWrapper& token, QAnnotatedTokenSet* parent)
    : QObject(parent)
    , d_ptr(new QAnnotatedTokenPrivate(token))
{
    Q_D(QAnnotatedToken);
    if ( parent ){
        CXString spelling = clang_getTokenSpelling(parent->translationUnit(), d->tokenWrap.token);
        d->tokenName      = clang_getCString(spelling);
        clang_disposeString(spelling);

        d->tokenRange     = clang_getTokenExtent(parent->translationUnit(), d->tokenWrap.token);
    }
}

QAnnotatedToken::~QAnnotatedToken(){
    delete d_ptr;
}

ast::QASTNode* QAnnotatedToken::associatedNode(){
    QAnnotatedTokenSet* tokenSet = qobject_cast<QAnnotatedTokenSet*>(parent());
    if ( tokenSet )
        return tokenSet->associatedNode();
    return 0;
}

QAnnotatedToken::TokenKind QAnnotatedToken::kind() const{
    const Q_D(QAnnotatedToken);
    return static_cast<QAnnotatedToken::TokenKind>(clang_getTokenKind(d->tokenWrap.token));
}

void QAnnotatedToken::before(const QString& value){
    Q_D(QAnnotatedToken);
    ast::QASTNode* node = associatedNode();
    if ( !node )
        return;

    QSourceLocation locStart = createSourceLocation(clang_getRangeStart(d->tokenRange));
    node->insert(value, &locStart);
}

void QAnnotatedToken::after(const QString& value){
    Q_D(QAnnotatedToken);
    ast::QASTNode* node = associatedNode();
    if ( !node )
        return;

    QSourceLocation locEnd = createSourceLocation(clang_getRangeEnd(d->tokenRange));
    node->insert(value, &locEnd);
}

void QAnnotatedToken::afterln(const QString& value){
    Q_D(QAnnotatedToken);
    QAnnotatedTokenSet* tokenSet = qobject_cast<QAnnotatedTokenSet*>(parent());
    if ( !tokenSet )
        return;

    ast::QASTNode* node = tokenSet->associatedNode();
    if ( !node )
        return;

    CXSourceLocation location = clang_getRangeEnd(d->tokenRange);

    CXFile file;
    unsigned line, column, offset;
    clang_getSpellingLocation(location, &file, &line, &column, &offset);

    QSourceLocation afterLocation = createSourceLocation(clang_getLocation(
                tokenSet->translationUnit(),
                file,
                line + 1,
                1
    )); // location will be automatically truncated to max lines

    node->insert(value, &afterLocation);
}

const QCXTokenWrapper& QAnnotatedToken::token() const{
    const Q_D(QAnnotatedToken);
    return d->tokenWrap;
}

QString QAnnotatedToken::name() const{
    const Q_D(QAnnotatedToken);
    return d->tokenName;
}

}// namespace
