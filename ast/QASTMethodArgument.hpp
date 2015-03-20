#ifndef QASTMETHODARGUMENT_HPP
#define QASTMETHODARGUMENT_HPP

#include "QASTNode.hpp"

namespace csa{ namespace ast{

class QASTMethodArgument : public QASTNode{

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
