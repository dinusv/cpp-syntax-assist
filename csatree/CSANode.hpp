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


#ifndef CSANODE_HPP
#define CSANODE_HPP

#include <string>
#include <vector>
#include <map>

namespace csa{
class AnnotatedTokenSet;
class SourceLocation;

namespace ast{

class CSANode{

public:
    typedef std::vector<CSANode*>                 NodeVector;
    typedef std::vector<CSANode*>::iterator       Iterator;
    typedef std::vector<CSANode*>::const_iterator ConstIterator;

    typedef std::map<std::string, int>     OffsetKeyMap;
    typedef std::map<int, SourceLocation*> OffsetMap;

    static int dumpIndentation;

public:
    CSANode(const std::string& typeString, SourceLocation* cursorLocation, SourceLocation* rangeStartLocation, SourceLocation* rangeEndLocation, CSANode* parent = 0);
    virtual ~CSANode();

    virtual std::string content() const = 0;
    virtual std::string identifier() const = 0;
    const  std::string& typeString() const;

    virtual const SourceLocation* location(const std::string& id) const;
    virtual const SourceLocation* location(int id) const;

    virtual int offset(const std::string& id) const;
    virtual int offset(int id) const;

    virtual CSANode* propagateUserCursor(const SourceLocation& location);

    const SourceLocation& rangeStartLocation() const;
    const SourceLocation& rangeEndLocation() const;
    const SourceLocation& cursorLocation() const;

    void addChild(CSANode* node);
    CSANode* parent();
    CSANode* find(CSANode *node);
    CSANode* find(const std::string& typeString, const std::string& identifier);

    Iterator childrenBegin();
    Iterator childrenEnd();
    ConstIterator childrenBegin() const;
    ConstIterator childrenEnd() const;

    void removeChildren();

    void dump(std::string& out, int depth = 0) const;

private:
    std::string m_typeString;

    SourceLocation* m_cursorLocation;
    SourceLocation* m_rangeStartLocation;
    SourceLocation* m_rangeEndLocation;

    NodeVector  m_children;
    CSANode*    m_parent;
};

inline const std::string &CSANode::typeString() const{
    return m_typeString;
}

inline const SourceLocation &CSANode::rangeStartLocation() const{
    return *m_rangeStartLocation;
}

inline const SourceLocation &CSANode::rangeEndLocation() const{
    return *m_rangeEndLocation;
}

inline const SourceLocation &CSANode::cursorLocation() const{
    return *m_cursorLocation;
}

inline void CSANode::addChild(CSANode *node){
    m_children.push_back(node);
}

inline CSANode *CSANode::parent(){
    return m_parent;
}

inline CSANode::Iterator CSANode::childrenBegin(){
    return m_children.begin();
}

inline CSANode::Iterator CSANode::childrenEnd(){
    return m_children.end();
}

inline CSANode::ConstIterator CSANode::childrenBegin() const{
    return m_children.begin();
}

inline CSANode::ConstIterator CSANode::childrenEnd() const{
    return m_children.end();
}


}}// namespace

#endif // CSANODE_HPP
