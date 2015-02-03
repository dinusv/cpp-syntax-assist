#include "QASTScriptConvert.hpp"

namespace csa{

QScriptValue nodeToScriptValue(QScriptEngine* engine, ast::QASTNode* const &node){
    QScriptValue obj = engine->newQObject(node, QScriptEngine::QtOwnership);
    return obj;
}

void nodeFromScriptValue(const QScriptValue& obj, ast::QASTNode*& node){
    node = qobject_cast<ast::QASTNode*>(obj.toQObject());
}

}// namespace
