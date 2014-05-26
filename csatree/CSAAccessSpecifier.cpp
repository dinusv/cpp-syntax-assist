#include "CSAAccessSpecifier.hpp"
#include "AnnotatedTokenSet.hpp"
#include "CSAClass.hpp"

namespace csa{ namespace ast{

CSAAccessSpecifier::CSAAccessSpecifier(
        AnnotatedTokenSet* tokenSet,
        SourceLocation* cursorLocation,
        SourceLocation* rangeStartLocation,
        SourceLocation* rangeEndLocation,
        CSANode *parent)

    : CSANode("access", cursorLocation, rangeStartLocation, rangeEndLocation, parent){

    switch( clang_getCXXAccessSpecifier(tokenSet->cursor()) ){
    case CX_CXXInvalidAccessSpecifier :
        m_typeValueName = "invalid";
        break;
    case CX_CXXPrivate :
        m_typeValueName = "private";
        break;
    case CX_CXXPublic :
        m_typeValueName = "public";
        break;
    case CX_CXXProtected :
        m_typeValueName = "protected";
        break;
    }

    // set class offsets
    if ( parent != 0 ){
        if ( parent->typeString() == "class" ){
            CSAClass* node = static_cast<CSAClass*>(parent);

            switch( clang_getCXXAccessSpecifier(tokenSet->cursor()) ){
            case CX_CXXInvalidAccessSpecifier :
                break;
            case CX_CXXPrivate :
                node->setChildLocation(CSAClass::PRIVATE,   *rangeStartLocation);
                break;
            case CX_CXXPublic :
                node->setChildLocation(CSAClass::PUBLIC,    *rangeStartLocation);
                break;
            case CX_CXXProtected :
                node->setChildLocation(CSAClass::PROTECTED, *rangeStartLocation);
                break;
            } // switch
        }
    }

}

}}// namespace
