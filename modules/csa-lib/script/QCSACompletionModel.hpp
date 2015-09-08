#ifndef QCSACOMPLETIONMODEL_HPP
#define QCSACOMPLETIONMODEL_HPP

#include <QAbstractListModel>
#include "QCSAGlobal.hpp"

namespace csa{

class QCSACompletionSet;

class QCSACompletionModelPrivate;
class Q_CSA_EXPORT QCSACompletionModel : public QAbstractListModel{

    Q_OBJECT
    Q_PROPERTY(QString completionPrefix READ completionPrefix NOTIFY completionPrefixChanged)

public:
    enum Roles{
        Name = Qt::UserRole + 1,
        Usage,
        Description
    };

    explicit QCSACompletionModel(QCSACompletionSet* completionSet, QObject *parent = 0);
    ~QCSACompletionModel();

    QVariant data(const QModelIndex &index, int role) const;
    int  rowCount(const QModelIndex &) const;
    virtual QHash<int, QByteArray> roleNames() const;

    const QString& completionPrefix() const;

signals:
    void completionPrefixChanged();

public slots:
    void filter(const QString& name);
    void registerPlugin(const QVariantMap& pluginData);

private:
    QCSACompletionModel(const QCSACompletionModel& other);
    QCSACompletionModel& operator=(const QCSACompletionModel& other);

    QCSACompletionModelPrivate* const d_ptr;
    Q_DECLARE_PRIVATE(QCSACompletionModel)

    QHash<int, QByteArray> m_roles;
};

inline QHash<int, QByteArray> QCSACompletionModel::roleNames() const{
    return m_roles;
}

}// namespace

#endif // QCSACOMPLETIONMODEL_HPP
