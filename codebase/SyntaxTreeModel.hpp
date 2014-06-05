#ifndef SYNTAXTREEMODEL_HPP
#define SYNTAXTREEMODEL_HPP

#include <QAbstractListModel>
#include <QHash>

namespace csa{ namespace ast{
class CSANode;
}}

class CodeBase;
class SyntaxTreeItem;
class SyntaxTreeModel : public QAbstractListModel{

    Q_OBJECT
    Q_PROPERTY(int selected READ selected WRITE setSelected NOTIFY selectedChanged)

    enum Roles{
        Identifier = Qt::UserRole + 1,
        Type,
        Indent,
        Line
    };

public:
    explicit SyntaxTreeModel(QObject *parent = 0);

    void safeClear();
    void parse(csa::ast::CSANode* root);
    QVariant data(const QModelIndex &index, int role) const;
    int  rowCount(const QModelIndex &) const;
    virtual QHash<int, QByteArray> roleNames() const;

    int selected() const;
    void setSelected(int selected);
    void setSelected(csa::ast::CSANode* node);

signals:
    void selectedChanged();

protected:

private:
    void recursiveParse(csa::ast::CSANode* node, int indent = 0);
    void clear();

    QHash<int, QByteArray> m_roles;
    QList<SyntaxTreeItem*> m_items;

    int       m_codebaseSelected;
};

inline int SyntaxTreeModel::rowCount(const QModelIndex &) const{
    return m_items.count();
}

inline QHash<int, QByteArray> SyntaxTreeModel::roleNames() const{
    return m_roles;
}

inline int SyntaxTreeModel::selected() const{
    return m_codebaseSelected;
}

inline void SyntaxTreeModel::setSelected(int selected){
    if ( m_codebaseSelected != selected ){
        m_codebaseSelected = selected;
        emit selectedChanged();
    }
}

#endif // SYNTAXTREEMODEL_HPP
