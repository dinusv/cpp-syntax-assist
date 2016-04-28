#ifndef QCODEBASECONFIG_HPP
#define QCODEBASECONFIG_HPP

#include <QObject>
#include <QJSValue>
#include "qcsaglobal.h"

namespace csa{

class Q_CSA_EXPORT QCodebaseConfig : public QObject{

    Q_OBJECT

public:
    explicit QCodebaseConfig(
            QJSEngine* engine,
            const QString& configPath,
            const QString& configName = "default",
            QObject *parent = 0
    );
    ~QCodebaseConfig();

    int translationUnitNumArgs() const;
    char** translationUnitArgs();

public slots:
    QJSValue require(const QString& config);
    void update(const QJSValue& val);
    void save(QString path = "");
//     void includeCode(QString code);

signals:
    void dataChanged();

private:
    // prevent copy
    QCodebaseConfig(const QCodebaseConfig& other);
    QCodebaseConfig& operator=(const QCodebaseConfig& other);

    // helpers
    QJsonObject serializeData();
    QJSValue requireFile(const QString& path);
    void parseTranslationUnitArgs(const QJSValue& val);
    void freeTranslationUnitArgs();

    // members
    QJSEngine* m_engine;

    QString m_configPath;
    QString m_configFile;

    int    m_translationUnitNumArgs;
    char** m_translationUnitArgs;
};

inline int QCodebaseConfig::translationUnitNumArgs() const{
    return m_translationUnitNumArgs;
}

inline char** QCodebaseConfig::translationUnitArgs(){
    return m_translationUnitArgs;
}

}// namespace

#endif // QCODEBASECONFIG_HPP
