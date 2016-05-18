#include "qcsatestingtest.h"
#include "qcsatest.h"

#include <QtTest/QtTest>

using namespace csa;

Q_TEST_RUNNER_REGISTER(QCSATestingTest);

QCSATestingTest::QCSATestingTest(QObject *parent)
    : QObject(parent)
{

}

QCSATestingTest::~QCSATestingTest(){

}

void QCSATestingTest::assertionsTest(){
    QCSATest assertionTest;
    assertionTest.loadTestFile(QCoreApplication::applicationDirPath() + "/core/csa-assertions-test.js");

}

void QCSATestingTest::beforeAfterScenarioTest(){
    QCSATest assertionTest;
    assertionTest.loadTestFile(QCoreApplication::applicationDirPath() + "/core/csa-before-after-scenario-test.js");

}

void QCSATestingTest::invalidParamsTest(){
    QCSATest assertionTest;
    assertionTest.loadTestFile(QCoreApplication::applicationDirPath() + "/core/csa-invalid-params-test.js");

}

void QCSATestingTest::restoreTest(){
    QCSATest assertionTest;
    assertionTest.loadTestFile(QCoreApplication::applicationDirPath() + "/core/csa-restore-test.js");
}
