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


#include "qastnode.h"
#include "qsourcelocation.h"
#include "qsourcelocation_p.h"
#include "qannotatedtokenset.h"

#include "qastsearch.h"

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

    if ( m_rangeStartLocation )
        m_rangeStartLocation->setParent(this);
    if ( m_rangeEndLocation )
        m_rangeEndLocation->setParent(this);
    if ( m_cursorLocation )
        m_cursorLocation->setParent(this);
}

QASTNode::~QASTNode(){
    removeChildren();
    delete m_rangeStartLocation;
    delete m_rangeEndLocation;
    delete m_cursorLocation;
}

const QASTNode*QASTNode::astParent() const{
    const QASTNode* p = qobject_cast<QASTNode*>(parent());
    return p;
}


QString QASTNode::breadcrumbs() const{
    if ( astParent() )
        return astParent()->breadcrumbs() + "/" + declaration().replace('/', "\\/");
    return declaration().replace('/', "\\/");
}

QString QASTNode::declaration() const{
    return identifier();
}

QVariant QASTNode::prop(const QString &) const{
    return QVariant();
}

QString QASTNode::text(){
    return text(rangeStartLocation(), rangeEndLocation());
}

QList<QObject*> QASTNode::children(const QString& type){
    if ( type == "" )
        return castNodeListToObjectList(m_children);

    QList<QObject*> foundChildren;
    for ( ConstIterator it = m_children.begin(); it != m_children.end(); ++it ){
        if ( (*it)->typeName() == type )
            foundChildren.append(*it);
    }
    return foundChildren;
}

QList<QObject*> QASTNode::arguments() const{
    return QList<QObject*>();
}

QList<QObject*> QASTNode::associatedTokens(){
    if ( m_tokenSet ){
        QList<QObject*> tokens;
        for ( QList<QAnnotatedToken*>::const_iterator it = m_tokenSet->tokenList().begin();
              it != m_tokenSet->tokenList().end();
              ++it
        ){
            tokens.append(*it);
        }
        return tokens;
    }

    return QList<QObject*>();
}

QASTNode* QASTNode::astParent(){
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

    CXSourceLocation cxAfterLocation = clang_getLocation(
        m_tokenSet->translationUnit(),
        clang_getFile(m_tokenSet->translationUnit(), rangeEndLocation()->filePath().toUtf8()),
        rangeEndLocation()->line() + 1,
        1); // location will be automatically truncated to max lines

    QSourceLocation* afterLocation = new QSourceLocation(createSourceLocation(cxAfterLocation));
    insert(value, afterLocation);
    delete afterLocation;
}

void QASTNode::remove(){
    erase(rangeStartLocation(), rangeEndLocation());
}

void QASTNode::dump(QString& out, int depth) const{
    QString space("");
    for ( int i = 0; i < depth * dumpIndentation; ++i )
        space.append(" ");

    out.append(space + declaration() + "\n");
    for ( NodeList::const_iterator it = m_children.begin(); it != m_children.end(); ++it ){
        (*it)->dump(out, depth + 1);
    }
}

QASTNode* QASTNode::findFirst(const QASTSearch& searchPattern, const QString& type){
    QASTSearch::MatchResult result = searchPattern.matchCurrentSegment(this);
    if ( result == QASTSearch::FullMatch && ( type == "" || type == typeName() ) )
        return this;
    else if ( result == QASTSearch::SegmentMatch ){
        QASTSearch nextPattern = QASTSearch(searchPattern).nextPosition();
        for ( NodeList::iterator it = m_children.begin(); it != m_children.end(); ++it ){
            QASTNode* child = *it;
            QASTNode* foundChild = child->findFirst(nextPattern, type);
            if ( foundChild )
                return foundChild;
        }
    }

    for ( NodeList::iterator it = m_children.begin(); it != m_children.end(); ++it ){
        QASTNode* child = *it;
        QASTNode* foundChild = child->findFirst(searchPattern, type);
        if ( foundChild )
            return foundChild;
    }
    return 0;
}

QList<QObject*> QASTNode::find(const QASTSearch& searchPattern, const QString& type){
    QList<QObject*> foundChildren;

    QASTSearch::MatchResult result = searchPattern.matchCurrentSegment(this);
    if ( result == QASTSearch::FullMatch && ( type == "" || type == typeName() ) ){
        foundChildren.append(this);
    } else if ( result == QASTSearch::SegmentMatch ){
        QASTSearch nextPattern = QASTSearch(searchPattern).nextPosition();
        for ( NodeList::iterator it = m_children.begin(); it != m_children.end(); ++it ){
            QASTNode* child = *it;
            QList<QObject*> foundNodes = child->find(nextPattern, type);
            if ( foundNodes.size() > 0 )
                foundChildren << foundNodes;
        }
    }

    for ( NodeList::iterator it = m_children.begin(); it != m_children.end(); ++it ){
        QASTNode* child = *it;
        QList<QObject*> foundNodes = child->find(searchPattern, type);
        if ( foundNodes.size() > 0 )
            foundChildren << foundNodes;
    }

    return foundChildren;
}

bool QASTNode::insert(const QString& value, QSourceLocation* location){
    QASTNode* p = astParent();
    if ( p )
        return p->insert(value, location);
    return false;
}

bool QASTNode::erase(QSourceLocation *from, QSourceLocation *to){
    QASTNode* p = astParent();
    if ( p )
        return p->erase(from, to);
    return false;
}

QString QASTNode::text(QSourceLocation *from, QSourceLocation *to){
    QASTNode* p = astParent();
    if ( p )
        return p->text(from, to);
    return "";
}

void QASTNode::setAstParent(QASTNode* parent){
    if ( parent )
        parent->addChild(this);
    setParent(parent);
}

QASTNode *QASTNode::nodeAt(const QSourceLocation &location){
    QASTNode* base = 0;
    if ( m_rangeStartLocation->offset() <= location.offset() && m_rangeEndLocation->offset() >= location.offset() ){
        base = this;
        for ( NodeList::iterator it = m_children.begin(); it != m_children.end(); ++it ){
            QASTNode* result = (*it)->nodeAt(location);
            if ( result != 0 )
                base = result;
        }
    }
    return base;
}

QASTNode *QASTNode::findNode(QASTNode* node){
    for ( NodeList::iterator it = m_children.begin(); it != m_children.end(); ++it ){
        QASTNode* child = *it;
        if ( child == node )
            return child;
        QASTNode* childFind = child->findNode(node);
        if ( childFind != 0 )
            return childFind;
    }
    return 0;
}

QList<QObject*> QASTNode::find(const QString &searchData, const QString& type){
    if ( QASTSearch::isPattern(searchData) )
        return find(QASTSearch(searchData), type);

    QList<QObject*> foundData;
    if ( identifier() == searchData && (type == "" || type == typeName()) )
        foundData << this;

    for ( NodeList::iterator it = m_children.begin(); it != m_children.end(); ++it ){
        QASTNode* child = *it;

        QList<QObject*> foundChildren = child->find(searchData, type);
        if ( foundChildren.size() > 0 )
            foundData << foundChildren;
    }

    return foundData;
}

QASTNode* QASTNode::findFirst(const QString& searchData, const QString& type){
    if ( QASTSearch::isPattern(searchData) )
        return findFirst(QASTSearch(searchData), type);

    if ( identifier() == searchData && (type == "" || type == typeName()) )
        return this;

    for ( NodeList::iterator it = m_children.begin(); it != m_children.end(); ++it ){
        QASTNode* child = *it;
        QASTNode* foundChild = child->findFirst(searchData, type);
        if ( foundChild )
            return foundChild;
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

QList<QObject*> QASTNode::castNodeListToObjectList(const QList<QASTNode*>& list){
    QList<QObject*> objectList;
    for ( ConstIterator it = list.begin(); it != list.end(); ++it)
        objectList.append(*it);
    return objectList;
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
