#include "QASTInsertionTest.hpp"
#include "QTestHelpers.hpp"

#include <QJSEngine>
#include <QtTest/QtTest>
#include <QTemporaryFile>

#include "QCodeBase.hpp"
#include "QCSAPluginLoader.hpp"

using namespace csa;

Q_TEST_RUNNER_REGISTER(QASTInsertionTest);

QASTInsertionTest::QASTInsertionTest(QObject *parent)
    : QObject(parent)
{
}

QASTInsertionTest::~QASTInsertionTest(){
    delete m_pluginLoader;
    delete m_engine;
}

void QASTInsertionTest::initTestCase(){
    m_parserTestPath = QCoreApplication::applicationDirPath() + "/data/insertion/";
    m_engine         = new QJSEngine;
    m_pluginLoader   = new csa::QCSAPluginLoader(m_engine);
}

void QASTInsertionTest::definedLocationInsertionTest(){

    QTemporaryFile tfile("csa-test");
    if ( !tfile.open() ){
        QFAIL("Unable to create temporary file.");
        return;
    }

    QFile testData(m_parserTestPath + "insertion.test");
    if ( !testData.open(QIODevice::ReadOnly) ){
        QFAIL(qPrintable("Unable to read test data file: " + testData.fileName()));
        return;
    }

    tfile.write(testData.readAll());
    tfile.close();

    QSharedPointer<QCodeBase> cbase = helpers::createCodeBaseFromFile(tfile.fileName());
    m_pluginLoader->setContextObject("codeBase", cbase.data());

    int parserEngineCode = m_pluginLoader->loadPlugins(m_parserTestPath + "insertion.js");
    if ( parserEngineCode != 0 ){
        QFAIL("Unable to load parser plugin: " + parserEngineCode);
        return;
    }

    QJSValue result;
    m_pluginLoader->execute("insertAtLocation(3, 1);", result);

    if ( !result.isBool() ){
        QFAIL(qPrintable("Unexpected function 'insertAtLocation' result type."));
        return;
    } else if ( result.toBool() == false ){
        QFAIL(qPrintable("Call to function 'insertAtLocation' returned false."));
        return;
    }

    if ( !tfile.open() ){
        QFAIL("Unable to reopen temporary file for reading.");
        return;
    }

    QFile expectedFile(m_parserTestPath + "insertion.expect");
    if ( !expectedFile.open(QIODevice::ReadOnly ) ){
        QFAIL("Failed to open test result file.");
        return;
    }

    QCOMPARE(expectedFile.readAll(), tfile.readAll());
}
