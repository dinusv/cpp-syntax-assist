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


#include "CSANode.hpp"
#include "SourceLocation.hpp"

namespace csa{ namespace ast{

int CSANode::dumpIndentation = 1;

CSANode::CSANode(const std::string& typeString, SourceLocation* cursorLocation, SourceLocation* rangeStartLocation, SourceLocation* rangeEndLocation, CSANode *parent)
    : m_typeString(typeString)
    , m_cursorLocation(cursorLocation)
    , m_rangeStartLocation(rangeStartLocation)
    , m_rangeEndLocation(rangeEndLocation)
    , m_parent(parent){

    if ( parent != 0 )
        parent->addChild(this);
}

CSANode::~CSANode(){
    removeChildren();
    delete m_rangeStartLocation;
    delete m_rangeEndLocation;
    delete m_cursorLocation;
}

void CSANode::dump(std::string &out, int depth) const{
    std::string space("");
    for ( int i = 0; i < depth * dumpIndentation; ++i )
        space.append(" ");

    out.append(space + content() + "\n");
    for ( NodeVector::const_iterator it = m_children.begin(); it != m_children.end(); ++it ){
        (*it)->dump(out, depth + 1);
    }
}

const SourceLocation *CSANode::location(const std::string &) const{
    return 0;
}

const SourceLocation *CSANode::location(int) const{
    return 0;
}

int CSANode::offset(const std::string &id) const{
    const SourceLocation* lc = location(id);
    if ( lc != 0 )
        return lc->offset();
    return -1;
}

int CSANode::offset(int id) const{
    const SourceLocation* lc = location(id);
    if ( lc != 0 )
        return lc->offset();
    return -1;
}

CSANode *CSANode::propagateUserCursor(const SourceLocation &location){
    CSANode* base = 0;
    if ( m_rangeStartLocation->offset() <= location.offset() && m_rangeEndLocation->offset() >= location.offset() ){
        for ( NodeVector::iterator it = m_children.begin(); it != m_children.end(); ++it ){
            CSANode* result = (*it)->propagateUserCursor(location);
            if ( result != 0 )
                base = result;
        }
    }
    return base;
}

CSANode *CSANode::find(CSANode* node){
    for ( NodeVector::iterator it = m_children.begin(); it != m_children.end(); ++it ){
        CSANode* child = *it;
        if ( child == node )
            return child;
        CSANode* childFind = child->find(node);
        if ( childFind != 0 )
            return childFind;
    }
    return 0;
}

CSANode *CSANode::find(const std::string &typeString, const std::string &identifier){
    for ( NodeVector::iterator it = m_children.begin(); it != m_children.end(); ++it ){
        CSANode* child = *it;
        if ( child->typeString() == typeString && child->identifier() == identifier )
            return child;
        CSANode* childFind = child->find(typeString, identifier);
        if ( childFind != 0 )
            return childFind;
    }
    return 0;
}

void CSANode::removeChildren(){
    for ( NodeVector::iterator it = m_children.begin(); it != m_children.end(); ++it )
        delete *it;
    m_children.clear();
}

}}// namespace
