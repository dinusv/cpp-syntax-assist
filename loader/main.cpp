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

using namespace csa;
using namespace csa::ast;

int main(int argc, char *argv[]){

    QString argUsage   = "<file> [-c <offset> -lc <line> <column> -e <command> -f].";
    QString argFile    = "";
    QString argCommand = "";
    int  argOffset = -1, argLine = -1, argColumn = -1;
    bool fullCommandFlag = false;

    // Parse Command line arguments
    // ----------------------------

    QGuiApplication app(argc, argv);
    QStringList arguments    = app.arguments();
    QStringList::Iterator it = arguments.begin(); ++it;
    for ( ; it != arguments.end(); ++it ){
        if ( *it == "-c" ){
            if ( ++it == arguments.end() ){
                qCritical( (   "Error parsing command line arguments: no value after '-c'. Usage : " +
                               argUsage).toStdString().c_str() );
                return 1;
            }
            argOffset = (*it).toInt();
        } else if ( *it == "-lc" ){
            if ( ++it == arguments.end() ){
                qCritical( (   "Error parsing command line arguments: no line value after '-cl'. Usage : " +
                               argUsage).toStdString().c_str() );
                return 1;
            }
            argLine   = (*it).toInt();
            if ( ++it == arguments.end() ){
                qCritical( (   "Error parsing command line arguments: no column value after '-cl'. Usage : " +
                               argUsage).toStdString().c_str() );
                return 1;
            }
            argColumn = (*it).toInt();
        } else if ( *it == "-e" ){
            if ( ++it == arguments.end() ){
                qCritical( (   "Error parsing command line arguments: no command value after '-e'. Usage : " +
                               argUsage).toStdString().c_str() );
                return 1;
            }
            argCommand = (*it);
        } else if ( *it == "-f" ){
            fullCommandFlag = true;
        } else {
            argFile    = (*it);
        }
    }

    if ( argFile == "" ){
        qCritical( (
            "Error parsing comamnd line arguments : no file specified. Usage : " + argUsage).toStdString().c_str() );
        return 1;
    }

    if ( argOffset == -1 && argColumn == -1 )// user no cursor
        argOffset = 0;

    // Create codebase
    // ---------------

    const char* args[] = {"-c", "-x", "c++"};

    QSyntaxTreeModel* astTreeModel = new QSyntaxTreeModel;
    QCodeBase cBase(args, 3, argFile, astTreeModel);

    if ( argOffset != -1 )
        cBase.propagateUserCursor(argOffset, argFile);
    else
        cBase.propagateUserCursor(argLine, argColumn, argFile);

    QCsaPluginLoader loader(QDir::currentPath() + "/plugins");

    QScriptValue scriptCBase = loader.engine()->newQObject(&cBase, QScriptEngine::QtOwnership);
    loader.engine()->globalObject().setProperty("codeBase", scriptCBase);

    int loaderError = loader.initializePlugins();
    if ( loaderError != 0 )
        return loaderError;

    // Interpret command
    // -----------------

    QConfiguredEngine configuredEngine(loader.engine());
    if( fullCommandFlag )
        if( configuredEngine.execute(argCommand) )
            return 0;

    QtQuick2ApplicationViewer viewer;
    qmlRegisterType<QSyntaxTreeModel>("CSA", 1, 0, "SyntaxTree" );
    qmlRegisterType<QConfiguredEngine>("CSA", 1, 0, "ConfiguredEngine");
    viewer.rootContext()->setContextProperty("syntaxTreeModel",   astTreeModel);
    viewer.rootContext()->setContextProperty("configuredEngine", &configuredEngine);
    viewer.rootContext()->setContextProperty("command",           argCommand);
    viewer.setTitle("C++ Snippet Assist");
    viewer.setMainQmlFile(QUrl::fromUserInput("qrc:/qml/main.qml"));
    viewer.show();

    return app.exec();
}


