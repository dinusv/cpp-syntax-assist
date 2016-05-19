#ifndef QCSATESTSCENARIO_H
#define QCSATESTSCENARIO_H

#include "qcsaglobal.h"
#include <QObject>
#include <QJSValue>

namespace csa{

class QCodebase;
class QCSAEngine;

class Q_CSA_EXPORT QCSATestScenario : public QObject{

    Q_OBJECT

public:
    explicit QCSATestScenario(
        const QString& name,
        const QJSValue& fn,
        QCSAEngine* scriptEngine,
        QObject *parent = 0);
    virtual ~QCSATestScenario();

    void setBeforeCall(const QJSValue& fn);
    void setAfterCall(const QJSValue& fn);

    const QString& name() const;

    QJSValue init();
    QJSValue runTest();
    QJSValue cleanup();

    static void stripError(const QJSValue& error, QString& message, QString& file, int& line, QString &lineText);

    bool hasError();
    const QJSValue& lastError() const;

private:
    QString  m_name;
    QJSValue m_before;
    QJSValue m_after;
    QJSValue m_test;

    QJSValue m_lastError;

    QCSAEngine* m_scriptEngine;
    QCodebase* m_codebase;

};

inline const QString &QCSATestScenario::name() const{
    return m_name;
}

inline bool QCSATestScenario::hasError(){
    return m_lastError.isError();
}

inline const QJSValue &QCSATestScenario::lastError() const{
    return m_lastError;
}

}// namespace

#endif // QASTTESTSCENARIO_H
