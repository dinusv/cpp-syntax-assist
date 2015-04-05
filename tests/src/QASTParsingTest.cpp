/****************************************************************************
**
** Copyright (C) 2014-2015 Dinu SV.
** (contact: mail@dinusv.com)
** This file is part of C++ Snippet Assist application.
**
** GNU General Public License Usage
** 
** This file may be used under the terms of the GNU General Public License 
** version 3.0 as published by the Free Software Foundation and appearing 
** in the file LICENSE.GPL included in the packaging of this file.  Please 
** review the following information to ensure the GNU General Public License 
** version 3.0 requirements will be met: http://www.gnu.org/copyleft/gpl.html.
**
****************************************************************************/


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

    bool parseOk;
    QJsonObject expectedResult = helpers::parseJsonFile(m_parserTestPath + "unknowntype.expect", &parseOk);
    if ( !parseOk ){
        QFAIL(qPrintable("Unable to parse result file: " + m_parserTestPath + "unknowntype.exepect"));
        return;
    }

    QVERIFY(helpers::compareJsonValues(jsonResult.toObject(), expectedResult));
}

void QASTParsingTest::init(){
    if ( m_engine == 0 ){
        m_engine = new csa::QCSAScriptEngine;
        m_parserTestPath = QCoreApplication::applicationDirPath() + "/data/parsing/";
        m_parserEngineCode = m_engine->loadPlugins(m_parserTestPath + "parserplugin.js");
    }
}
