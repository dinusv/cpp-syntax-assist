#ifndef COMMANDLINEINTERPRETER_HPP
#define COMMANDLINEINTERPRETER_HPP

#include <QQuickItem>

class QScriptValue;
class CodeBase;

class CommandLineInterpreter : public QQuickItem{

    Q_OBJECT

public:
    explicit CommandLineInterpreter(QQuickItem *parent = 0);
    CommandLineInterpreter(QScriptValue* configuration, CodeBase* astModel, QQuickItem *parent = 0);

    void updateSelectedProperty();

public slots:
    bool execute(const QString& command);
    const QString& message() const;

private:
    QString         m_message;
    QScriptValue*   m_configuration;
    CodeBase*       m_codeBase;
    QString         m_selectedProperty;
};

inline const QString& CommandLineInterpreter::message() const{
    return m_message;
}

#endif // COMMANDLINEINTERPRETER_HPP
