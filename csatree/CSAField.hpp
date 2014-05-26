#ifndef CSAFIELD_HPP
#define CSAFIELD_HPP

#include "CSANode.hpp"

namespace csa{ namespace ast{

class CSAField : public CSANode{

public:
    CSAField(AnnotatedTokenSet* tokenSet, SourceLocation* cursorLocation, SourceLocation* rangeStartLocation, SourceLocation* rangeEndLocation, CSANode* parent = 0);

    std::string content() const;
    std::string identifier() const;

private:
    std::string m_identifier;
};

inline std::string CSAField::identifier() const{
    return m_identifier;
}

inline std::string CSAField::content() const{
    return m_identifier;
}

}}// namespace

#endif // CSAFIELD_HPP
