#ifndef QCODEBASECONFIG_H
#define QCODEBASECONFIG_H

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

    const QStringList& sourceFilePatterns() const;
    const QStringList& headerFilePatterns() const;

    const QStringList& includeFiles() const;
    const QStringList& includeCode() const;

public slots:
    QJSValue require(const QString& config);
    void update(const QJSValue& val);
    void save(QString path = "");
    void include(QString code);
    void includeLine(QString line);
    void includeFile(QString file);

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
    QStringList listFromJsArray(const QJSValue& array);

    // members
    QJSEngine* m_engine;

    QString m_configPath;
    QString m_configFile;

    // configuration members
    int    m_translationUnitNumArgs;
    char** m_translationUnitArgs;

    QStringList m_sourceFilePatterns;
    QStringList m_headerFilePatterns;

    QStringList m_includeFiles;
    QString m_includeCode;
};

inline int QCodebaseConfig::translationUnitNumArgs() const{
    return m_translationUnitNumArgs;
}

inline char** QCodebaseConfig::translationUnitArgs(){
    return m_translationUnitArgs;
}

inline const QStringList &QCodebaseConfig::sourceFilePatterns() const{
    return m_sourceFilePatterns;
}

inline const QStringList &QCodebaseConfig::headerFilePatterns() const{
    return m_headerFilePatterns;
}

inline void QCodebaseConfig::include(QString code){
    m_includeCode += code;
    emit dataChanged();
}

inline void QCodebaseConfig::includeLine(QString line){
    m_includeCode += line + "\n";
    emit dataChanged();
}

inline void QCodebaseConfig::includeFile(QString file){
    m_includeFiles << file;
    emit dataChanged();
}

inline const QStringList &QCodebaseConfig::includeFiles() const{
    return m_includeFiles;
}

}// namespace

#endif // QCODEBASECONFIG_H
