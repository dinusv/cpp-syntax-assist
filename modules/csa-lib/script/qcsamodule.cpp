#include "qcsamodule.h"
#include "qcsaengine.h"
#include <QJSEngine>
#include <QFileInfo>
#include <QDir>

namespace csa{

QCSAModule::QCSAModule(const QString &filename, const QString &dirname, QCSAEngine *parent)
    : QObject(parent)
    , m_engine(parent)
    , m_filename(filename)
    , m_dirname(dirname)
    , m_exports(parent->engine()->newObject())
{
}

QCSAModule::~QCSAModule(){
}

QJSValue QCSAModule::evaluateExports(QJSValue& moduleCall){
    QJSValueList arglist;
    arglist << m_exports << m_engine->engine()->newQObject(this) << QJSValue(dirname() + "/" + filename()) << dirname();

    QJSValue result = moduleCall.call(arglist);
    if ( result.isError() )
        return result;
    return m_exports;
}

/**
 * @brief Depth first module loading
 */
QJSValue QCSAModule::load(QString module){
    QFileInfo finfo(module);
    if ( finfo.isAbsolute() ){
        return m_engine->loadFile(finfo, this);
    } else {
        QFileInfo finfo(QDir::cleanPath(dirname() + (dirname().isEmpty() ? "" : "/") + module));
        return m_engine->loadFile(finfo, this);
    }
}

}// namespace
