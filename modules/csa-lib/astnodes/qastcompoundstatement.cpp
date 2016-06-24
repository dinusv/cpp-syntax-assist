#include "qastcompoundstatement.h"

namespace csa{ namespace ast{


QASTCompoundStatement::QASTCompoundStatement(
        QAnnotatedTokenSet *tokenSet,
        QSourceLocation *cursorLocation,
        QSourceLocation *rangeStartLocation,
        QSourceLocation *rangeEndLocation,
        QASTNode *parent)
    : QASTNode("compoundStatement", tokenSet, cursorLocation, rangeStartLocation, rangeEndLocation, parent)
{
}

QASTCompoundStatement::~QASTCompoundStatement(){
}

}}// namespace
