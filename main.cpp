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

#include "CommandLineInterpreter.hpp"
#include "SyntaxTreeModel.hpp"
#include "CSANode.hpp"
#include "CodeBase.hpp"
#include "TokenClassifier.hpp"

using namespace csa;
using namespace csa::ast;

int main(int argc, char *argv[]){

    QString argUsage   = "<file> [-c <offset> -cl <line> <column> -e <command> -f].";
    QString argFile    = "";
    QString argCommand = "";
    int  argOffset = -1, argLine = -1, argColumn = -1;
    bool fullCommandFlag = false;

    // Parse Command line arguments

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
        } else if ( *it == "-cl" ){
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

    const char* args[] = {"-c", "-x", "c++"};

    SyntaxTreeModel* astTreeModel = new SyntaxTreeModel;
    CodeBase cBase(args, 3, argFile, astTreeModel);

    if ( argOffset != -1 )
        cBase.propagateUserCursor(argOffset, argFile);
    else
        cBase.propagateUserCursor(argLine, argColumn, argFile);

    // QtScript

    QFile configScript(QDir::currentPath() + "/configuration.js");
    if ( !configScript.open(QIODevice::ReadOnly) ){
        qDebug() << "Error opening js configuration file. Make sure the file is present in " + QDir::currentPath() + ".";
        return 2;
    }
    QTextStream configStream(&configScript);
    QScriptEngine configEngine;
    QScriptValue scriptCBase = configEngine.newQObject(&cBase, QScriptEngine::QtOwnership);
    configEngine.globalObject().setProperty("codeBase", scriptCBase);
    configEngine.evaluate(configStream.readAll(), "jscodecompletion.js");
    QScriptValue result = configEngine.globalObject();
    if ( configEngine.hasUncaughtException() ){
        int line = configEngine.uncaughtExceptionLineNumber();
        qFatal( (
                    "Uncaught javascript exception at line " + QString::number(line) + ":" +
                    result.toString()).toStdString().c_str());
        return 3;
    }

    // Interpret command

    CommandLineInterpreter cmdInterpreter(&result, &cBase);
    if( fullCommandFlag )
        if( cmdInterpreter.execute(argCommand) )
            return 0;

    QtQuick2ApplicationViewer viewer;
    qmlRegisterType<SyntaxTreeModel>("CSA", 1, 0, "SyntaxTree" );
    qmlRegisterType<CommandLineInterpreter>("CSA", 1, 0, "CommandInterpreter");
    viewer.rootContext()->setContextProperty("syntaxTreeModel",    astTreeModel);
    viewer.rootContext()->setContextProperty("commandInterpreter", &cmdInterpreter);
    viewer.rootContext()->setContextProperty("command",            argCommand);
    viewer.setTitle("C++ Snippet Assist");
    viewer.setMainQmlFile(QUrl::fromUserInput("qrc:/qml/main.qml"));
    viewer.show();

    return app.exec();
}


