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


#ifndef SYNTAXTREEMODEL_HPP
#define SYNTAXTREEMODEL_HPP

#include <QAbstractListModel>
#include <QHash>

#include "QCodeBaseObserver.hpp"

namespace csa{

namespace ast{ class QASTNode; class QASTFile; }

class QCodeBase;
}

class QSyntaxTreeItem;
class QSyntaxTreeModel : public QAbstractListModel, public csa::QCodeBaseObserver{

    Q_OBJECT
    Q_PROPERTY(int selected    READ selected    WRITE setSelected    NOTIFY selectedChanged)
    Q_PROPERTY(int expandLevel READ expandLevel WRITE setExpandLevel NOTIFY expandLevelChanged)

    enum Roles{
        Identifier = Qt::UserRole + 1,
        Type,
        Indent,
        Line,
        IsCollapsible,
        IsCollapsed,
        Breadcrumbs
    };

public:
    explicit QSyntaxTreeModel(QObject *parent = 0);

    void clearAndReset();
    void parse(const QList<csa::ast::QASTFile*>& files);
    QVariant data(const QModelIndex &index, int role) const;
    int  rowCount(const QModelIndex &) const;
    virtual QHash<int, QByteArray> roleNames() const;

    int  selected() const;
    void setSelected(int selected);
    void setSelected(csa::ast::QASTNode* node);

    int  expandLevel() const;
    void setExpandLevel(int arg);

public slots:
    void collapse(int index);
    void expand(int index);

signals:
    void selectedChanged();
    void expandLevelChanged();

private:
    void recursiveParse(csa::ast::QASTNode* node, int indent = 0);
    void recursiveCollapse(int index);
    void clear();

    QHash<int, QByteArray>  m_roles;
    QList<QSyntaxTreeItem*> m_items;

    int m_codebaseSelected;
    int m_expandLevel;
};

inline int QSyntaxTreeModel::rowCount(const QModelIndex &) const{
    return m_items.count();
}

inline QHash<int, QByteArray> QSyntaxTreeModel::roleNames() const{
    return m_roles;
}

inline int QSyntaxTreeModel::selected() const{
    return m_codebaseSelected;
}

inline void QSyntaxTreeModel::setSelected(int selected){
    if ( m_codebaseSelected != selected ){
        m_codebaseSelected = selected;
        emit selectedChanged();
    }
}

inline int QSyntaxTreeModel::expandLevel() const{
    return m_expandLevel;
}

inline void QSyntaxTreeModel::setExpandLevel(int arg){
    if (m_expandLevel == arg)
        return;

    m_expandLevel = arg;
    emit expandLevelChanged();
}

#endif // SYNTAXTREEMODEL_HPP
