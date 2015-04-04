#ifndef QCSAPLUGINLOADER_HPP
#define QCSAPLUGINLOADER_HPP

#include "QCSAGlobal.hpp"
#include <QString>
#include <QObject>

class QScriptEngine;
class QScriptValue;

namespace csa{

class QCodeBase;
class Q_CSA_EXPORT QCSAScriptEngine : public QObject{

    Q_OBJECT

public:
    QCSAScriptEngine(QObject* parent = 0);
    ~QCSAScriptEngine();

    void setCodeBase(QCodeBase* base);

    QScriptEngine* engine();
    int loadPlugins(const QString& path);

    bool execute(const QString &jsCode, QScriptValue& result);

public slots:
    bool execute(const QString& jsCode);

private:
    QScriptEngine* m_engine;
};

inline QScriptEngine* QCSAScriptEngine::engine(){
    return m_engine;
}

}// namespace

#endif // QCSAPLUGINLOADER_HPP
