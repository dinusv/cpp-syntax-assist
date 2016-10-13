#ifndef QCODEBASECONFIG_H
#define QCODEBASECONFIG_H

#include <QObject>
#include <QJSValue>
#include <QString>
#include <QStringList>
#include <QTemporaryFile>
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

    void translationUnitArgsUpdate() const;

    const QStringList& sourceFilePatterns() const;
    const QStringList& headerFilePatterns() const;

    const QStringList& includeFiles() const;
    const QString& includeCode() const;

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
    void freeTranslationUnitArgs() const;
    QStringList listFromJsArray(const QJSValue& array);

    // members
    QJSEngine* m_engine;

    QString m_configPath;
    QString m_configFile;

    // configuration members
    mutable int    m_translationUnitNumArgs;
    mutable char** m_translationUnitFullArgs;
    mutable bool m_translationUnitArgsDirty;

    QStringList m_translationUnitArgs;

    QStringList m_sourceFilePatterns;
    QStringList m_headerFilePatterns;

    //QStringList m_translationUnitArgs;

    QStringList m_includeFiles;
    QString     m_includeCode;
    mutable bool m_includeCodeDirty;

    mutable QTemporaryFile m_includeFile;
};

inline int QCodebaseConfig::translationUnitNumArgs() const{
    translationUnitArgsUpdate();
    return m_translationUnitNumArgs;
}

inline char** QCodebaseConfig::translationUnitArgs(){
    translationUnitArgsUpdate();
    return m_translationUnitFullArgs;
}

inline const QStringList &QCodebaseConfig::sourceFilePatterns() const{
    return m_sourceFilePatterns;
}

inline const QStringList &QCodebaseConfig::headerFilePatterns() const{
    return m_headerFilePatterns;
}

inline void QCodebaseConfig::include(QString code){
    m_includeCode += code;
    m_includeCodeDirty = true;
    emit dataChanged();
}

inline void QCodebaseConfig::includeLine(QString line){
    m_includeCode += line + "\n";
    m_includeCodeDirty = true;
    emit dataChanged();
}

inline void QCodebaseConfig::includeFile(QString file){
    m_includeFiles << file;
    emit dataChanged();
}

inline const QStringList &QCodebaseConfig::includeFiles() const{
    return m_includeFiles;
}

inline const QString &QCodebaseConfig::includeCode() const{
    return m_includeCode;
}

}// namespace

#endif // QCODEBASECONFIG_H
