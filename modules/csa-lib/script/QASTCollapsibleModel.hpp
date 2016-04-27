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


#ifndef QASTCOLLAPSIBLEMODEL_HPP
#define QASTCOLLAPSIBLEMODEL_HPP

#include <QAbstractListModel>
#include <QHash>

#include "QCSAGlobal.hpp"

namespace csa{

namespace ast{ class QASTNode; class QASTFile; }

class QCodebase;
}

class QSyntaxTreeItem;
class Q_CSA_EXPORT QASTCollapsibleModel : public QAbstractListModel{

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
    explicit QASTCollapsibleModel(const QList<csa::ast::QASTFile*>& files, QObject *parent = 0);

    QVariant data(const QModelIndex &index, int role) const;
    int  rowCount(const QModelIndex &) const;
    virtual QHash<int, QByteArray> roleNames() const;

    int  selected() const;
    void setSelected(int selected);

    int  expandLevel() const;
    void setExpandLevel(int arg);

public slots:
    void collapse(int index);
    void expand(int index);

    void addFile(csa::ast::QASTFile* file);
    void collapseFile(csa::ast::QASTFile* file);
    void reparseFile(csa::ast::QASTFile* file);
    void selectNode(csa::ast::QASTNode* node);

signals:
    void selectedChanged();
    void expandLevelChanged();

private:
    void recursiveParse(int index);
    void recursiveCollapse(int index);
    void clear();

    QHash<int, QByteArray>  m_roles;
    QList<QSyntaxTreeItem*> m_items;

    int m_codebaseSelected;
    int m_expandLevel;
};

inline int QASTCollapsibleModel::rowCount(const QModelIndex &) const{
    return m_items.count();
}

inline QHash<int, QByteArray> QASTCollapsibleModel::roleNames() const{
    return m_roles;
}

inline int QASTCollapsibleModel::selected() const{
    return m_codebaseSelected;
}

inline void QASTCollapsibleModel::setSelected(int selected){
    if ( m_codebaseSelected != selected ){
        m_codebaseSelected = selected;
        emit selectedChanged();
    }
}

inline int QASTCollapsibleModel::expandLevel() const{
    return m_expandLevel;
}

inline void QASTCollapsibleModel::setExpandLevel(int arg){
    if (m_expandLevel == arg)
        return;

    m_expandLevel = arg;
    emit expandLevelChanged();
}

#endif // QASTCOLLAPSIBLEMODEL_HPP
