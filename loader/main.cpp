/****************************************************************************
**
** Copyright (C) 2014 Dinu SV.
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
#include "qtquick2applicationviewer.h"

#include "QConfiguredEngine.hpp"
#include "QSyntaxTreeModel.hpp"
#include "QASTNode.hpp"
#include "QCodeBase.hpp"
#include "QTokenClassifier.hpp"
#include "QASTNodeConvert.hpp"
#include "QAnnotatedTokenConvert.hpp"
#include "QCsaPluginLoader.hpp"

#include "QCsaCommandLineArguments.hpp"

using namespace csa;
using namespace csa::ast;

int main(int argc, char *argv[]){

    QGuiApplication app(argc, argv);
    QGuiApplication::setApplicationName("cpp-snippet-assist");
    QGuiApplication::setApplicationDisplayName("C++ Snippet Assist");
    QGuiApplication::setApplicationVersion("0.3.0");

    QCsaCommandLineArguments commandLineArguments(
                app,
                "\nA configurable C and C++ code parser that exposes the ast model to javascript for manipulation.");

    // Create codebase
    // ---------------

    const char* args[] = {"-c", "-x", "c++"};

    QSyntaxTreeModel* astTreeModel = new QSyntaxTreeModel;
    QCodeBase cBase(args, 3, commandLineArguments.file(), astTreeModel);

    if ( commandLineArguments.isCursorOffsetSet() ){
        cBase.propagateUserCursor(commandLineArguments.cursorOffset(), commandLineArguments.file());
    } else if ( commandLineArguments.isCursorLineColumnSet() ){
        cBase.propagateUserCursor(
                    commandLineArguments.cursorLine(), commandLineArguments.cursorColumn(), commandLineArguments.file());
    } else {
        cBase.propagateUserCursor(0, commandLineArguments.file());
    }

    QCsaPluginLoader loader(QDir::currentPath() + "/plugins");

    QScriptValue scriptCBase = loader.engine()->newQObject(&cBase, QScriptEngine::QtOwnership);
    loader.engine()->globalObject().setProperty("codeBase", scriptCBase);

    int loaderError = loader.initializePlugins();
    if ( loaderError != 0 )
        return loaderError;

    // Interpret command
    // -----------------

    QConfiguredEngine configuredEngine(loader.engine());
    if( commandLineArguments.isExecuteAndQuitSet() )
        if( configuredEngine.execute(commandLineArguments.selectedFunction()) )
            return 0;

    QtQuick2ApplicationViewer viewer;
    qmlRegisterType<QSyntaxTreeModel>("CSA", 1, 0, "SyntaxTree" );
    qmlRegisterType<QConfiguredEngine>("CSA", 1, 0, "ConfiguredEngine");
    viewer.rootContext()->setContextProperty("syntaxTreeModel",   astTreeModel);
    viewer.rootContext()->setContextProperty("configuredEngine", &configuredEngine);
    viewer.rootContext()->setContextProperty("command",           commandLineArguments.selectedFunction());
    viewer.setTitle("C++ Snippet Assist");
    viewer.setMainQmlFile(QUrl::fromUserInput("qrc:/qml/main.qml"));
    viewer.show();

    return app.exec();
}


