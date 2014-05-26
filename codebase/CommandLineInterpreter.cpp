#include "CommandLineInterpreter.hpp"
#include "CodeBase.hpp"
#include "CSANode.hpp"
#include <QtScript>
#include <QDebug>

CommandLineInterpreter::CommandLineInterpreter(QQuickItem *parent)
    : QQuickItem(parent)
    , m_configuration(0)
    , m_codeBase(0){

}

CommandLineInterpreter::CommandLineInterpreter(QScriptValue* configuration, CodeBase* astModel, QQuickItem *parent)
    : QQuickItem(parent)
    , m_configuration(configuration)
    , m_codeBase(astModel){

    updateSelectedProperty();
}

void CommandLineInterpreter::updateSelectedProperty(){
    m_selectedProperty = QString("csa") + QString::fromStdString(m_codeBase->selected()->typeString());
}


bool CommandLineInterpreter::execute(const QString &command){
    if ( !m_configuration )
        return false;

    int from = 0, to = 0;
    QScriptValue current = m_configuration->property(m_selectedProperty);

    while( ( to = command.indexOf(QChar(' '), from) ) != -1 ){
        current = current.property(command.mid(from, to - from));
        if ( !current.isValid() ){
            qDebug() << "Invalid property : " << command.mid(from, to - from);
            return false;
        }
        if ( current.isFunction() ){
            break;
        }
        from = to + 1;
    }
    if ( to == -1 ){
        current = current.property(command.mid(from));
        to      = command.length();
    }
    if ( current.isFunction() ){
        QString params = command.mid(to + 1).trimmed();
        QStringList paramList;
        QString currentParam;
        QString::Iterator c = params.begin();
        bool escapeFlag = false;
        while ( c != params.end() ){
            if ( *c == QChar('\\') ){
                if ( !escapeFlag ){
                    escapeFlag = true;
                } else {
                    currentParam.append('\\');
                    escapeFlag = false;
                }
            } else if ( *c == QChar('.')){
                if ( !escapeFlag ){ // add to array
                    paramList << currentParam;
                    currentParam.clear();
                } else {
                    currentParam.append(*c);
                    escapeFlag = false;
                }
            } else {
                if ( escapeFlag ){
                    currentParam.append(QChar('\\'));
                    escapeFlag = false;
                }
                currentParam.append(*c);
            }
            ++c;
        }
        if ( currentParam != "" )
            paramList << currentParam;

        QScriptValue paramScriptArray = current.engine()->newArray(paramList.size());
        for ( int i = 0; i < paramList.size(); ++i ){
            paramScriptArray.setProperty(i, paramList[i]);
        }
        QScriptValueList args;
        args << paramScriptArray;
        current.call(QScriptValue(), args);
        if ( current.engine()->hasUncaughtException() ){
            int line    = current.engine()->uncaughtExceptionLineNumber();
            QString str = current.engine()->uncaughtException().toString();
            current.engine()->clearExceptions();
            qDebug() << "Uncaught javascript exception at line " << line << ":" << str;
            return false;
        }

        updateSelectedProperty();
        return true;
    } else {
        qDebug() << "Invalid function : " << command.mid(from, to - from);
    }

    return false;
}
