/****************************************************************************
**
** Copyright (C) 2014-2015 Dinu SV.
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

#include "QCSAGlobal.hpp"
#include <QObject>
#include <QList>
#include <QVariant>

namespace csa{

class QAnnotatedToken;
class QAnnotatedTokenSet;
class QSourceLocation;
class QASTSearch;
class QCodeBase;

namespace ast{

class Q_CSA_EXPORT QASTNode : public QObject{

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

    const QList<csa::ast::QASTNode*>& astChildren() const;
    QList<csa::ast::QASTNode*> astChildren(const QString& type) const;

    const csa::ast::QASTNode* astParent() const;

public slots:
    QString typeName() const;
    QString identifier() const;

    QString breadcrumbs() const;

    virtual QString content() const;
    virtual QString prop(const QString& key) const;

    QList<QObject*> children() const;
    QList<QObject*> children(const QString& type);

    virtual QList<QObject*> arguments() const;

    QList<QObject*> associatedTokens();

    csa::ast::QASTNode* astParent();
    csa::ast::QASTNode* firstChild();
    csa::ast::QASTNode* firstChild(const QString& identifier);
    csa::ast::QASTNode* firstChild(const QString& identifier, const QString& typeString);
    csa::ast::QASTNode* lastChild();
    csa::ast::QASTNode* lastChild(const QString& identifier);
    csa::ast::QASTNode* lastChild(const QString& identifier, const QString& typeString);

    csa::ast::QASTNode* next();
    csa::ast::QASTNode* prev();

    csa::ast::QASTNode* find(csa::ast::QASTNode *node);
    QList<QObject*> find(const QString& searchData, const QString& type = "");
    csa::ast::QASTNode* findFirst(const QString& searchData, const QString& type = "");
    csa::ast::QASTNode* parentFind(const QString& typeString);

    void append(const QString& value);
    void prepend(const QString& value);
    void before(const QString& value);
    void after(const QString& value);
    void afterln(const QString& value);

    void remove();

    // Location Getters
    // ----------------

    csa::QSourceLocation* rangeStartLocation();
    csa::QSourceLocation* rangeEndLocation();
    csa::QSourceLocation* cursorLocation();

    virtual csa::QSourceLocation* bodyStartLocation();
    virtual csa::QSourceLocation* bodyEndLocation();

public:
    // Initializers
    // ------------

    virtual QASTNode* propagateUserCursor(const QSourceLocation& location);

    // Children Handlers
    // -----------------

    void addChild(QASTNode* node);

    Iterator childrenBegin();
    Iterator childrenEnd();
    ConstIterator childrenBegin() const;
    ConstIterator childrenEnd() const;

    void removeChildren();

    virtual bool insert(const QString& value, QSourceLocation* location);
    virtual bool erase(QSourceLocation* from, QSourceLocation* to);

    void setAstParent(csa::ast::QASTNode* parent);

    // Debugging
    // ---------

    void dump(QString& out, int depth = 0) const;

protected:
    void setIdentifier(const QString& identifier);
    csa::ast::QASTNode* findFirst(const QASTSearch& searchPattern, const QString& type = "");
    QList<QObject*> find(const QASTSearch& searchPattern, const QString& type = "");

    csa::ast::QASTNode* childAfter(csa::ast::QASTNode* child);
    csa::ast::QASTNode* childBefore(csa::ast::QASTNode* child);

    QAnnotatedTokenSet* tokenSet();

    static QList<QObject*> castNodeListToObjectList(const QList<QASTNode*>& list);

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

inline QAnnotatedTokenSet*QASTNode::tokenSet(){
    return m_tokenSet;
}

inline QString QASTNode::typeName() const{
    return m_typeName;
}

inline QString QASTNode::identifier() const{
    return m_identifier;
}

inline const QList<QASTNode*>& QASTNode::astChildren() const{
    return m_children;
}

inline QList<QASTNode*> QASTNode::astChildren(const QString &type) const{
    QList<QASTNode*> children;
    for ( NodeList::ConstIterator it = m_children.begin(); it != m_children.end(); ++it ){
        if ( (*it)->typeName() == type )
            children.append(*it);
    }
    return children;
}

inline QASTNode* QASTNode::firstChild(const QString &identifier){
    for ( NodeList::iterator it = m_children.begin(); it != m_children.end(); ++it ){
        if ( (*it)->identifier() == identifier )
            return *it;
    }
    return 0;
}

inline QASTNode* QASTNode::firstChild(const QString &identif, const QString &typeString){
    for ( NodeList::iterator it = m_children.begin(); it != m_children.end(); ++it ){
        if ( (*it)->identifier() == identif && (*it)->typeName() == typeString )
            return *it;
    }
    return 0;
}

inline QASTNode *QASTNode::lastChild(const QString &identifier){
    for( int i = m_children.size() - 1; i >= 0; --i ){
        QASTNode* node = m_children[i];
        if ( node->identifier() == identifier)
            return node;
    }
    return 0;
}

inline QASTNode *QASTNode::lastChild(const QString &identif, const QString &typeString){
    for( int i = m_children.size() - 1; i >= 0; --i ){
        QASTNode* node = m_children[i];
        if ( node->identifier() == identif && node->typeName() == typeString)
            return node;
    }
    return 0;
}

inline QSourceLocation* QASTNode::rangeStartLocation(){
    return m_rangeStartLocation;
}

inline QSourceLocation* QASTNode::rangeEndLocation(){
    return m_rangeEndLocation;
}

inline QSourceLocation* QASTNode::cursorLocation(){
    return m_cursorLocation;
}

inline QSourceLocation* QASTNode::bodyStartLocation(){
    return m_rangeStartLocation;
}

inline QSourceLocation* QASTNode::bodyEndLocation(){
    return m_rangeEndLocation;
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
