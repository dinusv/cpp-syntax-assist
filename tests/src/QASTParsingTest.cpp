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
#include <QtGui/QGuiApplication>
#include "QTestHelpers.hpp"

#include <QtTest/QtTest>
#include <QJSEngine>
#include <QJSValue>

#include <qqml.h>
#include "QCodeBase.hpp"
#include "QCSAPluginLoader.hpp"

using namespace csa;

Q_TEST_RUNNER_REGISTER(QASTParsingTest);

QASTParsingTest::QASTParsingTest(QObject *parent)
    : QObject(parent)
    , m_loader(0)
{
}

QASTParsingTest::~QASTParsingTest(){
    delete m_loader;
    delete m_engine;
}

void QASTParsingTest::initTestCase(){
    m_engine             = new QJSEngine;
    m_loader             = new csa::QCSAPluginLoader(m_engine);
    m_parserTestPath     = QCoreApplication::applicationDirPath() + "/data/parsing/";
    int parserEngineCode = m_loader->loadPlugins(m_parserTestPath + "parserplugin.js");

    if ( parserEngineCode != 0 ){
        QFAIL("Unable to load parser plugin: " + parserEngineCode);
        return;
    }
}

void QASTParsingTest::unknownTypeDeductionTest(){
    QSharedPointer<QCodeBase> cbase = helpers::createCodeBaseFromFile(m_parserTestPath + "unknowntype.test");

//    m_engine->globalObject().setProperty("codeBase", m_engine->newQObject(cbase.data()));
    m_engine->globalObject().setProperty("codeBase", m_engine->newQObject(cbase.data()));//m_engine->newQObject(cbase.data()));

    QJSValue result;
    m_loader->execute("parserplugin();", result);
    QJsonValue jsonResult = helpers::jsonFromScriptValue(result);

    bool parseOk;
    QJsonObject expectedResult = helpers::parseJsonFile(m_parserTestPath + "unknowntype.expect", &parseOk);
    if ( !parseOk ){
        QFAIL(qPrintable("Unable to parse result file: " + m_parserTestPath + "unknowntype.exepect"));
        return;
    }

    QVERIFY(helpers::compareJsonValues(jsonResult.toObject(), expectedResult));
}
