#ifndef QCODEBASEOBSERVER_HPP
#define QCODEBASEOBSERVER_HPP

#include "QCSAGlobal.hpp"

namespace csa{

namespace ast{
class QASTNode;
}

class Q_CSA_EXPORT QCodeBaseObserver{

public:
    QCodeBaseObserver();
    virtual ~QCodeBaseObserver();

    virtual void clearAndReset() = 0;
    virtual void setSelected(ast::QASTNode* node) = 0;
    virtual void parse(ast::QASTNode* root) = 0;
};

}// namespace

#endif // QCODEBASEOBSERVER_HPP
