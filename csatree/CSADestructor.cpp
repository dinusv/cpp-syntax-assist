#include "CSADestructor.hpp"
#include "CSAClass.hpp"
#include "AnnotatedTokenSet.hpp"
#include "clang-c/Index.h"

namespace csa{ namespace ast{

CSADestructor::CSADestructor(
        AnnotatedTokenSet *tokenSet,
        SourceLocation* cursorLocation,
        SourceLocation* rangeStartLocation,
        SourceLocation* rangeEndLocation,
        CSANode* parent)

    : CSANode("destructor", cursorLocation, rangeStartLocation, rangeEndLocation, parent){


    if ( tokenSet->size() == 0 ){
        CXType type          = clang_getCursorType(tokenSet->cursor());
        CXString typeStr     = clang_getTypeSpelling(type);
        const char* cTypeStr = clang_getCString(typeStr);
        m_identifier         = cTypeStr;
        clang_disposeString(typeStr);
    }

    bool spaceFlag = false;
    for ( AnnotatedTokenSet::Iterator it = tokenSet->begin(); it != tokenSet->end(); ++it ){
        CXToken t = *it;
        CXString tSpelling = clang_getTokenSpelling(tokenSet->translationUnit(), t);
        CXTokenKind tKind  = clang_getTokenKind(t);
        if ( spaceFlag ){
            m_identifier += std::string(" ");
            spaceFlag = false;
        }
        if ( tKind == CXToken_Punctuation ){
            m_identifier += clang_getCString(tSpelling);
            spaceFlag    = false;
        } else {
            m_identifier += clang_getCString(tSpelling);
            spaceFlag     = true;
        }
        clang_disposeString(tSpelling);
    }

}

}}// namespace
