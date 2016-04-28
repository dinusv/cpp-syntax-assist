#ifndef QJSONCONVERT_H
#define QJSONCONVERT_H

#include "qcsaglobal.h"
#include <QJsonObject>
#include <QJsonDocument>
#include <QJSValue>

class QJSEngine;

namespace csa{

class Q_CSA_EXPORT QJsonConvert{

public:
    static QJsonValue jsonFromScriptValue(const QJSValue& jsval);
    static QJSValue scriptValueFromJson(QJSEngine* engine, const QJsonValue& jsonval);
    static QJSValue scriptValueFromJsonDoc(QJSEngine* engine, const QJsonDocument& jsondoc);

private:
    static QJSValue scriptValueFromJsonObject(QJSEngine* engine, const QJsonObject& jsonobject);
    static QJSValue scriptValueFromJsonArray(QJSEngine *engine, const QJsonArray& jsonarray);

    QJsonConvert();
};

}// namespace

#endif // QJSONCONVERT_H
