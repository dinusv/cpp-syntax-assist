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
#include "QSourceLocation.hpp"
#include "QAnnotatedTokenSet.hpp"

namespace csa{ namespace ast{

int QASTNode::dumpIndentation = 1;

QASTNode::QASTNode(
        const QString &typeName,
        QAnnotatedTokenSet* tokenSet,
        QSourceLocation* cursorLocation,
        QSourceLocation* rangeStartLocation,
        QSourceLocation* rangeEndLocation,
        QASTNode *parent)
    : QObject(parent)
    , m_typeName(typeName)
    , m_tokenSet(tokenSet)
    , m_cursorLocation(cursorLocation)
    , m_rangeStartLocation(rangeStartLocation)
    , m_rangeEndLocation(rangeEndLocation)
{
    if ( parent != 0 )
        parent->addChild(this);

    if ( m_tokenSet )
        m_tokenSet->setParent(this);
}

QASTNode::~QASTNode(){
    removeChildren();
    delete m_rangeStartLocation;
    delete m_rangeEndLocation;
    delete m_cursorLocation;
}

QString QASTNode::content() const{
    return identifier();
}

QString QASTNode::prop(const QString &) const{
    return "";
}

QList<QAnnotatedToken*> QASTNode::associatedTokens(){
    if ( m_tokenSet )
        return m_tokenSet->tokenList();

    return QList<QAnnotatedToken*>();
}

QASTNode*QASTNode::astParent(){
    QASTNode* p = qobject_cast<QASTNode*>(parent());
    return p;
}

void QASTNode::append(const QString& value){
    insert(value, bodyEndLocation());
}

void QASTNode::prepend(const QString& value){
    insert(value, bodyStartLocation());
}

void QASTNode::before(const QString& value){
    insert(value, rangeStartLocation());
}

void QASTNode::after(const QString& value){
    insert(value, rangeEndLocation());
}

void QASTNode::afterln(const QString& value){
    if ( !m_tokenSet )
        after(value);

    CXSourceLocation afterLocation = clang_getLocation(
                m_tokenSet->translationUnit(),
                clang_getFile(m_tokenSet->translationUnit(), rangeEndLocation().filePath().toUtf8()),
                rangeEndLocation().line() + 1,
                1); // location will be automatically truncated to max lines

    insert(value, QSourceLocation(afterLocation));
}

void QASTNode::dump(QString& out, int depth) const{
    QString space("");
    for ( int i = 0; i < depth * dumpIndentation; ++i )
        space.append(" ");

    out.append(space + content() + "\n");
    for ( NodeList::const_iterator it = m_children.begin(); it != m_children.end(); ++it ){
        (*it)->dump(out, depth + 1);
    }
}

void QASTNode::insert(const QString& value, const QSourceLocation& location){
    QASTNode* p = qobject_cast<QASTNode*>(parent());
    if ( p )
        p->insert(value, location);
}

void QASTNode::setAstParent(QASTNode* parent){
    if ( parent )
        parent->addChild(this);
    setParent(parent);
}

QASTNode *QASTNode::propagateUserCursor(const QSourceLocation &location){
    QASTNode* base = 0;
    if ( m_rangeStartLocation->offset() <= location.offset() && m_rangeEndLocation->offset() >= location.offset() ){
        base = this;
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

QASTNode *QASTNode::find(const QString &typeString, const QString& identifier){
    for ( NodeList::iterator it = m_children.begin(); it != m_children.end(); ++it ){
        QASTNode* child = *it;
        if ( child->typeName() == typeString && child->identifier() == identifier )
            return child;
        QASTNode* childFind = child->find(typeString, identifier);
        if ( childFind != 0 )
            return childFind;
    }
    return 0;
}

QASTNode* QASTNode::findFirst(const QString& typeString){
    for ( NodeList::iterator it = m_children.begin(); it != m_children.end(); ++it ){
        QASTNode* child = *it;
        if ( child->typeName() == typeString )
            return child;
        QASTNode* childFind = child->findFirst(typeString);
        if ( childFind != 0 )
            return childFind;
    }
    return 0;
}

QASTNode* QASTNode::parentFind(const QString& typeString){
    if ( parent() == 0 )
        return 0;

    QASTNode* p = qobject_cast<QASTNode*>(parent());
    if ( p ){
        if ( p->typeName() == typeString )
            return p;
        return p->parentFind(typeString);
    }

    return 0;
}

QASTNode* QASTNode::childAfter(QASTNode* child){
    for ( NodeList::iterator it = m_children.begin(); it != m_children.end(); ++it ){
        QASTNode* currentChild = *it;
        if ( currentChild == child ){
            ++it;
            if ( it == m_children.end() )
                return 0;
            return *it;
        }
    }
    return 0;
}

QASTNode *QASTNode::childBefore(QASTNode *child){
    QASTNode* prevChild = 0;
    for ( NodeList::iterator it = m_children.begin(); it != m_children.end(); ++it ){
        QASTNode* currentChild = *it;
        if ( currentChild == child )
            return prevChild;
        prevChild = currentChild;
    }
    return 0;
}

QASTNode* QASTNode::next(){
    QASTNode* p = qobject_cast<QASTNode*>(parent());
    if ( p )
        return p->childAfter(this);
    return 0;
}

QASTNode *QASTNode::prev(){
    QASTNode* p = qobject_cast<QASTNode*>(parent());
    if ( p )
        return p->childBefore(this);
    return 0;
}

void QASTNode::removeChildren(){
    for ( NodeList::iterator it = m_children.begin(); it != m_children.end(); ++it )
        delete *it;
    m_children.clear();
}

}}// namespace
