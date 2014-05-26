#ifndef VISITOR_HPP
#define VISITOR_HPP

#include "clang-c/Index.h"

namespace csa{

class TokenClassifier;
namespace ast{
class CSANode;
}

class Visitor{

public:
    Visitor();

    static void createCSANodeTree(const CXCursor& rootCursor, ast::CSANode* root, TokenClassifier* classifier );
    static CXChildVisitResult callback(CXCursor cursor, CXCursor, CXClientData data);

};

}// namespace

#endif // VISITOR_HPP
