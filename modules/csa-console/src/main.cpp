#include <QQmlContext>
#include <QQmlEngine>
#include <qqml.h>

#include "qcodebase.h"
#include "qcodebaseconfig.h"
#include "qcsaconsole.h"
#include "qcsaengine.h"
#include "qcsaconsolearguments.h"
#include "qcsainputhandler.h"

#include "qsourcelocation.h"
#include "qannotatedtoken.h"
#include "qcsaengine.h"
#include "qcsacompletionset.h"
#include "qastnode.h"
#include "qastfile.h"

using namespace csa;
using namespace csa::ast;

int main(int argc, char* argv[]){

    // Initialize command line arguments
    // ---------------------------------

    QCoreApplication app(argc, argv);
    QCoreApplication::setApplicationName("csa-console");
    QCoreApplication::setApplicationVersion("0.3.0");

    QCSAConsoleArguments commandLineArguments(
        app,
        "\nA configurable C and C++ code parser that exposes the ast model to javascript for manipulation."
    );

    QCSAConsole::setLogLevel(
        commandLineArguments.logLevel() > 4 ?
            QCSAConsole::getLogLevel() : static_cast<QCSAConsole::LogLevel>(commandLineArguments.logLevel())
    );

    // Assert Arguments
    // ----------------

    if ( commandLineArguments.hasFileErrors() ){
        for ( QStringList::const_iterator it = commandLineArguments.fileErrors().begin();
              it != commandLineArguments.fileErrors().end();
              ++it )
        {
            QCSAConsole::logError(qPrintable(*it));
        }
        return -1;
    }

    // Create Codebase
    // ---------------

    QCSAEngine scriptEngine(new QJSEngine);
    QCodebaseConfig* config = new QCodebaseConfig(
        scriptEngine.engine(),
        QCoreApplication::applicationDirPath() + "/config", "default"
    );
    QCodebase codeBase(config, commandLineArguments.files(), commandLineArguments.projectDir(), 0);

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

    // Configure Engine
    // ----------------

    qmlRegisterUncreatableType<QCSAConsole>(
        "CSA", 1, 0, "ConfiguredDebugger", "Only access to the debug property is allowed.");

    qmlRegisterUncreatableType<QCSAEngine>(
        "CSA", 1, 0, "ConfiguredEngine", "Only access to the engine property is allowed.");

    qmlRegisterUncreatableType<QCSACompletionSet>(
        "CSA", 1, 0, "CompletinoSet", "Only access to the plugins property is allowed.");

    qmlRegisterUncreatableType<csa::QCodebase>(
        "CSA", 1, 0, "CodeBase", "Codebase is available only as a property.");

    qmlRegisterUncreatableType<csa::QSourceLocation>(
        "CSA", 1, 0, "SourceLocation", "Source locations can be created from the codeBase or ASTFiles.");

    qmlRegisterUncreatableType<csa::QAnnotatedToken>(
        "CSA", 1, 0, "Token", "Only access to Token properties of nodes is allowed.");

    qmlRegisterUncreatableType<csa::ast::QASTFile>(
        "CSA", 1, 0, "ASTFile", "ASTFile is available only as a property.");

    qmlRegisterUncreatableType<csa::ast::QASTNode>(
        "CSA", 1, 0, "ASTNode", "ASTNode is available only as a property.");

    QCSACompletionSet completionSet;

    scriptEngine.setContextObject("codeBase", &codeBase);
    scriptEngine.setContextObject("plugins",  &completionSet);
    scriptEngine.loadNodeCollection();
    scriptEngine.loadNodesFunction();
    scriptEngine.loadFileFunctions();

    int loaderError = scriptEngine.loadPlugins(QCoreApplication::applicationDirPath() + "/plugins");
    if ( loaderError != 0 )
        return loaderError;

    if( commandLineArguments.isExecuteAndQuitSet() ){
        QJSValue result;
        if ( scriptEngine.execute(commandLineArguments.selectedFunction(), result) ){
            if ( !result.isUndefined() )
                QCSAConsole::log(QCSAConsole::General, result.toString());
            return 0;
        }
        return 1;
    }

    QCSAInputHandler::getInstance().initPluginHandlers(&scriptEngine, &completionSet);
    return QCSAInputHandler::getInstance().inputLoop();
}
