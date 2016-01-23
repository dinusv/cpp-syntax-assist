#ifndef QJSONCONVERT_HPP
#define QJSONCONVERT_HPP

#include "QCSAGlobal.hpp"
#include <QJsonObject>
#include <QJsonDocument>
#include <QJSValue>

namespace csa{

class Q_CSA_EXPORT QJsonConvert{

public:
    static QJsonValue jsonFromScriptValue(const QJSValue& jsval);
    static QJSValue scriptValueFromJson(const QJsonValue& jsonval);

private:
    QJsonConvert();
};

}// namespace

#endif // QJSONCONVERT_HPP
