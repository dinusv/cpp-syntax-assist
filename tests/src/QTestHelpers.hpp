#ifndef QTESTHELPERS_HPP
#define QTESTHELPERS_HPP

#include <QJsonValue>

class QScriptValue;

namespace helpers{

    QJsonValue jsonFromScriptValue(const QScriptValue& val);
    bool compareJsonValues(const QJsonValue& val1, const QJsonValue& val2);
    QJsonObject parseJsonFile(const QString& filePath, bool* parseOk = 0);

}// namespace

#endif // QTESTHELPERS_HPP
