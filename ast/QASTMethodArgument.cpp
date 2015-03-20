#include "QASTMethodArgument.hpp"
#include "QAnnotatedToken.hpp"
#include "QAnnotatedTokenSet.hpp"
#include "clang-c/Index.h"

namespace csa{ namespace ast{

QASTMethodArgument::QASTMethodArgument(
        QAnnotatedTokenSet* tokenSet,
        QSourceLocation* cursorLocation,
        QSourceLocation* rangeStartLocation,
        QSourceLocation* rangeEndLocation,
        QASTNode* parent)
    : QASTNode("MethodArgument", tokenSet, cursorLocation, rangeStartLocation, rangeEndLocation, parent)
{

    CXString id = clang_getCursorSpelling(tokenSet->cursor());
    setIdentifier(clang_getCString(id));

    if ( tokenSet ){
        for ( QAnnotatedTokenSet::Iterator it = tokenSet->begin(); it != tokenSet->end(); ++it ){
            CXToken t = (*it)->token();
            CXString tSpelling = clang_getTokenSpelling(tokenSet->translationUnit(), t);
            const char* tCSpelling = clang_getCString(tSpelling);
            CXTokenKind tKind  = clang_getTokenKind(t);

            if ( tKind == CXToken_Identifier && std::string(clang_getCString(id)) == tCSpelling )
                break;

            if ( ( tKind == CXToken_Identifier || tKind == CXToken_Keyword ) && !m_type.isEmpty() )
                m_type += " ";

            m_type += tCSpelling;
        }
    }

    clang_disposeString(id);
}

QASTMethodArgument::~QASTMethodArgument(){
}

QString QASTMethodArgument::content() const{
    return m_type + " " + identifier();
}

}} // namespace ast, csa
