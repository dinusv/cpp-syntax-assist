#ifndef CSADESTRUCTOR_HPP
#define CSADESTRUCTOR_HPP

#include "CSANode.hpp"

namespace csa{ namespace ast{

class CSADestructor : public CSANode{

public:
    CSADestructor(AnnotatedTokenSet* tokenSet, SourceLocation* cursorLocation, SourceLocation* rangeStartLocation, SourceLocation* rangeEndLocation, CSANode* parent = 0);

    std::string content() const;
    std::string identifier() const;

private:
    std::string m_identifier;
};

inline std::string CSADestructor::identifier() const{
    return m_identifier;
}

inline std::string CSADestructor::content() const{
    return m_identifier;
}

}}// namespace

#endif // CSADESTRUCTOR_HPP
