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


#ifndef QANNOTATEDTOKEN_HPP
#define QANNOTATEDTOKEN_HPP

#include "clang-c/Index.h"
#include <QObject>
#include <QCSAGlobal.hpp>

namespace csa{

namespace ast{
class QASTNode;
}
class QAnnotatedTokenSet;

class Q_CSA_EXPORT QAnnotatedToken : public QObject{

    Q_OBJECT

public:
    enum TokenKind{
        Punctuation = CXToken_Punctuation,
        Keyword     = CXToken_Keyword,
        Identifier  = CXToken_Identifier,
        Literal     = CXToken_Literal,
        Comment     = CXToken_Comment
    };

    Q_ENUMS(TokenKind)

public:
    QAnnotatedToken(QObject* parent = 0);
    QAnnotatedToken(CXToken token, QAnnotatedTokenSet* parent = 0);
    ~QAnnotatedToken();

    void setParent(QAnnotatedTokenSet* parent);
    const CXToken& token() const;

public slots:
    csa::ast::QASTNode* associatedNode();
    QString name() const;
    TokenKind kind() const;

    void before(const QString& value);
    void after(const QString& value);
    void afterln(const QString& value);

private:
    QAnnotatedTokenSet* m_parent;
    CXToken             m_token;
    CXSourceRange       m_tokenRange;
    QString             m_tokenName;
};

inline const CXToken& QAnnotatedToken::token() const{
    return m_token;
}

inline QString QAnnotatedToken::name() const{
    return m_tokenName;
}

}// namespace

Q_DECLARE_METATYPE(csa::QAnnotatedToken*)
Q_DECLARE_METATYPE(csa::QAnnotatedToken::TokenKind)

#endif // QANNOTATEDTOKEN_HPP
