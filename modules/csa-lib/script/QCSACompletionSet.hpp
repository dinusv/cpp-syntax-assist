#ifndef QCSACOMPLETIONSET_HPP
#define QCSACOMPLETIONSET_HPP

#include <QObject>
#include <QCSAGlobal.hpp>

namespace csa{

// QCSACompletionItem
// ------------------

class Q_CSA_EXPORT QCSACompletionItem{

public:
    enum Type{
        Object = 0x01,
        Global = 0x02
    };

    QCSACompletionItem(int type, QString name, QString usage, QString description);
    ~QCSACompletionItem();

    int type() const;
    const QString& name() const;
    const QString& usage() const;
    const QString& description() const;

private:
    int     m_type;
    QString m_name;
    QString m_usage;
    QString m_description;
};


inline int QCSACompletionItem::type() const{
    return m_type;
}

inline const QString &QCSACompletionItem::name() const{
    return m_name;
}

inline const QString &QCSACompletionItem::usage() const{
    return m_usage;
}

inline const QString &QCSACompletionItem::description() const{
    return m_description;
}

// QCSACompletionSet
// -----------------

class Q_CSA_EXPORT QCSACompletionSet : public QObject{

    Q_OBJECT

public:
    QCSACompletionSet(QObject* parent = 0);
    ~QCSACompletionSet();

    int totalItems();
    virtual void registerItem(QCSACompletionItem* item);
    void registerItem(int type, const QString& name, const QString& usage, const QString& description);

    QString getDescription(const QString& name, QCSACompletionItem::Type type);

    QString getCompletionContext(const QString& text, QCSACompletionItem::Type* type) const;
    QList<QCSACompletionItem*> getCompletionItems(const QString& context, QCSACompletionItem::Type type);

    void initDefaultCompletions();

public slots:
    void registerPlugin(const QVariantMap& pluginData);

private:
    QList<QCSACompletionItem*> m_items;
};

inline int QCSACompletionSet::totalItems(){
    return m_items.size();
}

}// namespace

#endif // QCSACOMPLETIONSET_HPP

