#include "qcsatestingtest.h"
#include "qcsatest.h"
#include "qcsatestcase.h"
#include "qcsatestscenario.h"

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
    assertionTest.loadTestFile(QCoreApplication::applicationDirPath() + "/testing/csa-assertions-test.js");
    QCSATest::Iterator it = assertionTest.begin();
    QVERIFY(it != assertionTest.end());
    QVERIFY(it + 1 == assertionTest.end());

    QCSATestCase* testCase = *it;
    QVERIFY(testCase->name() == "assertions");

    QCSATestScenario* successAssertion = testCase->findScenario("successful assertion");
    QVERIFY(successAssertion != 0);
    QJSValue result = successAssertion->runTest();
    QVERIFY(!result.isError());

    QCSATestScenario* failedAssertion = testCase->findScenario("failed assertion");
    QVERIFY(failedAssertion != 0);
    result = failedAssertion->runTest();
    QVERIFY(result.isError());

    QString message, file, lineText; int line;
    QCSATestScenario::stripError(result, message, file, line, lineText);
    QVERIFY(lineText == "assert(false)");

    QCSATestScenario* successfulException = testCase->findScenario("successful exception assertion");
    QVERIFY(successfulException != 0);
    result = successfulException->runTest();
    QVERIFY(!result.isError());

    QCSATestScenario* failedException = testCase->findScenario("failed exception assertion");
    QVERIFY(failedException != 0);
    result = failedException->runTest();
    QVERIFY(result.isError());
    QCSATestScenario::stripError(result, message, file, line, lineText);
    QVERIFY(lineText == "assertThrows(function(){ return 42; }, Error)");

    QCSATestScenario* successfulCustomException = testCase->findScenario("successful custom exception assertion");
    QVERIFY(successfulCustomException != 0);
    result = successfulCustomException->runTest();
    QVERIFY(!result.isError());

    QCSATestScenario* failedCustomException = testCase->findScenario("failed custom exception assertion");
    QVERIFY(failedCustomException != 0);
    result = failedCustomException->runTest();
    QVERIFY(result.isError());
    QCSATestScenario::stripError(result, message, file, line, lineText);
    QVERIFY(lineText == "assertThrows(function(){ throw new Error(); }, CustomError)");
}

void QCSATestingTest::beforeAfterScenarioTest(){
    QCSATest beforeAfterTest;
    beforeAfterTest.loadTestFile(QCoreApplication::applicationDirPath() + "/testing/csa-before-after-scenario-test.js");

    QCSATest::Iterator it = beforeAfterTest.begin();
    QVERIFY(it != beforeAfterTest.end());
    QCSATestCase* failedBeforeTestCase = *it;
    QVERIFY(failedBeforeTestCase->name() == "failed before scenario");

    QCSATestScenario* scenario1 = failedBeforeTestCase->findScenario("this shouldn\'t get called");
    QVERIFY(scenario1 != 0);
    QJSValue result = scenario1->init();
    QVERIFY(result.isError());
    QVERIFY(scenario1->hasError());
    QJSValue testResult = scenario1->runTest();
    QVERIFY(testResult.isError());
    QVERIFY(testResult.property("stack").toString() == result.property("stack").toString());
    QJSValue cleanupResult = scenario1->cleanup();
    QVERIFY(cleanupResult.property("stack").toString() == result.property("stack").toString());


    QCSATestScenario* scenario2 = failedBeforeTestCase->findScenario("this shouldn\'t get called either");
    QVERIFY(scenario2 != 0);
    result = scenario2->init();
    QVERIFY(result.isError());
    QVERIFY(scenario2->hasError());
    testResult = scenario2->runTest();
    QVERIFY(testResult.isError());
    QVERIFY(testResult.property("stack").toString() == result.property("stack").toString());
    cleanupResult = scenario2->cleanup();
    QVERIFY(cleanupResult.property("stack").toString() == result.property("stack").toString());

    ++it;
    QVERIFY(it != beforeAfterTest.end());
    QVERIFY(it + 1 == beforeAfterTest.end());
    QCSATestCase* failedAfterTestCase = *it;
    QVERIFY(failedAfterTestCase->name() == "failed after scenario");

    scenario1 = failedAfterTestCase->findScenario("scenario 1");
    QVERIFY(scenario1 != 0);
    result = scenario1->init();
    QVERIFY(!result.isError());
    testResult = scenario1->runTest();
    QVERIFY(!testResult.isError());
    cleanupResult = scenario1->cleanup();
    QVERIFY(cleanupResult.isError());
    QString message, file, lineText; int line;
    QCSATestScenario::stripError(cleanupResult, message, file, line, lineText);
    QVERIFY(lineText == "throw new Error('Failed to clean test case for scenarios')");

    scenario2 = failedAfterTestCase->findScenario("scenario 2");
    QVERIFY(scenario2 != 0);
    result = scenario2->init();
    testResult = scenario2->runTest();
    cleanupResult = scenario2->cleanup();
    QCSATestScenario::stripError(cleanupResult, message, file, line, lineText);
    QVERIFY(lineText == "throw new Error('Failed to clean test case for scenarios')");
}

void QCSATestingTest::invalidParamsTest(){
    QCSATest invalidParamsTest;
    invalidParamsTest.loadTestFile(QCoreApplication::applicationDirPath() + "/testing/csa-invalid-params-test.js");

    QCSATestCase* invalidBeforeTestCase = invalidParamsTest.findTestCase("invalid before scenario");
    QVERIFY(invalidBeforeTestCase != 0);

    QCSATestScenario* scenario = invalidBeforeTestCase->findScenario("this shouldn\'t get called");
    QVERIFY(scenario != 0);
    QJSValue initResult = scenario->init();
    QVERIFY(initResult.isError());
    QJSValue testResult = scenario->runTest();
    QVERIFY(testResult.isError());
    QJSValue cleanupResult = scenario->cleanup();
    QVERIFY(cleanupResult.isError());

    QCSATestCase* invalidAfterTestCase = invalidParamsTest.findTestCase("invalid after scenario");
    QVERIFY(invalidAfterTestCase != 0);

    scenario = invalidAfterTestCase->findScenario("scenario");
    QVERIFY(scenario != 0);
    initResult = scenario->init();
    QVERIFY(initResult.isError());
    testResult = scenario->runTest();
    QVERIFY(testResult.isError());
    cleanupResult = scenario->cleanup();
    QVERIFY(cleanupResult.isError());

    QCSATestCase* invalidDescriptionTestCase = invalidParamsTest.findTestCase("invalid description");
    QVERIFY(invalidDescriptionTestCase != 0);

    scenario = invalidDescriptionTestCase->findScenario("uncallable");
    QVERIFY(scenario != 0);
    initResult = scenario->init();
    QVERIFY(initResult.isError());
    testResult = scenario->runTest();
    QVERIFY(testResult.isError());
    cleanupResult = scenario->cleanup();
    QVERIFY(cleanupResult.isError());
}

void QCSATestingTest::restoreTest(){
    QCSATest restoreTest;
    restoreTest.loadTestFile(QCoreApplication::applicationDirPath() + "/testing/csa-restore-test.js");

    QCSATestCase* restoreTestCase = restoreTest.findTestCase("restore");
    QVERIFY(restoreTestCase != 0);

    QCSATestScenario* writeScenario = restoreTestCase->findScenario("write to file");
    QVERIFY(writeScenario != 0);
    QCSATestScenario* removeScenario = restoreTestCase->findScenario("delete file");
    QVERIFY(removeScenario != 0);

    QFile inputFile(QCoreApplication::applicationDirPath() + "/testing/csa-restore-test.in");
    if ( !inputFile.open(QFile::ReadOnly) )
        QFAIL(qPrintable("Failed to open input file: " + inputFile.fileName()));
    QTextStream inputStream(&inputFile);
    QString inputContents = inputStream.readAll();
    inputFile.close();;

    QVERIFY(!writeScenario->init().isError());
    QVERIFY(!writeScenario->runTest().isError());
    QString expectedInputContents = "class ToRemove{};\n" + inputContents;

    if ( !inputFile.open(QFile::ReadOnly) )
        QFAIL(qPrintable("Failed to open input file: " + inputFile.fileName()));
    inputStream.setDevice(&inputFile);
    QString newInputContents = inputStream.readAll();
    inputFile.close();
    QCOMPARE(newInputContents, expectedInputContents);
    QVERIFY(!writeScenario->cleanup().isError());

    if ( !inputFile.open(QFile::ReadOnly) )
        QFAIL(qPrintable("Failed to open input file: " + inputFile.fileName()));
    inputStream.setDevice(&inputFile);
    newInputContents = inputStream.readAll();
    inputFile.close();
    QCOMPARE(newInputContents, inputContents);

    QVERIFY(!removeScenario->init().isError());
    QVERIFY(!removeScenario->runTest().isError());
    QVERIFY(!QFileInfo(inputFile).exists());
    QVERIFY(!removeScenario->cleanup().isError());
    QVERIFY(QFileInfo(inputFile).exists());
}
