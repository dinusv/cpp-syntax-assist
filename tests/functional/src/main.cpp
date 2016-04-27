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


#include <QCoreApplication>
#include <QTest>
#include "QASTSearchTest.hpp"

#include <qqml.h>
#include "QCodeBase.hpp"
#include "QASTFile.hpp"
#include "QASTNode.hpp"
#include "QAnnotatedToken.hpp"
#include "QSourceLocation.hpp"
#include "qcsatest.h"
#include "qcsatestcase.h"
#include "qcsatestscenario.h"
#include "testscenario.h"

int main(int argc, char *argv[])
{
    qmlRegisterUncreatableType<csa::QSourceLocation>(
        "CSA", 1, 0, "SourceLocation", "Source locations can be created from the codeBase or ASTFiles.");

    qmlRegisterUncreatableType<csa::QAnnotatedToken>(
        "CSA", 1, 0, "Token", "Only access to Token properties of nodes is allowed.");

    qmlRegisterUncreatableType<csa::QCodebase>(
        "CSA", 1, 0, "CodeBase", "Codebase is available only as a property.");

    qmlRegisterUncreatableType<csa::ast::QASTFile>(
        "CSA", 1, 0, "ASTFile", "ASTFile is available only as a property.");

    qmlRegisterUncreatableType<csa::ast::QASTNode>(
        "CSA", 1, 0, "ASTNode", "ASTNode is available only as a property.");

    qmlRegisterUncreatableType<csa::QCSATest>(
        "CSA", 1, 0, "Test", "CSATest is available only as a property.");
    qmlRegisterUncreatableType<csa::QCSATestCase>(
        "CSA", 1, 0, "TestCase", "CSATestCase is available only as a property.");
    qmlRegisterUncreatableType<csa::QCSATestScenario>(
        "CSA", 1, 0, "TestScenario", "CSATestScenario is available only as a property.");

    QCoreApplication app(argc, argv);
    app.setAttribute(Qt::AA_Use96Dpi, true);

    csa::QCSATest csatest;
    csatest.loadTestFile("D:/sometest.js");

    int code = 0;

    for ( csa::QCSATest::Iterator it = csatest.begin(); it != csatest.end(); ++it ){
        csa::QCSATestCase* testcase = *it;
        for ( csa::QCSATestCase::Iterator tcit = testcase->begin(); tcit != testcase->end(); ++tcit ){
            TestScenario scenario(testcase->name(), *tcit);
            code += QTest::qExec(&scenario, argc, argv);
        }
    }

    return code;
}
