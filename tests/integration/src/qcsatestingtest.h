#ifndef QCSATESTINGTEST_H
#define QCSATESTINGTEST_H

#include <QObject>
#include "qtestrunner.h"

class QCSATestingTest : public QObject{

    Q_OBJECT
    Q_TEST_RUNNER_SUITE

public:
    explicit QCSATestingTest(QObject *parent = 0);
    ~QCSATestingTest();

private slots:
    void assertionsTest();
    void beforeAfterScenarioTest();
    void invalidParamsTest();
    void restoreTest();

};

#endif // QCSATESTINGTEST_H
