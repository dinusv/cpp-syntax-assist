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

#include <QtGui/QGuiApplication>
#include <QtQuick/QQuickItem>
#include <QtScript/QScriptEngine>
#include <QQmlContext>
#include <QQmlApplicationEngine>

#include "QCodeBase.hpp"
#include "QCSAScriptEngine.hpp"

#include "QSyntaxTreeModel.hpp"
#include "QCsaCommandLineArguments.hpp"

using namespace csa;
using namespace csa::ast;

int main(int argc, char *argv[]){

    // Initialize command line arguments
    // ---------------------------------

    QGuiApplication app(argc, argv);
    QGuiApplication::setApplicationName("cpp-snippet-assist");
    QGuiApplication::setApplicationDisplayName("C++ Snippet Assist");
    QGuiApplication::setApplicationVersion("0.3.0");

    QCsaCommandLineArguments commandLineArguments(
        app,
        "\nA configurable C and C++ code parser that exposes the ast model to javascript for manipulation."
    );

    // Assert Arguments
    // ----------------

    if ( !QFileInfo::exists(commandLineArguments.file()) ){
        qCritical("%s", qPrintable("Error: Input file does not exist: " + commandLineArguments.file()));
        return -1;
    }

    // Create Codebase
    // ---------------

    QSyntaxTreeModel* astTreeModel = new QSyntaxTreeModel;

    const char* args[] = {"-c", "-x", "c++"};
    QCodeBase codeBase(args, 3, commandLineArguments.file(), astTreeModel);

    if ( commandLineArguments.isCursorOffsetSet() ){
        codeBase.propagateUserCursor(commandLineArguments.cursorOffset(), commandLineArguments.file());
    } else if ( commandLineArguments.isCursorLineColumnSet() ){
        codeBase.propagateUserCursor(
            commandLineArguments.cursorLine(), commandLineArguments.cursorColumn(), commandLineArguments.file());
    } else {
        codeBase.propagateUserCursor(0, commandLineArguments.file());
    }

    // Load plugins
    // ------------

    QCSAScriptEngine engine;
    engine.setCodeBase(&codeBase);

    int loaderError = engine.loadPlugins(QGuiApplication::applicationDirPath() + "/plugins");
    if ( loaderError != 0 )
        return loaderError;

    // Interpret command
    // -----------------

    if( commandLineArguments.isExecuteAndQuitSet() )
        if( engine.execute(commandLineArguments.selectedFunction()) )
            return 0;

    QQmlApplicationEngine viewer;
    qmlRegisterType<QSyntaxTreeModel>("CSA", 1, 0, "SyntaxTree" );
    qmlRegisterType<QCSAScriptEngine>("CSA", 1, 0, "ConfiguredEngine");
    viewer.rootContext()->setContextProperty("syntaxTreeModel",   astTreeModel);
    viewer.rootContext()->setContextProperty("configuredEngine",  &engine);
    viewer.rootContext()->setContextProperty("command",           commandLineArguments.selectedFunction());
    viewer.load(QUrl(QStringLiteral("qrc:/main.qml")));

    return app.exec();
}


