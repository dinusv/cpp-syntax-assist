/****************************************************************************
**
** Copyright (C) 2014 Dinu SV.
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


#include "CSAClass.hpp"
#include "SourceLocation.hpp"
#include "AnnotatedTokenSet.hpp"

namespace csa{ namespace ast{

const CSANode::OffsetKeyMap CSAClass::OFFSET_KEYS = CSAClass::createOffsetKeys();

CSAClass::CSAClass(
        AnnotatedTokenSet* tokenSet,
        SourceLocation* cursorLocation,
        SourceLocation* rangeStartLocation,
        SourceLocation* rangeEndLocation,
        CSANode* parent)

    : CSANode("class", cursorLocation, rangeStartLocation, rangeEndLocation, parent){

    // Set default offsets
    m_offsets[CSAClass::BEGIN]          = new SourceLocation(*rangeStartLocation);
    m_offsets[CSAClass::END]            = new SourceLocation(*rangeEndLocation);
    m_offsets[CSAClass::QPROPERTY]      = new SourceLocation(*rangeEndLocation);
    m_offsets[CSAClass::PUBLIC]         = new SourceLocation(*rangeEndLocation);
    m_offsets[CSAClass::PROTECTED]      = new SourceLocation(*rangeEndLocation);
    m_offsets[CSAClass::PRIVATE]        = new SourceLocation(*rangeEndLocation);
    m_offsets[CSAClass::CURSOR]         = 0;
    m_offsetsBeginSet                   = false;
    m_lastSet                           = BEGIN;

    m_offsets[CSAClass::IMPLEMENTATION] = new SourceLocation(*rangeEndLocation);
    SourceLocation* sl = m_offsets[CSAClass::IMPLEMENTATION];
    sl->assign(sl->offset() + 1, tokenSet->translationUnit());

    // Find '{' and '}' in token set
    for ( AnnotatedTokenSet::Iterator it = tokenSet->begin(); it != tokenSet->end(); ++it ){
        CXTokenKind tokenKind = clang_getTokenKind(*it);

        if ( tokenKind == CXToken_Punctuation ){
            CXString tokenString           = clang_getTokenSpelling(tokenSet->translationUnit(), *it);
            std::string tokenStdString     = clang_getCString(tokenString);
            CXSourceLocation tokenLocation = clang_getTokenLocation(tokenSet->translationUnit(), *it);
            if ( tokenStdString == "{" && !m_offsetsBeginSet ){
                SourceLocation* sl = m_offsets[CSAClass::BEGIN];
                sl->assign(tokenLocation);
                sl->assign(sl->offset() + 1, tokenSet->translationUnit());
                m_offsetsBeginSet = true;

            } else if ( tokenStdString == "}" ){
                SourceLocation* sl = m_offsets[CSAClass::END];
                sl->assign(tokenLocation);
                m_offsets[CSAClass::QPROPERTY]->assign(*sl);
                m_offsets[CSAClass::PUBLIC]->assign(*sl);
                m_offsets[CSAClass::PROTECTED]->assign(*sl);
                m_offsets[CSAClass::PRIVATE]->assign(*sl);
            }
            clang_disposeString(tokenString);
        }
    }

    // Get name
    CXString name = clang_getCursorDisplayName(tokenSet->cursor());
    const char* cName = clang_getCString(name);
    m_identifier = cName;
    clang_disposeString(name);

}

CSAClass::~CSAClass(){
    for ( CSANode::OffsetMap::iterator it = m_offsets.begin(); it != m_offsets.end(); ++it ){
        delete it->second;
    }
    m_offsets.clear();
}


const SourceLocation *CSAClass::location(const std::string &id) const{
    CSANode::OffsetKeyMap::const_iterator it = OFFSET_KEYS.find(id);
    if ( it == OFFSET_KEYS.end() )
        return 0;
    return CSAClass::location(it->second);
}

const SourceLocation *CSAClass::location(int id) const{
    CSANode::OffsetMap::const_iterator it = m_offsets.find(id);
    if ( it == m_offsets.end() )
        return 0;
    return it->second;
}

void CSAClass::setChildLocation(CSAClass::OffsetId id, const SourceLocation &location){
    if ( id == CSAClass::IMPLEMENTATION )
        m_offsets[CSAClass::IMPLEMENTATION]->assign(location);
    else {
        switch ( m_lastSet ){
        case CSAClass::BEGIN:
            m_offsets[CSAClass::QPROPERTY]->assign(location); break;
        case CSAClass::PUBLIC :
        case CSAClass::PROTECTED :
        case CSAClass::PRIVATE :
            m_offsets[m_lastSet]->assign(location); break;
        default : break;
        }
        m_lastSet = id;
    }
}

CSANode *CSAClass::propagateUserCursor(const SourceLocation &location){
    CSANode* base = this;
    if ( rangeStartLocation().offset() <= location.offset() && rangeEndLocation().offset() >= location.offset() ){
        m_offsets[CSAClass::CURSOR] = new SourceLocation(location);
        CSANode* result = CSANode::propagateUserCursor(location);
        if ( result != 0 )
            base = result;
    }
    return base;
}

const CSANode::OffsetKeyMap CSAClass::createOffsetKeys(){
    CSANode::OffsetKeyMap base;
    base["BEGIN"]          = CSAClass::BEGIN;
    base["END"]            = CSAClass::END;
    base["QPROPERTY"]      = CSAClass::QPROPERTY;
    base["PUBLIC"]         = CSAClass::PUBLIC;
    base["PROTECTED"]      = CSAClass::PROTECTED;
    base["PRIVATE"]        = CSAClass::PRIVATE;
    base["IMPLEMENTATION"] = CSAClass::IMPLEMENTATION;
    base["CURSOR"]         = CSAClass::CURSOR;
    return base;
}

}}// namespace
