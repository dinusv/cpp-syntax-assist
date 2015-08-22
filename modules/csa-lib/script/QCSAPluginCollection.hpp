#ifndef QCSAPLUGINCOLLECTION_HPP
#define QCSAPLUGINCOLLECTION_HPP

#include <QAbstractListModel>
#include "QCSAGlobal.hpp"

namespace csa{

class QCSAPluginCollectionPrivate;
class Q_CSA_EXPORT QCSAPluginCollection : public QAbstractListModel{

    Q_OBJECT

public:
    enum Roles{
        Name = Qt::UserRole + 1,
        Usage,
        Description
    };

    explicit QCSAPluginCollection(QObject *parent = 0);
    ~QCSAPluginCollection();

    QVariant data(const QModelIndex &index, int role) const;
    int  rowCount(const QModelIndex &) const;
    virtual QHash<int, QByteArray> roleNames() const;

signals:

public slots:
    void registerPlugin(const QVariantMap& pluginData);
    void filter(const QString& name);

private:
    QCSAPluginCollection(const QCSAPluginCollection& other);
    QCSAPluginCollection& operator=(const QCSAPluginCollection& other);

    QCSAPluginCollectionPrivate* const d_ptr;
    Q_DECLARE_PRIVATE(QCSAPluginCollection)

    QHash<int, QByteArray> m_roles;
};

inline QHash<int, QByteArray> QCSAPluginCollection::roleNames() const{
    return m_roles;
}

}// namespace

#endif // QCSAPLUGINCOLLECTION_HPP
