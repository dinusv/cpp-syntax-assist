#include "CSANamespace.hpp"
#include "AnnotatedTokenSet.hpp"
#include "SourceLocation.hpp"
#include "clang-c/Index.h"

#include <QDebug>

namespace csa{ namespace ast{

const CSANode::OffsetKeyMap CSANamespace::OFFSET_KEYS = CSANamespace::createOffsetKeys();

CSANamespace::CSANamespace(
        AnnotatedTokenSet *tokenSet,
        SourceLocation* cursorLocation,
        SourceLocation* rangeStartLocation,
        SourceLocation* rangeEndLocation,
        CSANode *parent)
    : CSANode("namespace", cursorLocation, rangeStartLocation, rangeEndLocation, parent ){

    // Set default offsets
    m_offsets[CSANamespace::BEGIN]  = new SourceLocation(*rangeStartLocation);
    m_offsets[CSANamespace::END]    = new SourceLocation(*rangeEndLocation);
    m_offsets[CSANamespace::CURSOR] = 0;

    // Find '{' and '}' in token set
    for ( AnnotatedTokenSet::Iterator it = tokenSet->begin(); it != tokenSet->end(); ++it ){
        CXTokenKind tokenKind = clang_getTokenKind(*it);

        if ( tokenKind == CXToken_Punctuation ){
            CXString tokenString           = clang_getTokenSpelling(tokenSet->translationUnit(), *it);
            std::string tokenStdString     = clang_getCString(tokenString);
            CXSourceLocation tokenLocation = clang_getTokenLocation(tokenSet->translationUnit(), *it);
            if ( tokenStdString == "{" ){
                SourceLocation* sl = m_offsets[CSANamespace::BEGIN];
                sl->assign(tokenLocation);
                sl->assign(sl->offset() + 1, tokenSet->translationUnit());
            } else if ( tokenStdString == "}" ){
                SourceLocation* sl = m_offsets[CSANamespace::END];
                sl->assign(tokenLocation);
            }
            clang_disposeString(tokenString);
        }
    }

    CXString name = clang_getCursorDisplayName(tokenSet->cursor());
    const char* cName = clang_getCString(name);
    m_identifier = std::string(cName);
    clang_disposeString(name);
}

CSANamespace::~CSANamespace(){
    for ( CSANode::OffsetMap::iterator it = m_offsets.begin(); it != m_offsets.end(); ++it ){
        delete it->second;
    }
    m_offsets.clear();
}

const SourceLocation *CSANamespace::location(const std::string &id) const{
    CSANode::OffsetKeyMap::const_iterator it = OFFSET_KEYS.find(id);
    if ( it == OFFSET_KEYS.end() )
        return 0;
    return CSANamespace::location(it->second);
}

const SourceLocation *CSANamespace::location(int id) const{
    CSANode::OffsetMap::const_iterator it = m_offsets.find(id);
    if ( it == m_offsets.end() )
        return 0;
    return it->second;
}

CSANode *CSANamespace::propagateUserCursor(const SourceLocation &location){
    CSANode* base = this;
    if ( rangeStartLocation().offset() <= location.offset() && rangeEndLocation().offset() >= location.offset() ){
        m_offsets[CSANamespace::CURSOR] = new SourceLocation(location);
        CSANode* result = CSANode::propagateUserCursor(location);
        if ( result != 0 )
            base = result;
    }
    return base;
}

const CSANode::OffsetKeyMap CSANamespace::createOffsetKeys(){
    CSANode::OffsetKeyMap base;
    base["BEGIN"]  = CSANamespace::BEGIN;
    base["END"]    = CSANamespace::END;
    base["CURSOR"] = CSANamespace::CURSOR;
    return base;
}

}}
