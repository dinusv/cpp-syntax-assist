#ifndef QCSACONSOLE_H
#define QCSACONSOLE_H

#include "QCSAGlobal.hpp"
#include <QObject>
#include <QString>

namespace csa{

class Q_CSA_EXPORT QCSAConsole : public QObject{

    Q_OBJECT

public:
    enum LogLevel{
        General,
        Warning,
        Debug,
        Info1,
        Info2
    };

    static void log(LogLevel level, const QString &message);
    static void logError(const QString& message);

    static void setLogLevel(LogLevel level);
    static LogLevel getLogLevel();

    static QCSAConsole& instance();

public slots:
    void log(const QString& message);
    void warn(const QString& message);
    void error(const QString& message);

private:
    QCSAConsole();
    ~QCSAConsole();

    QCSAConsole(const QCSAConsole& other);
    QCSAConsole& operator = (const QCSAConsole& other);

    static LogLevel maxLogLevel;

};

}// namespace

#endif // QCSACONSOLE_H
