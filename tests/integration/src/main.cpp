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
#include "qtestrunner.h"
#include "qastsearchtest.h"
#include "qcsatestingtest.h"

#include <qqml.h>
#include "qcsaengine.h"
#include "qcsatest.h"
#include "qcsatestcase.h"
#include "qcsatestscenario.h"

int main(int argc, char *argv[])
{
    QCoreApplication app(argc, argv);
    app.setAttribute(Qt::AA_Use96Dpi, true);

    csa::QCSAEngine::registerBaseTypes();
    csa::QCSAEngine::registerASTTypes();
    csa::QCSATest::registerTestingTypes();

    return QTestRunner::runTests(argc, argv);
}
