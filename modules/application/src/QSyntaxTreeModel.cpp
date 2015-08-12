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

#include "QSyntaxTreeModel.hpp"
#include "QSourceLocation.hpp"
#include "QASTNode.hpp"
#include "QASTFile.hpp"

using namespace csa;
using namespace csa::ast;

class QSyntaxTreeItem{

public:
    int       indent;
    int       line;
    QASTNode* node;
    QString   display;
    QString   type;

    bool      isCollapsible;
    bool      isCollapsed;
};

QSyntaxTreeModel::QSyntaxTreeModel(QObject *parent)
    : QAbstractListModel(parent)
    , m_expandLevel(3){

    m_roles[QSyntaxTreeModel::Identifier]    = "identifier";
    m_roles[QSyntaxTreeModel::Indent]        = "indent";
    m_roles[QSyntaxTreeModel::Type]          = "type";
    m_roles[QSyntaxTreeModel::Line]          = "line";
    m_roles[QSyntaxTreeModel::IsCollapsible] = "isCollapsible";
    m_roles[QSyntaxTreeModel::IsCollapsed]   = "isCollapsed";
    m_roles[QSyntaxTreeModel::Breadcrumbs]   = "breadcrumbs";
}

void QSyntaxTreeModel::clearAndReset(){
    beginResetModel();
    clear();
    endResetModel();
}

void QSyntaxTreeModel::clear(){
    for ( QList<QSyntaxTreeItem*>::iterator it = m_items.begin(); it != m_items.end(); ++it ){
        delete (*it);
    }
    m_items.clear();
}

void QSyntaxTreeModel::parse(const QList<ast::QASTFile *> &files){
    beginResetModel();
    clear();
    for ( QList<ast::QASTFile*>::const_iterator it = files.begin(); it != files.end(); ++it ){
        QASTFile* file = *it;
        recursiveParse(file, 0);
    }
    endResetModel();
}

void QSyntaxTreeModel::recursiveParse(QASTNode *node, int indent){

    QSyntaxTreeItem* item = new QSyntaxTreeItem;
    item->display = node->content();
    item->indent  = indent;
    item->node    = node;
    item->type    = node->typeName();
    item->line    = node->rangeStartLocation()->line();
    item->isCollapsed = true;
    item->isCollapsible = node->children().size() > 0;
    m_items.append(item);

    if ( indent + 1 < m_expandLevel ){
        item->isCollapsed = false;
        for ( QASTNode::Iterator it = node->childrenBegin(); it != node->childrenEnd(); ++it ){
            recursiveParse(*it, indent + 1);
        }
    }
}

QVariant QSyntaxTreeModel::data(const QModelIndex &index, int role) const{
    if ( role == QSyntaxTreeModel::Identifier)
        return m_items[index.row()]->display;
    else if ( role == QSyntaxTreeModel::Indent )
        return m_items[index.row()]->indent;
    else if ( role == QSyntaxTreeModel::Type )
        return m_items[index.row()]->type;
    else if ( role == QSyntaxTreeModel::Line )
        return m_items[index.row()]->line;
    else if ( role == QSyntaxTreeModel::IsCollapsed )
        return m_items[index.row()]->isCollapsed;
    else if ( role == QSyntaxTreeModel::IsCollapsible )
        return m_items[index.row()]->isCollapsible;
    else if ( role == QSyntaxTreeModel::Breadcrumbs )
        return m_items[index.row()]->node->breadcrumbs();
    return QVariant();
}

void QSyntaxTreeModel::collapse(int index){
    QSyntaxTreeItem* item = m_items[index];

    if ( item->isCollapsible && !item->isCollapsed ){

        item->isCollapsed = true;
        emit dataChanged(createIndex(index, 0), createIndex(index, 0));

        recursiveCollapse(index);
    }
}

void QSyntaxTreeModel::recursiveCollapse(int index){
    QSyntaxTreeItem* item = m_items[index];

    int totalChildren = item->node->astChildren().size();

    for ( int i = index + 1; i < index + 1 + totalChildren; ++i ){
        if ( !m_items[i]->isCollapsed )
            recursiveCollapse(i);
    }

    beginRemoveRows(QModelIndex(), index + 1, index + totalChildren);

    for ( int i = index + 1; i < index + 1 + totalChildren; ++i ){
        delete m_items[i];
    }
    m_items.erase(m_items.begin() + index + 1, m_items.begin() + index + totalChildren + 1);

    endRemoveRows();
}

void QSyntaxTreeModel::expand(int index){
    QSyntaxTreeItem* item = m_items[index];

    if ( item->isCollapsible && item->isCollapsed ){

        item->isCollapsed = false;
        emit dataChanged(createIndex(index, 0), createIndex(index, 0));

        int totalChildren = item->node->astChildren().size();

        beginInsertRows(QModelIndex(), index + 1, index + totalChildren);

        for ( QASTNode::NodeList::ConstIterator it = item->node->astChildren().begin();
              it != item->node->astChildren().end(); ++it
        ){
            ++index;
            QSyntaxTreeItem* newItem = new QSyntaxTreeItem;
            newItem->node          = (*it);
            newItem->display       = (*it)->content();
            newItem->indent        = item->indent + 1;
            newItem->isCollapsed   = true;
            newItem->isCollapsible = (*it)->children().size();
            newItem->line          = (*it)->rangeStartLocation() ? (*it)->rangeStartLocation()->line() : 0;
            newItem->type          = (*it)->typeName();

            m_items.insert(index, newItem);
        }

        endInsertRows();

    }
}

void QSyntaxTreeModel::setSelected(csa::ast::QASTNode *node){
    for ( int i = 0; i < m_items.size(); ++i ){
        QSyntaxTreeItem* item = m_items[i];
        if ( item->node == node ){
            setSelected(i);
            return;
        }
    }
}
