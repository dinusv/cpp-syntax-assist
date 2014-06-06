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
