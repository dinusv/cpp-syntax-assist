#ifndef QASTSCRIPTCONVERT_HPP
#define QASTSCRIPTCONVERT_HPP

#include <QScriptEngine>
#include <QScriptValue>
#include "QASTNode.hpp"

namespace csa{

// JS Conversion
// -------------

QScriptValue Q_CSA_EXPORT nodeToScriptValue(QScriptEngine *engine, ast::QASTNode* const & node);
void Q_CSA_EXPORT nodeFromScriptValue(const QScriptValue &obj, ast::QASTNode* &node);

}// namespace


#endif // QASTSCRIPTCONVERT_HPP
