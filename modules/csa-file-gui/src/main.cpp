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
#include <QQmlContext>
#include <QQmlApplicationEngine>

#include "QCodeBase.hpp"
#include "QCSAPluginLoader.hpp"
#include "QCSAPluginCollection.hpp"

#include "QSourceLocation.hpp"
#include "QAnnotatedToken.hpp"
#include "QASTNode.hpp"
#include "QASTFile.hpp"

#include "QASTCollapsibleModel.hpp"
#include "QCSAFileGuiArguments.hpp"

using namespace csa;
using namespace csa::ast;

int main(int argc, char *argv[]){

    // Initialize command line arguments
    // ---------------------------------

    QGuiApplication app(argc, argv);
    QGuiApplication::setApplicationName("csa-file-gui");
    QGuiApplication::setApplicationDisplayName("C++ Snippet Assist - File Gui");
    QGuiApplication::setApplicationVersion("0.3.0");

    QCSAFileGuiArguments commandLineArguments(
        app,
        "\nA configurable C and C++ code parser that exposes the ast model to javascript for manipulation."
    );

    // Assert Arguments
    // ----------------

    if ( commandLineArguments.hasFileErrors() ){
        for ( QStringList::const_iterator it = commandLineArguments.fileErrors().begin();
              it != commandLineArguments.fileErrors().end();
              ++it )
        {
            qCritical("%s\n", qPrintable(*it));
        }
        return -1;
    }

    // Create Codebase
    // ---------------

    const char* args[] = {"-c", "-x", "c++"};
    QCodeBase codeBase(args, 3, commandLineArguments.files(), commandLineArguments.projectDir(), 0);

    if ( commandLineArguments.isCursorOffsetSet() ){
        codeBase.propagateUserCursor(
            commandLineArguments.cursorOffset(),
            commandLineArguments.files().first()
        );
    } else if ( commandLineArguments.isCursorLineColumnSet() ){
        codeBase.propagateUserCursor(
            commandLineArguments.cursorLine(),
            commandLineArguments.cursorColumn(),
            commandLineArguments.files().first()
        );
    } else {
        codeBase.propagateUserCursor(0, commandLineArguments.files().first());
    }

    // Connect Codebase Signals
    // ------------------------

    QASTCollapsibleModel* astTreeModel = new QASTCollapsibleModel(codeBase.astFiles());
    astTreeModel->selectNode(codeBase.selectedNode());

    QObject::connect(&codeBase, SIGNAL(fileAdded(csa::ast::QASTFile*)),
                     astTreeModel, SLOT(addFile(csa::ast::QASTFile*)));
    QObject::connect(&codeBase, SIGNAL(fileAboutToBeReparsed(csa::ast::QASTFile*)),
                     astTreeModel, SLOT(collapseFile(csa::ast::QASTFile*)));
    QObject::connect(&codeBase, SIGNAL(fileReparsed(csa::ast::QASTFile*)),
                     astTreeModel, SLOT(reparseFile(csa::ast::QASTFile*)));
    QObject::connect(&codeBase, SIGNAL(nodeSelected(csa::ast::QASTNode*)),
                     astTreeModel, SLOT(selectNode(csa::ast::QASTNode*)));

    // Configure Engine
    // ----------------

    qmlRegisterUncreatableType<QASTCollapsibleModel>(
        "CSA", 1, 0, "SyntaxTree", "Only access to the SyntaxTree property is allowed.");

    qmlRegisterUncreatableType<QCSAPluginConsole>(
        "CSA", 1, 0, "ConfiguredDebugger", "Only access to the debug property is allowed.");

    qmlRegisterUncreatableType<QCSAPluginLoader>(
        "CSA", 1, 0, "ConfiguredEngine", "Only access to the engine property is allowed.");

    qmlRegisterUncreatableType<QCSAPluginCollection>(
        "CSA", 1, 0, "PluginCollection", "Only access to the plugins property is allowed.");

    qmlRegisterUncreatableType<csa::QCodeBase>(
        "CSA", 1, 0, "CodeBase", "Codebase is available only as a property.");

    qmlRegisterUncreatableType<csa::QSourceLocation>(
        "CSA", 1, 0, "SourceLocation", "Source locations can be created from the codeBase or ASTFiles.");

    qmlRegisterUncreatableType<csa::QAnnotatedToken>(
        "CSA", 1, 0, "Token", "Only access to Token properties of nodes is allowed.");

    qmlRegisterUncreatableType<csa::ast::QASTFile>(
        "CSA", 1, 0, "ASTFile", "ASTFile is available only as a property.");

    qmlRegisterUncreatableType<csa::ast::QASTNode>(
        "CSA", 1, 0, "ASTNode", "ASTNode is available only as a property.");

    QCSAPluginCollection pluginCollection;

    QCSAPluginLoader scriptEngine(new QJSEngine);
    scriptEngine.setContextObject("codeBase", &codeBase);
    scriptEngine.setContextObject("plugins",  &pluginCollection);
    scriptEngine.loadNodeCollection();
    scriptEngine.loadNodesFunction();
    scriptEngine.loadFileFunctions();

    QQmlApplicationEngine engine;
    engine.rootContext()->setContextProperty("syntaxTreeModel",  astTreeModel);
    engine.rootContext()->setContextProperty("configuredEngine", &scriptEngine);
    engine.rootContext()->setContextProperty("codeBase",         &codeBase);
    engine.rootContext()->setContextProperty("plugins",          &pluginCollection);
    engine.rootContext()->setContextProperty("command",          commandLineArguments.selectedFunction());

    int loaderError = scriptEngine.loadPlugins(QGuiApplication::applicationDirPath() + "/plugins");
    if ( loaderError != 0 )
        return loaderError;

    // Interpret command
    // -----------------

    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));

    return app.exec();
}


