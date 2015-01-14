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


#include "QASTClass.hpp"
#include "SourceLocation.hpp"
#include "AnnotatedTokenSet.hpp"

namespace csa{ namespace ast{

const QASTNode::OffsetKeyMap QASTClass::OFFSET_KEYS = QASTClass::createOffsetKeys();

QASTClass::QASTClass(
        AnnotatedTokenSet* tokenSet,
        SourceLocation* cursorLocation,
        SourceLocation* rangeStartLocation,
        SourceLocation* rangeEndLocation,
        QASTNode* parent)

    : QASTNode("class", cursorLocation, rangeStartLocation, rangeEndLocation, parent){

    // Set default offsets
    m_offsets[QASTClass::BEGIN]          = new SourceLocation(*rangeStartLocation);
    m_offsets[QASTClass::END]            = new SourceLocation(*rangeEndLocation);
    m_offsets[QASTClass::QPROPERTY]      = new SourceLocation(*rangeEndLocation);
    m_offsets[QASTClass::PUBLIC]         = new SourceLocation(*rangeEndLocation);
    m_offsets[QASTClass::PROTECTED]      = new SourceLocation(*rangeEndLocation);
    m_offsets[QASTClass::PRIVATE]        = new SourceLocation(*rangeEndLocation);
    m_offsets[QASTClass::CURSOR]         = 0;
    m_offsetsBeginSet                   = false;
    m_lastSet                           = BEGIN;

    m_offsets[QASTClass::IMPLEMENTATION] = new SourceLocation(*rangeEndLocation);
    SourceLocation* sl = m_offsets[QASTClass::IMPLEMENTATION];
    sl->assign(sl->offset() + 1, tokenSet->translationUnit());

    // Find '{' and '}' in token set
    for ( AnnotatedTokenSet::Iterator it = tokenSet->begin(); it != tokenSet->end(); ++it ){
        CXTokenKind tokenKind = clang_getTokenKind(*it);

        if ( tokenKind == CXToken_Punctuation ){
            CXString tokenString           = clang_getTokenSpelling(tokenSet->translationUnit(), *it);
            std::string tokenStdString     = clang_getCString(tokenString);
            CXSourceLocation tokenLocation = clang_getTokenLocation(tokenSet->translationUnit(), *it);
            if ( tokenStdString == "{" && !m_offsetsBeginSet ){
                SourceLocation* sl = m_offsets[QASTClass::BEGIN];
                sl->assign(tokenLocation);
                sl->assign(sl->offset() + 1, tokenSet->translationUnit());
                m_offsetsBeginSet = true;

            } else if ( tokenStdString == "}" ){
                SourceLocation* sl = m_offsets[QASTClass::END];
                sl->assign(tokenLocation);
                m_offsets[QASTClass::QPROPERTY]->assign(*sl);
                m_offsets[QASTClass::PUBLIC]->assign(*sl);
                m_offsets[QASTClass::PROTECTED]->assign(*sl);
                m_offsets[QASTClass::PRIVATE]->assign(*sl);
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

QASTClass::~QASTClass(){
    for ( QASTNode::OffsetMap::iterator it = m_offsets.begin(); it != m_offsets.end(); ++it ){
        delete it->second;
    }
    m_offsets.clear();
}


const SourceLocation *QASTClass::location(const std::string &id) const{
    QASTNode::OffsetKeyMap::const_iterator it = OFFSET_KEYS.find(id);
    if ( it == OFFSET_KEYS.end() )
        return 0;
    return QASTClass::location(it->second);
}

const SourceLocation *QASTClass::location(int id) const{
    QASTNode::OffsetMap::const_iterator it = m_offsets.find(id);
    if ( it == m_offsets.end() )
        return 0;
    return it->second;
}

void QASTClass::setChildLocation(QASTClass::OffsetId id, const SourceLocation &location){
    if ( id == QASTClass::IMPLEMENTATION )
        m_offsets[QASTClass::IMPLEMENTATION]->assign(location);
    else {
        switch ( m_lastSet ){
        case QASTClass::BEGIN:
            m_offsets[QASTClass::QPROPERTY]->assign(location); break;
        case QASTClass::PUBLIC :
        case QASTClass::PROTECTED :
        case QASTClass::PRIVATE :
            m_offsets[m_lastSet]->assign(location); break;
        default : break;
        }
        m_lastSet = id;
    }
}

QASTNode *QASTClass::propagateUserCursor(const SourceLocation &location){
    QASTNode* base = this;
    if ( rangeStartLocation().offset() <= location.offset() && rangeEndLocation().offset() >= location.offset() ){
        m_offsets[QASTClass::CURSOR] = new SourceLocation(location);
        QASTNode* result = QASTNode::propagateUserCursor(location);
        if ( result != 0 )
            base = result;
    }
    return base;
}

const QASTNode::OffsetKeyMap QASTClass::createOffsetKeys(){
    QASTNode::OffsetKeyMap base;
    base["BEGIN"]          = QASTClass::BEGIN;
    base["END"]            = QASTClass::END;
    base["QPROPERTY"]      = QASTClass::QPROPERTY;
    base["PUBLIC"]         = QASTClass::PUBLIC;
    base["PROTECTED"]      = QASTClass::PROTECTED;
    base["PRIVATE"]        = QASTClass::PRIVATE;
    base["IMPLEMENTATION"] = QASTClass::IMPLEMENTATION;
    base["CURSOR"]         = QASTClass::CURSOR;
    return base;
}

}}// namespace
