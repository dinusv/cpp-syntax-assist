#include "SyntaxTreeModel.hpp"
#include "SourceLocation.hpp"
#include "CSANode.hpp"

using namespace csa;
using namespace csa::ast;

class SyntaxTreeItem{

public:
    int      indent;
    int      line;
    CSANode* node;
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

void SyntaxTreeModel::safeClear(){
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

void SyntaxTreeModel::parse(CSANode *root){
    beginResetModel();
    clear();
    recursiveParse(root, 0);
    endResetModel();
}

void SyntaxTreeModel::recursiveParse(CSANode *node, int indent){

    SyntaxTreeItem* item = new SyntaxTreeItem;
    item->identifier   = node->identifier().c_str();
    item->indent = indent;
    item->node   = node;
    item->type   = node->typeString().c_str();
    item->line   = node->rangeStartLocation().line();
    m_items.append(item);
    for ( CSANode::Iterator it = node->childrenBegin(); it != node->childrenEnd(); ++it ){
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

void SyntaxTreeModel::setSelected(csa::ast::CSANode *node){
    for ( int i = 0; i < m_items.size(); ++i ){
        SyntaxTreeItem* item = m_items[i];
        if ( item->node == node ){
            setSelected(i);
            return;
        }
    }
}
