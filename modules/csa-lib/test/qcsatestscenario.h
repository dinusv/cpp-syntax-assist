#ifndef QCSATESTSCENARIO_H
#define QCSATESTSCENARIO_H

#include <QObject>
#include <QJSValue>
#include "QCSAGlobal.hpp"

namespace csa{

class QCodebase;
class QCSAPluginLoader;

class Q_CSA_EXPORT QCSATestScenario : public QObject{

    Q_OBJECT

public:
    explicit QCSATestScenario(
        const QString& name,
        const QJSValue& fn,
        QCSAPluginLoader* scriptEngine,
        QObject *parent = 0);
    virtual ~QCSATestScenario();

    void setBeforeCall(const QJSValue& fn);
    void setAfterCall(const QJSValue& fn);

    const QString& name() const;

    QJSValue init();
    QJSValue runTest();
    QJSValue cleanup();

private:
    QString  m_name;
    QJSValue m_before;
    QJSValue m_after;
    QJSValue m_test;

    QCSAPluginLoader* m_scriptEngine;
    QCodebase* m_codebase;

};

inline const QString &QCSATestScenario::name() const{
    return m_name;
}

}// namespace

#endif // QASTTESTSCENARIO_H
