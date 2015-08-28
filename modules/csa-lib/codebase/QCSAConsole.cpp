#include "QCSAConsole.hpp"

namespace csa{

QCSAConsole::LogLevel QCSAConsole::maxLogLevel = QCSAConsole::Debug;

QCSAConsole::QCSAConsole()
    : QObject(0)
{
}

QCSAConsole::~QCSAConsole(){
}

QCSAConsole &QCSAConsole::instance(){
    static QCSAConsole instance;
    return instance;
}

void QCSAConsole::log(LogLevel level, const QString &message){
    if ( level <= maxLogLevel )
        qDebug("%s", qPrintable(message));
}

void QCSAConsole::logError(const QString &message){
    qCritical("%s\n", qPrintable(message));
}

void QCSAConsole::setLogLevel(QCSAConsole::LogLevel level){
    maxLogLevel = level;
}

QCSAConsole::LogLevel QCSAConsole::getLogLevel(){
    return maxLogLevel;
}

void QCSAConsole::log(const QString& message){
    log(QCSAConsole::Debug, message);
}

void QCSAConsole::warn(const QString &message){
    log(QCSAConsole::Warning, message);
}

void QCSAConsole::error(const QString &message){
    logError("Script error: " + message);
}

}// namespace
