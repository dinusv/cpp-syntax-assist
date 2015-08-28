#include "QCSAPluginCollection.hpp"
#include "QCSAConsole.hpp"

namespace csa{

class QCSAPluginCollectionItem{

public:
    QString name;
    QString usage;
    QString description;
};

class QCSAPluginCollectionPrivate{

public:
    QList<QCSAPluginCollectionItem*> registeredPlugins;
    QList<QCSAPluginCollectionItem*> filteredPlugins;

    QString lastFilter;
};


QCSAPluginCollection::QCSAPluginCollection(QObject *parent)
    : QAbstractListModel(parent)
    , d_ptr(new QCSAPluginCollectionPrivate)
{
    m_roles[QCSAPluginCollection::Name]        = "name";
    m_roles[QCSAPluginCollection::Usage]       = "usage";
    m_roles[QCSAPluginCollection::Description] = "description";
}

QCSAPluginCollection::~QCSAPluginCollection(){
    delete d_ptr;
}

QVariant QCSAPluginCollection::data(const QModelIndex& index, int role) const{
    const Q_D(QCSAPluginCollection);
    if ( role == QCSAPluginCollection::Name ){
        return d->filteredPlugins[index.row()]->name;
    } else if ( role == QCSAPluginCollection::Usage ){
        return d->filteredPlugins[index.row()]->usage;
    } else if ( role == QCSAPluginCollection::Description ){
        return d->filteredPlugins[index.row()]->description;
    }
    return QVariant();
}

int QCSAPluginCollection::rowCount(const QModelIndex&) const{
    const Q_D(QCSAPluginCollection);
    return d->filteredPlugins.size();
}

int QCSAPluginCollection::totalRows() const{
    const Q_D(QCSAPluginCollection);
    return d->filteredPlugins.size();
}

const QString& QCSAPluginCollection::name(int index) const{
    const Q_D(QCSAPluginCollection);
    return d->filteredPlugins[index]->name;
}

const QString& QCSAPluginCollection::usage(int index) const{
    const Q_D(QCSAPluginCollection);
    return d->filteredPlugins[index]->usage;
}

void QCSAPluginCollection::registerPlugin(const QVariantMap& pluginData){
    Q_D(QCSAPluginCollection);
    if ( !pluginData.contains("name") ){
        QCSAConsole::logError("Cannot find \'name\' property in given arguments.");
        return;
    }

    QCSAPluginCollectionItem* newItem = new QCSAPluginCollectionItem;
    newItem->name        = pluginData["name"].toString();
    newItem->usage       = pluginData.contains("usage") ? pluginData["usage"].toString() : "";
    newItem->description = pluginData.contains("description") ? pluginData["description"].toString() : "";

    int position = 0;
    QList<QCSAPluginCollectionItem*>::iterator it = d->registeredPlugins.begin();
    while( it != d->registeredPlugins.end() ){
        QCSAPluginCollectionItem* current = *it;
        if ( newItem->name < current->name ){
            d->registeredPlugins.insert(position, newItem);
            break;
        }
        ++it;
        ++position;
    }
    if ( it == d->registeredPlugins.end() )
        d->registeredPlugins.append(newItem);

    filter(d->lastFilter);
}

void QCSAPluginCollection::filter(const QString& name){
    Q_D(QCSAPluginCollection);
    if ( name == "" ){
        d->filteredPlugins = d->registeredPlugins;
        d->lastFilter = name;
    }

    QString lowerName = name.toLower();

    if ( d->lastFilter == lowerName )
        return;

    d->lastFilter = lowerName;
    beginResetModel();

    d->filteredPlugins.clear();

    for (
         QList<QCSAPluginCollectionItem*>::iterator it = d->registeredPlugins.begin();
         it != d->registeredPlugins.end(); ++it
    ){
        QCSAPluginCollectionItem* current = *it;
        if ( current->name.toLower().startsWith(lowerName) ){
            d->filteredPlugins.append(current);
        }
    }

    endResetModel();
}

}// namespace
