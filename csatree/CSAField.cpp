#include "CSAField.hpp"
#include "CSAClass.hpp"
#include "AnnotatedTokenSet.hpp"
#include "SourceLocation.hpp"
#include "clang-c/Index.h"

#include <QDebug>

namespace csa{ namespace ast{

CSAField::CSAField(
        AnnotatedTokenSet *tokenSet,
        SourceLocation* cursorLocation,
        SourceLocation* rangeStartLocation,
        SourceLocation* rangeEndLocation,
        CSANode* parent)

    : CSANode("field", cursorLocation, rangeStartLocation, rangeEndLocation, parent){


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
        if ( tKind == CXToken_Punctuation ){
            m_identifier += clang_getCString(tSpelling);
            spaceFlag    = false;
        } else {
            if ( spaceFlag ){
                m_identifier += std::string(" ");
                spaceFlag = false;
            }
            m_identifier += clang_getCString(tSpelling);
            spaceFlag     = true;
        }
        clang_disposeString(tSpelling);
    }

    // set class offsets
//    if ( parent != 0 ){
//        if ( parent->typeString() == "class" ){
//            CSAClass* node = static_cast<CSAClass*>(parent);

//            switch( clang_getCXXAccessSpecifier(tokenSet->cursor()) ){
//            case CX_CXXInvalidAccessSpecifier :
//                break;
//            case CX_CXXPrivate :
//                node->setChildLocation(CSAClass::PRIVATE,   *rangeStartLocation);
//                break;
//            case CX_CXXPublic :
//                node->setChildLocation(CSAClass::PUBLIC,    *rangeStartLocation);
//                break;
//            case CX_CXXProtected :
//                node->setChildLocation(CSAClass::PROTECTED, *rangeStartLocation);
//                break;
//            } // switch
//        }
//    }
}

}}// namespace
