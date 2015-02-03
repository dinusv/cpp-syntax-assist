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


#include "SyntaxTreeModel.hpp"
#include "QSourceLocation.hpp"
#include "QASTNode.hpp"

using namespace csa;
using namespace csa::ast;

class SyntaxTreeItem{

public:
    int      indent;
    int      line;
    QASTNode* node;
    QString  identifier;
    QString  type;

};

SyntaxTreeModel::SyntaxTreeModel(QObject *parent)
    : QAbstractListModel(parent){

    m_roles[SyntaxTreeModel::Identifier] = "identifier";
    m_roles[SyntaxTreeModel::Indent]     = "indent";
    m_roles[SyntaxTreeModel::Type]       = "type";
    m_roles[SyntaxTreeModel::Line]       = "line";
}

void SyntaxTreeModel::clearAndReset(){
    beginResetModel();
    clear();
    endResetModel();
}

void SyntaxTreeModel::clear(){
    for ( QList<SyntaxTreeItem*>::iterator it = m_items.begin(); it != m_items.end(); ++it ){
        delete (*it);
    }
    m_items.clear();
}

void SyntaxTreeModel::parse(QASTNode *root){
    beginResetModel();
    clear();
    recursiveParse(root, 0);
    endResetModel();
}

void SyntaxTreeModel::recursiveParse(QASTNode *node, int indent){

    SyntaxTreeItem* item = new SyntaxTreeItem;
    item->identifier   = node->identifier();
    item->indent = indent;
    item->node   = node;
    item->type   = node->typeName();
    item->line   = node->rangeStartLocation().line();
    m_items.append(item);
    for ( QASTNode::Iterator it = node->childrenBegin(); it != node->childrenEnd(); ++it ){
        recursiveParse(*it, indent + 1);
    }
}

QVariant SyntaxTreeModel::data(const QModelIndex &index, int role) const{
    if ( role == SyntaxTreeModel::Identifier)
        return m_items[index.row()]->identifier;
    else if ( role == SyntaxTreeModel::Indent )
        return m_items[index.row()]->indent;
    else if ( role == SyntaxTreeModel::Type )
        return m_items[index.row()]->type;
    else if ( role == SyntaxTreeModel::Line )
        return m_items[index.row()]->line;
    return QVariant();
}

void SyntaxTreeModel::setSelected(csa::ast::QASTNode *node){
    for ( int i = 0; i < m_items.size(); ++i ){
        SyntaxTreeItem* item = m_items[i];
        if ( item->node == node ){
            setSelected(i);
            return;
        }
    }
}
