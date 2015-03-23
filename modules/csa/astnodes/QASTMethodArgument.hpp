#ifndef QASTMETHODARGUMENT_HPP
#define QASTMETHODARGUMENT_HPP

#include "QCSAGlobal.hpp"
#include "QCSAGlobal.hpp"
#include "QASTNode.hpp"

namespace csa{ namespace ast{

class Q_CSA_EXPORT QASTMethodArgument : public QASTNode{

    Q_OBJECT

public:
    QASTMethodArgument(
            QAnnotatedTokenSet* tokenSet,
            QSourceLocation* cursorLocation,
            QSourceLocation* rangeStartLocation,
            QSourceLocation* rangeEndLocation,
            QASTNode* parent = 0);
    virtual ~QASTMethodArgument();

    virtual QString content() const;

signals:

public slots:

private:
    QString m_type;

};

}} // namespace ast, csa

#endif // QASTMETHODARGUMENT_HPP
