#include "QCSACompletionModel.hpp"
#include "QCSACompletionSet.hpp"
#include "QCSAConsole.hpp"

namespace csa{

class QCSACompletionModelPrivate{

public:
    QCSACompletionSet*         completion;
    QList<QCSACompletionItem*> filteredPlugins;

    QString lastFilter;

    QString completionPrefix;
};


QCSACompletionModel::QCSACompletionModel(QCSACompletionSet *completionSet, QObject *parent)
    : QAbstractListModel(parent)
    , d_ptr(new QCSACompletionModelPrivate)
{
    d_ptr->completion = completionSet;

    m_roles[QCSACompletionModel::Name]        = "name";
    m_roles[QCSACompletionModel::Usage]       = "usage";
    m_roles[QCSACompletionModel::Description] = "description";
}

QCSACompletionModel::~QCSACompletionModel(){
    delete d_ptr;
}

QVariant QCSACompletionModel::data(const QModelIndex& index, int role) const{
    const Q_D(QCSACompletionModel);
    if ( role == QCSACompletionModel::Name ){
        return d->filteredPlugins[index.row()]->name();
    } else if ( role == QCSACompletionModel::Usage ){
        return d->filteredPlugins[index.row()]->usage();
    } else if ( role == QCSACompletionModel::Description ){
        return d->filteredPlugins[index.row()]->description();
    }
    return QVariant();
}

int QCSACompletionModel::rowCount(const QModelIndex&) const{
    const Q_D(QCSACompletionModel);
    return d->filteredPlugins.size();
}

const QString &QCSACompletionModel::completionPrefix() const{
    const Q_D(QCSACompletionModel);
    return d->completionPrefix;
}

void QCSACompletionModel::filter(const QString& text){
    Q_D(QCSACompletionModel);

    QCSACompletionItem::Type completionType;
    QString context = d->completion->getCompletionContext(text, &completionType);
    if ( d->lastFilter != context && context != ""){
        d->lastFilter = context;
        d->completionPrefix = text.mid(0, text.length() - context.length());
        emit completionPrefixChanged();

        beginResetModel();

        d->filteredPlugins = d->completion->getCompletionItems(context, completionType);

        endResetModel();
    }
}

void QCSACompletionModel::registerPlugin(const QVariantMap &pluginData){
    Q_D(QCSACompletionModel);
    d->completion->registerPlugin(pluginData);
}

}// namespace
