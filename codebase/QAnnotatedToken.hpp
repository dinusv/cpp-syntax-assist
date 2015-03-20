#ifndef QANNOTATEDTOKEN_HPP
#define QANNOTATEDTOKEN_HPP

#include "clang-c/Index.h"
#include <QObject>

namespace csa{

namespace ast{
class QASTNode;
}
class QAnnotatedTokenSet;

class QAnnotatedToken : public QObject{

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

    void before(const QString& value);
    void after(const QString& value);
    void afterln(const QString& value);

public slots:
    csa::ast::QASTNode* associatedNode();
    QString name() const;
    TokenKind kind() const;

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
