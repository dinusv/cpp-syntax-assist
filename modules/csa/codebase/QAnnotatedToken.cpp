#include "QAnnotatedToken.hpp"
#include "QAnnotatedTokenSet.hpp"
#include "QSourceLocation.hpp"
#include "QASTNode.hpp"

namespace csa{

QAnnotatedToken::QAnnotatedToken(QObject* parent)
    : QObject(parent)
{
}

QAnnotatedToken::QAnnotatedToken(CXToken token, QAnnotatedTokenSet* parent)
    : QObject(parent)
    , m_token(token)
{
    if ( parent ){
        CXString spelling = clang_getTokenSpelling(parent->translationUnit(), m_token);
        m_tokenName       = clang_getCString(spelling);
        clang_disposeString(spelling);

        m_tokenRange      = clang_getTokenExtent(parent->translationUnit(), m_token);
    }
}

QAnnotatedToken::~QAnnotatedToken(){
}

ast::QASTNode* QAnnotatedToken::associatedNode(){
    QAnnotatedTokenSet* tokenSet = qobject_cast<QAnnotatedTokenSet*>(parent());
    if ( tokenSet )
        return tokenSet->associatedNode();
    return 0;
}

QAnnotatedToken::TokenKind QAnnotatedToken::kind() const{
    return static_cast<QAnnotatedToken::TokenKind>(clang_getTokenKind(m_token));
}

void QAnnotatedToken::before(const QString& value){
    ast::QASTNode* node = associatedNode();
    if ( !node )
        return;

    CXSourceLocation locStart = clang_getRangeStart(m_tokenRange);
    node->insert(value, QSourceLocation(locStart));
}

void QAnnotatedToken::after(const QString& value){
    ast::QASTNode* node = associatedNode();
    if ( !node )
        return;

    CXSourceLocation locEnd = clang_getRangeEnd(m_tokenRange);
    node->insert(value, QSourceLocation(locEnd));
}

void QAnnotatedToken::afterln(const QString& value){
    QAnnotatedTokenSet* tokenSet = qobject_cast<QAnnotatedTokenSet*>(parent());
    if ( !tokenSet )
        return;

    ast::QASTNode* node = tokenSet->associatedNode();
    if ( !node )
        return;

    CXSourceLocation location = clang_getRangeEnd(m_tokenRange);

    CXFile file;
    unsigned line, column, offset;
    clang_getSpellingLocation(location, &file, &line, &column, &offset);

    CXSourceLocation afterLocation = clang_getLocation(
                tokenSet->translationUnit(),
                file,
                line + 1,
                1); // location will be automatically truncated to max lines

    node->insert(value, QSourceLocation(afterLocation));
}

}// namespace
