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

#include <QObject>
#include "qcsaglobal.h"

namespace csa{

namespace ast{ class QASTNode; }

class QCXTokenWrapper;
class QAnnotatedTokenSet;

class QAnnotatedTokenPrivate;
class Q_CSA_EXPORT QAnnotatedToken : public QObject{

    Q_OBJECT

public:
    enum TokenKind{
        Punctuation,
        Keyword,
        Identifier,
        Literal,
        Comment
    };

    Q_ENUMS(TokenKind)

public:
    QAnnotatedToken(const QCXTokenWrapper& token, QAnnotatedTokenSet* parent = 0);
    ~QAnnotatedToken();

    void setParent(QAnnotatedTokenSet* parent);
    const QCXTokenWrapper& token() const;

public slots:
    csa::ast::QASTNode* associatedNode();
    QString name() const;
    TokenKind kind() const;

    void before(const QString& value);
    void after(const QString& value);
    void afterln(const QString& value);

private:
    // prevent copy
    QAnnotatedToken(const QAnnotatedToken& other);
    QAnnotatedToken& operator=(const QAnnotatedToken& other);

    // d ptr
    QAnnotatedTokenPrivate* const d_ptr;
    Q_DECLARE_PRIVATE(QAnnotatedToken)

};

}// namespace

Q_DECLARE_METATYPE(csa::QAnnotatedToken*)
Q_DECLARE_METATYPE(csa::QAnnotatedToken::TokenKind)

#endif // QANNOTATEDTOKEN_HPP
