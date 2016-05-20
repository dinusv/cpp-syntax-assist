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

#include <qqml.h>
#include "qcsaengine.h"
#include "qcsaconsole.h"
#include "qcsatest.h"
#include "qcsatestcase.h"
#include "qcsatestscenario.h"
#include "testscenario.h"

int main(int argc, char *argv[])
{
    QCoreApplication app(argc, argv);
    app.setAttribute(Qt::AA_Use96Dpi, true);

    csa::QCSAEngine::registerBaseTypes();
    csa::QCSATest::registerTestingTypes();

    csa::QCSATest csatest;

    QString applicationPath = QCoreApplication::applicationDirPath();

    QStringList appArgs = app.arguments();
    if ( appArgs.size() > 1 ){
        for ( int i = 1; i < appArgs.size(); ++i ){
            QString& arg = appArgs[i];
            csa::QCSAConsole::log(csa::QCSAConsole::General, "Loading tests from " + arg);
            if ( QFileInfo(arg).isRelative() )
                csatest.loadTests(QDir::cleanPath(applicationPath + "/" + arg));
            else
                csatest.loadTests(arg);
        }
    } else {
        if ( QDir(applicationPath + "/coretests").exists() )
            csatest.loadTests(applicationPath + "/coretests");
        if ( QDir(applicationPath + "/plugintests").exists() )
            csatest.loadTests(applicationPath + "/plugintests");
    }

    csa::QCSAConsole::log(csa::QCSAConsole::General, "Loaded " + QString::number(csatest.totalTests()) + " tests.");

    int code = 0;

    for ( csa::QCSATest::Iterator it = csatest.begin(); it != csatest.end(); ++it ){
        csa::QCSATestCase* testcase = *it;
        for ( csa::QCSATestCase::Iterator tcit = testcase->begin(); tcit != testcase->end(); ++tcit ){
            TestScenario scenario(testcase->name(), *tcit);
            code += QTest::qExec(&scenario, 1, argv);
        }
    }

    return code;
}
