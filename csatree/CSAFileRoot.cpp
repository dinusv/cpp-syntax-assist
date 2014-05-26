#include "CSAFileRoot.hpp"
#include "SourceLocation.hpp"
#include "TokenClassifier.hpp"

namespace csa{ namespace ast{

CSAFileRoot::CSAFileRoot(const std::string& file, SourceLocation *endOfFile)
    : CSANode("file",
              new SourceLocation(file, 0, 0, 0),
              new SourceLocation(file, 0, 0, 0),
              endOfFile, 0)
    , m_identifier(file){


}

}}//namespace
