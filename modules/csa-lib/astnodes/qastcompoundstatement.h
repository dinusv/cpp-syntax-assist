#ifndef QASTCOMPOUNDSTATEMENT_H
#define QASTCOMPOUNDSTATEMENT_H

#include "qcsaglobal.h"
#include "qastnode.h"

namespace csa{ namespace ast{

class Q_CSA_EXPORT QASTCompoundStatement : public QASTNode{

public:
    QASTCompoundStatement(
            QAnnotatedTokenSet* tokenSet,
            QSourceLocation* cursorLocation,
            QSourceLocation* rangeStartLocation,
            QSourceLocation* rangeEndLocation,
            QASTNode* parent = 0
    );
    ~QASTCompoundStatement();
};

}}// namespace

#endif // QASTCOMPOUNDSTATEMENT_H
