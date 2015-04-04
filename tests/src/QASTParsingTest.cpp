#include "QASTParsingTest.hpp"
#include "QTestHelpers.hpp"

#include <QtTest/QtTest>

#include "QCodeBase.hpp"
#include "QCSAScriptEngine.hpp"

using namespace csa;

Q_TEST_RUNNER_REGISTER(QASTParsingTest);

QASTParsingTest::QASTParsingTest(QObject *parent)
    : QObject(parent)
    , m_engine(0)
{
}

QASTParsingTest::~QASTParsingTest(){
    delete m_engine;
}

void QASTParsingTest::unknownTypeDeduction(){
    init();

    if ( m_parserEngineCode != 0 ){
        QFAIL("Unable to load parser plugin: " + m_parserEngineCode);
        return;
    }

    const char* args[] = {"-c", "-x", "c++"};
    QCodeBase cbase(args, 3, m_parserTestPath + "unknowntype.test");

    m_engine->setCodeBase(&cbase);

    QScriptValue result;
    m_engine->execute("parserplugin();", result);

    QJsonValue jsonResult = helpers::jsonFromScriptValue(result);

    QFile file(m_parserTestPath + "unknowntype.expect");
    if ( file.open(QIODevice::ReadOnly) ){
        QByteArray bstr = file.readAll();
        QJsonDocument expectedResult = QJsonDocument::fromJson(bstr);

        QJsonObject er = expectedResult.object();
        QJsonObject gr = jsonResult.toObject();

        QVERIFY(helpers::compareJsonValues(er, gr));
    }
}

void QASTParsingTest::init(){
    if ( m_engine == 0 ){
        m_engine = new csa::QCSAScriptEngine;
        m_parserTestPath = QCoreApplication::applicationDirPath() + "/data/parsing/";
        m_parserEngineCode = m_engine->loadPlugins(m_parserTestPath + "parserplugin.js");
    }
}
