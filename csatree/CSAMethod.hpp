#ifndef CSAMETHOD_HPP
#define CSAMETHOD_HPP

#include "CSANode.hpp"

namespace csa{

class TokenClassifier;

namespace ast{

class CSAMethod : public CSANode{

public:
    CSAMethod(AnnotatedTokenSet* tokenSet, TokenClassifier* classifier, SourceLocation* cursorLocation, SourceLocation* rangeStartLocation, SourceLocation* rangeEndLocation, CSANode* parent = 0);

    std::string content() const;
    std::string identifier() const;

private:
    std::string m_identifier;
};

inline std::string CSAMethod::identifier() const{
    return m_identifier;
}

inline std::string CSAMethod::content() const{
    return m_identifier;
}

}}// namespace

#endif // CSAMETHOD_HPP
