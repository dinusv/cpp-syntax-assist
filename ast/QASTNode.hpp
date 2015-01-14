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
#include <QObject>
#include <QList>
#include <QVariant>

namespace csa{
class AnnotatedTokenSet;
class SourceLocation;

namespace ast{

class QASTNode : public QObject{

    Q_OBJECT

public:
    typedef QList<QASTNode*>                 NodeList;
    typedef QList<QASTNode*>::iterator       Iterator;
    typedef QList<QASTNode*>::const_iterator ConstIterator;

    static int dumpIndentation;

public:
    // Constructor / Destructor
    // ------------------------

    QASTNode(
            const QString& typeName,
            SourceLocation* cursorLocation,
            SourceLocation* rangeStartLocation,
            SourceLocation* rangeEndLocation,
            QASTNode* parent = 0);
    virtual ~QASTNode();

    // Callable Methods
    // ----------------

public slots:
    const QString& typeName() const;
    const QString& identifier() const;

    const QList<QASTNode*>& astChildren() const;
    QList<QASTNode *> astChildren(const QString& type) const;

    QVariant astChild(const QString& identifier);
    QVariant astChild(const QString& typeString, const QString& identif);

    void append(const QString& value);
    void prepend(const QString& value);
    void before(const QString& value);
    void after(const QString& value);

public:
    // Location Getters
    // ----------------

    const SourceLocation& rangeStartLocation() const;
    const SourceLocation& rangeEndLocation() const;
    const SourceLocation& cursorLocation() const;

    virtual const SourceLocation& bodyStartLocation() const;
    virtual const SourceLocation& bodyEndLocation() const;

    // Initializers
    // ------------

    virtual QASTNode* propagateUserCursor(const SourceLocation& location);

    // Children Handlers
    // -----------------

    void addChild(QASTNode* node);
    QASTNode* find(QASTNode *node);
    QASTNode* find(const std::string& typeString, const std::string& identifier);

    Iterator childrenBegin();
    Iterator childrenEnd();
    ConstIterator childrenBegin() const;
    ConstIterator childrenEnd() const;

    void removeChildren();

    // Debugging
    // ---------

    void dump(std::string& out, int depth = 0) const;

protected:
    void setIdentifier(const QString& identifier);

private:
    // Parameters
    // ----------

    QString m_typeName;
    QString m_identifier;

    SourceLocation* m_cursorLocation;
    SourceLocation* m_rangeStartLocation;
    SourceLocation* m_rangeEndLocation;

    NodeList     m_children;
    QASTNode*    m_parent;
};

inline void QASTNode::setIdentifier(const QString &identifier){
    m_identifier = identifier;
}

inline const QString &QASTNode::typeName() const{
    return m_typeName;
}

inline const QString &QASTNode::identifier() const{
    return m_identifier;
}

inline const QList<QASTNode *> &QASTNode::astChildren() const{
    return m_children;
}

inline QList<QASTNode *> QASTNode::astChildren(const QString &type) const{
    QList<QASTNode*> astChildren;
    for ( NodeList::ConstIterator it = m_children.begin(); it != m_children.end(); ++it ){
        if ( (*it)->typeName() == type )
            astChildren.append(*it);
    }
    return astChildren;
}

inline QVariant QASTNode::astChild(const QString &identifier){
    for ( NodeList::iterator it = m_children.begin(); it != m_children.end(); ++it ){
        if ( (*it)->identifier() == identifier )
            return QVariant::fromValue(*it);
    }
    return QVariant();
}

inline QVariant QASTNode::astChild(const QString &typeString, const QString &identif){
    for ( NodeList::iterator it = m_children.begin(); it != m_children.end(); ++it ){
        if ( (*it)->identifier() == identif && (*it)->typeName == typeString )
            return QVariant::fromValue(*it);
    }
    return QVariant();
}

inline const SourceLocation &QASTNode::rangeStartLocation() const{
    return *m_rangeStartLocation;
}

inline const SourceLocation &QASTNode::rangeEndLocation() const{
    return *m_rangeEndLocation;
}

inline const SourceLocation &QASTNode::cursorLocation() const{
    return *m_cursorLocation;
}

inline const SourceLocation &QASTNode::bodyStartLocation() const{
    return *m_rangeStartLocation;
}

inline const SourceLocation &QASTNode::bodyEndLocation() const{
    return *m_rangeEndLocation;
}

inline void QASTNode::addChild(QASTNode *node){
    m_children.push_back(node);
}

inline QASTNode::Iterator QASTNode::childrenBegin(){
    return m_children.begin();
}

inline QASTNode::Iterator QASTNode::childrenEnd(){
    return m_children.end();
}

inline QASTNode::ConstIterator QASTNode::childrenBegin() const{
    return m_children.begin();
}

inline QASTNode::ConstIterator QASTNode::childrenEnd() const{
    return m_children.end();
}


}}// namespace

#endif // CSANODE_HPP
