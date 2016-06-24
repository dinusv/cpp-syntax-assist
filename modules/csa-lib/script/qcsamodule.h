#ifndef QCSAMODULE_H
#define QCSAMODULE_H

#include "qcsaglobal.h"
#include <QObject>
#include <QJSValue>
#include <QLinkedList>

class QJSEngine;

namespace csa{

class QCSAEngine;
class Q_CSA_EXPORT QCSAModule : public QObject{

    Q_OBJECT
    Q_PROPERTY(QJSValue exports READ exports WRITE setExports)

public:
    explicit QCSAModule(const QString& fileName, const QString& dirName, QCSAEngine *parent = 0);
    ~QCSAModule();

    const QString& dirname() const;
    const QString& filename() const;
    QJSValue& exports();
    void setExports(QJSValue exports);

    QJSValue evaluateExports(QJSValue &moduleCall);

public slots:
    QJSValue load(QString module);

private:
    QCSAEngine* m_engine;
    QString  m_filename;
    QString  m_dirname;
    QJSValue m_exports;

};

inline const QString &QCSAModule::dirname() const{
    return m_dirname;
}

inline const QString &QCSAModule::filename() const{
    return m_filename;
}

inline QJSValue &QCSAModule::exports(){
    return m_exports;
}

inline void QCSAModule::setExports(QJSValue exports){
    m_exports = exports;
}

}

#endif // QCSAMODULE_H
