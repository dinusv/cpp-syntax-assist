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


#include "QASTNode.hpp"
#include "SourceLocation.hpp"

namespace csa{ namespace ast{

int QASTNode::dumpIndentation = 1;

QASTNode::QASTNode(const QString &typeName,
        SourceLocation* cursorLocation,
        SourceLocation* rangeStartLocation,
        SourceLocation* rangeEndLocation,
        QASTNode *parent)
    : QObject(parent)
    , m_typeName(typeName)
    , m_cursorLocation(cursorLocation)
    , m_rangeStartLocation(rangeStartLocation)
    , m_rangeEndLocation(rangeEndLocation)
{
    if ( parent != 0 )
        parent->addChild(this);
}

QASTNode::~QASTNode(){
    removeChildren();
    delete m_rangeStartLocation;
    delete m_rangeEndLocation;
    delete m_cursorLocation;
}

void QASTNode::dump(std::string &out, int depth) const{
    std::string space("");
    for ( int i = 0; i < depth * dumpIndentation; ++i )
        space.append(" ");

    out.append(space + content() + "\n");
    for ( NodeList::const_iterator it = m_children.begin(); it != m_children.end(); ++it ){
        (*it)->dump(out, depth + 1);
    }
}

QASTNode *QASTNode::propagateUserCursor(const SourceLocation &location){
    QASTNode* base = 0;
    if ( m_rangeStartLocation->offset() <= location.offset() && m_rangeEndLocation->offset() >= location.offset() ){
        for ( NodeList::iterator it = m_children.begin(); it != m_children.end(); ++it ){
            QASTNode* result = (*it)->propagateUserCursor(location);
            if ( result != 0 )
                base = result;
        }
    }
    return base;
}

QASTNode *QASTNode::find(QASTNode* node){
    for ( NodeList::iterator it = m_children.begin(); it != m_children.end(); ++it ){
        QASTNode* child = *it;
        if ( child == node )
            return child;
        QASTNode* childFind = child->find(node);
        if ( childFind != 0 )
            return childFind;
    }
    return 0;
}

QASTNode *QASTNode::find(const std::string &typeString, const std::string &identifier){
    for ( NodeList::iterator it = m_children.begin(); it != m_children.end(); ++it ){
        QASTNode* child = *it;
        if ( child->typeString() == typeString && child->identifier() == identifier )
            return child;
        QASTNode* childFind = child->find(typeString, identifier);
        if ( childFind != 0 )
            return childFind;
    }
    return 0;
}

void QASTNode::removeChildren(){
    for ( NodeList::iterator it = m_children.begin(); it != m_children.end(); ++it )
        delete *it;
    m_children.clear();
}

}}// namespace
