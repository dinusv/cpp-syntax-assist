#ifndef CSAFILEROOT_HPP
#define CSAFILEROOT_HPP

#include "CSANode.hpp"

namespace csa{ namespace ast{

class CSAFileRoot : public CSANode{

public:
    CSAFileRoot(const std::string& file, SourceLocation* endOfFile);

    std::string content() const;
    std::string identifier() const;

private:
    std::string m_identifier;
};

inline std::string CSAFileRoot::content() const{
    return std::string("file \"") + m_identifier + "\"";
}

inline std::string CSAFileRoot::identifier() const{
    return m_identifier;
}

}}//namespace

#endif // CSAFILEROOT_HPP
