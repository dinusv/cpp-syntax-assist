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


#ifndef QASTNODE_HPP
#define QASTNODE_HPP

#include <string>
#include <vector>
#include <map>
#include <QObject>
#include <QList>
#include <QVariant>


namespace csa{
class QAnnotatedTokenSet;
class QSourceLocation;

class QCodeBase;

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

    QASTNode(const QString& typeName,
            QAnnotatedTokenSet* tokenSet,
            QSourceLocation* cursorLocation,
            QSourceLocation* rangeStartLocation,
            QSourceLocation* rangeEndLocation,
            QASTNode* parent = 0);
    virtual ~QASTNode();

    // Callable Methods
    // ----------------

public slots:
    QString typeName() const;
    QString identifier() const;

    virtual QString content() const = 0;

    QList<csa::ast::QASTNode*> astChildren() const;
    QList<csa::ast::QASTNode*> astChildren(const QString& type) const;

    csa::ast::QASTNode* astParent();
    csa::ast::QASTNode* astChild(const QString& identifier);
    csa::ast::QASTNode* astChild(const QString& typeString, const QString& identif);
    csa::ast::QASTNode* find(const QString& typeString, const QString& identifier);
    csa::ast::QASTNode* findFirst(const QString& typeString);
    csa::ast::QASTNode* parentFind(const QString& typeString);
    csa::ast::QASTNode* childAfter(csa::ast::QASTNode* child);
    csa::ast::QASTNode* next();

    void append(const QString& value);
    void prepend(const QString& value);
    void before(const QString& value);
    void after(const QString& value);
    void afterln(const QString& value);

public:
    // Location Getters
    // ----------------

    const QSourceLocation& rangeStartLocation() const;
    const QSourceLocation& rangeEndLocation() const;
    const QSourceLocation& cursorLocation() const;

    virtual const QSourceLocation& bodyStartLocation() const;
    virtual const QSourceLocation& bodyEndLocation() const;

    // Initializers
    // ------------

    virtual QASTNode* propagateUserCursor(const QSourceLocation& location);

    // Children Handlers
    // -----------------

    void addChild(QASTNode* node);
    QASTNode* find(QASTNode *node);

    Iterator childrenBegin();
    Iterator childrenEnd();
    ConstIterator childrenBegin() const;
    ConstIterator childrenEnd() const;

    void removeChildren();

    // Debugging
    // ---------

    void dump(QString& out, int depth = 0) const;

protected:
    void setIdentifier(const QString& identifier);

    virtual void insert(const QString& value, const QSourceLocation& location);

private:
    // Parameters
    // ----------

    QString m_typeName;
    QString m_identifier;

    QAnnotatedTokenSet* m_tokenSet;

    QSourceLocation* m_cursorLocation;
    QSourceLocation* m_rangeStartLocation;
    QSourceLocation* m_rangeEndLocation;

    NodeList     m_children;
    QASTNode*    m_parent;
    QCodeBase*   m_codeBase;

};

inline void QASTNode::setIdentifier(const QString &identifier){
    m_identifier = identifier;
}

inline QString QASTNode::typeName() const{
    return m_typeName;
}

inline QString QASTNode::identifier() const{
    return m_identifier;
}

inline QList<QASTNode*> QASTNode::astChildren() const{
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

inline QASTNode* QASTNode::astChild(const QString &identifier){
    for ( NodeList::iterator it = m_children.begin(); it != m_children.end(); ++it ){
        if ( (*it)->identifier() == identifier )
            return *it;
    }
    return 0;
}

inline QASTNode* QASTNode::astChild(const QString &typeString, const QString &identif){
    for ( NodeList::iterator it = m_children.begin(); it != m_children.end(); ++it ){
        if ( (*it)->identifier() == identif && (*it)->typeName() == typeString )
            return *it;
    }
    return 0;
}

inline const QSourceLocation &QASTNode::rangeStartLocation() const{
    return *m_rangeStartLocation;
}

inline const QSourceLocation &QASTNode::rangeEndLocation() const{
    return *m_rangeEndLocation;
}

inline const QSourceLocation &QASTNode::cursorLocation() const{
    return *m_cursorLocation;
}

inline const QSourceLocation &QASTNode::bodyStartLocation() const{
    return *m_rangeStartLocation;
}

inline const QSourceLocation &QASTNode::bodyEndLocation() const{
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

Q_DECLARE_METATYPE(csa::ast::QASTNode*)

#endif // QASTNODE_HPP
