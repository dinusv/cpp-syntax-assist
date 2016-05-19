#ifndef TESTSCENARIO_H
#define TESTSCENARIO_H

#include <QObject>

namespace csa{
class QCSATestScenario;
}

class QJSValue;
class TestScenario : public QObject{

    Q_OBJECT

public:
    explicit TestScenario(const QString &testcase, csa::QCSATestScenario* scenario, QObject *parent = 0);
    ~TestScenario();

private slots:
    void init();
    void runTest();
    void cleanup();

private:
    QString createErrorMessage(const QJSValue& error);

    csa::QCSATestScenario* m_scenario;
    QString m_testcase;

};

#endif // TESTSCENARIO_H
