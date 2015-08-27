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

#include "QASTCollapsibleModel.hpp"
#include "QSourceLocation.hpp"
#include "QASTNode.hpp"
#include "QASTFile.hpp"

#include <QDebug>

using namespace csa;
using namespace csa::ast;

// QSyntaxTreeItem
// ---------------

class QSyntaxTreeItem{

public:
    QSyntaxTreeItem(QASTNode* pnode);

    int       indent;
    int       line;
    QASTNode* node;
    QString   display;
    QString   type;

    bool      isCollapsible;
    bool      isCollapsed;
};

QSyntaxTreeItem::QSyntaxTreeItem(QASTNode *pnode)
    : indent(0)
    , line(pnode->rangeStartLocation() ? pnode->rangeStartLocation()->line() : 0)
    , node(pnode)
    , display(pnode->content())
    , type(pnode->typeName())
    , isCollapsible(pnode->children().size() > 0)
    , isCollapsed(false)
{
}


// QASTCollapsibleModel
// --------------------

QASTCollapsibleModel::QASTCollapsibleModel(const QList<QASTFile *> &files, QObject *parent)
    : QAbstractListModel(parent)
    , m_expandLevel(3){
    m_roles[QASTCollapsibleModel::Identifier]    = "identifier";
    m_roles[QASTCollapsibleModel::Indent]        = "indent";
    m_roles[QASTCollapsibleModel::Type]          = "type";
    m_roles[QASTCollapsibleModel::Line]          = "line";
    m_roles[QASTCollapsibleModel::IsCollapsible] = "isCollapsible";
    m_roles[QASTCollapsibleModel::IsCollapsed]   = "isCollapsed";
    m_roles[QASTCollapsibleModel::Breadcrumbs]   = "breadcrumbs";

    for ( QList<QASTFile*>::const_iterator it = files.begin(); it != files.end(); ++it ){
        addFile(*it);
    }
}

void QASTCollapsibleModel::clear(){
    for ( QList<QSyntaxTreeItem*>::iterator it = m_items.begin(); it != m_items.end(); ++it ){
        delete (*it);
    }
    m_items.clear();
}

void QASTCollapsibleModel::recursiveParse(int index){
    QSyntaxTreeItem* item = m_items[index];

    if ( item->isCollapsible && item->isCollapsed && item->indent + 1 < m_expandLevel ){
        expand(index);

        for ( int i = index + 1; i < index + 1 + item->node->children().size(); ++i ){
            recursiveParse(i);
        }
    }
}

QVariant QASTCollapsibleModel::data(const QModelIndex &index, int role) const{
    if ( role == QASTCollapsibleModel::Identifier)
        return m_items[index.row()]->display;
    else if ( role == QASTCollapsibleModel::Indent )
        return m_items[index.row()]->indent;
    else if ( role == QASTCollapsibleModel::Type )
        return m_items[index.row()]->type;
    else if ( role == QASTCollapsibleModel::Line )
        return m_items[index.row()]->line;
    else if ( role == QASTCollapsibleModel::IsCollapsed )
        return m_items[index.row()]->isCollapsed;
    else if ( role == QASTCollapsibleModel::IsCollapsible )
        return m_items[index.row()]->isCollapsible;
    else if ( role == QASTCollapsibleModel::Breadcrumbs )
        return m_items[index.row()]->node->breadcrumbs();
    return QVariant();
}

void QASTCollapsibleModel::collapse(int index){
    QSyntaxTreeItem* item = m_items[index];

    if ( item->isCollapsible && !item->isCollapsed ){
        recursiveCollapse(index);
    }
}

void QASTCollapsibleModel::recursiveCollapse(int index){
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

    item->isCollapsed = true;
    emit dataChanged(createIndex(index, 0), createIndex(index, 0));
}

void QASTCollapsibleModel::expand(int index){
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
            QSyntaxTreeItem* newItem = new QSyntaxTreeItem(*it);
            newItem->indent        = item->indent + 1;
            newItem->isCollapsed   = true;
            m_items.insert(index, newItem);
        }

        endInsertRows();

    }
}

void QASTCollapsibleModel::addFile(QASTFile *file){
    beginInsertRows(QModelIndex(), m_items.size(), m_items.size());
    QSyntaxTreeItem* item = new QSyntaxTreeItem(file);
    item->isCollapsed = true;
    m_items.append(item);
    endInsertRows();

    recursiveParse(m_items.size() - 1);

}

void QASTCollapsibleModel::collapseFile(QASTFile *file){
    for ( int i = 0; i < m_items.size(); ++i ){
        if ( m_items[i]->node == file ){
            recursiveCollapse(i);
            return;
        }
    }
}

void QASTCollapsibleModel::reparseFile(QASTFile *file){
    for ( int i = 0; i < m_items.size(); ++i ){
        if ( m_items[i]->node == file ){
            recursiveParse(i);
            return;
        }
    }
}

void QASTCollapsibleModel::selectNode(QASTNode *node){
    for ( int i = 0; i < m_items.size(); ++i ){
        QSyntaxTreeItem* item = m_items[i];
        if ( item->node == node ){
            setSelected(i);
            return;
        }
    }
}
