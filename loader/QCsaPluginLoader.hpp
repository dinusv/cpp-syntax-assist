#ifndef QCSAPLUGINLOADER_HPP
#define QCSAPLUGINLOADER_HPP

#include <QString>

class QScriptEngine;

namespace csa{

class QCsaPluginLoader{

public:
    QCsaPluginLoader(const QString& path);
    ~QCsaPluginLoader();

    QScriptEngine* engine();

    int initializePlugins();
private:
    int loadPlugin(const QString& path);

    QString m_path;
    QScriptEngine* m_engine;
};

inline QScriptEngine* QCsaPluginLoader::engine(){
    return m_engine;
}

}// namespace

#endif // QCSAPLUGINLOADER_HPP
