#ifndef QASTSCRIPTCONVERT_HPP
#define QASTSCRIPTCONVERT_HPP

#include <QScriptEngine>
#include <QScriptValue>
#include "QASTNode.hpp"

namespace csa{

// JS Conversion
// -------------

QScriptValue nodeToScriptValue(QScriptEngine *engine, ast::QASTNode* const & node);
void nodeFromScriptValue(const QScriptValue &obj, ast::QASTNode* &node);


}// namespace


#endif // QASTSCRIPTCONVERT_HPP
