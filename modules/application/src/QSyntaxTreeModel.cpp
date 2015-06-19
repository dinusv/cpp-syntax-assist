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

};

QSyntaxTreeModel::QSyntaxTreeModel(QObject *parent)
    : QAbstractListModel(parent){

    m_roles[QSyntaxTreeModel::Identifier] = "identifier";
    m_roles[QSyntaxTreeModel::Indent]     = "indent";
    m_roles[QSyntaxTreeModel::Type]       = "type";
    m_roles[QSyntaxTreeModel::Line]       = "line";
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
    m_items.append(item);
    for ( QASTNode::Iterator it = node->childrenBegin(); it != node->childrenEnd(); ++it ){
        recursiveParse(*it, indent + 1);
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
    return QVariant();
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
