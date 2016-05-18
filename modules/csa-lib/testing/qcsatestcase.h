#ifndef QCSATESTCASE_H
#define QCSATESTCASE_H

#include "qcsaglobal.h"
#include <QObject>
#include <QJSValue>

namespace csa{

class QCSATestScenario;
class QCSAEngine;

class Q_CSA_EXPORT QCSATestCase : public QObject{

    Q_OBJECT

public:
    typedef QList<QCSATestScenario*>::iterator Iterator;
    typedef QList<QCSATestScenario*>::const_iterator ConstIterator;

public:
    explicit QCSATestCase(const QString& name, QCSAEngine* scriptEngine, QObject *parent = 0);
    ~QCSATestCase();

    const QString& name() const;

    Iterator begin();
    ConstIterator begin() const;
    Iterator end();
    ConstIterator end() const;

public slots:
    void beforeScenario(const QJSValue& fn);
    void afterScenario(const QJSValue& fn);
    void scenario(const QString& name, const QJSValue& fn);

private:
    QJSValue m_beforeScenario;
    QJSValue m_afterScenario;
    QString  m_name;
    QList<QCSATestScenario*> m_scenarios;
    QCSAEngine* m_scriptEngine;

};

inline const QString &QCSATestCase::name() const{
    return m_name;
}

inline QCSATestCase::Iterator QCSATestCase::begin(){
    return m_scenarios.begin();
}

inline QCSATestCase::ConstIterator QCSATestCase::begin() const{
    return m_scenarios.begin();
}

inline QCSATestCase::Iterator QCSATestCase::end(){
    return m_scenarios.end();
}

inline QCSATestCase::ConstIterator QCSATestCase::end() const{
    return m_scenarios.end();
}

}// namespace

#endif // QCSATESTCASE_H
