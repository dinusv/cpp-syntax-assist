#ifndef CSACONSTRUCTOR_HPP
#define CSACONSTRUCTOR_HPP

#include "CSANode.hpp"

namespace csa{

class TokenClassifier;

namespace ast{

class CSAConstructor : public CSANode{

public:
    CSAConstructor(AnnotatedTokenSet* tokenSet, TokenClassifier* classifier, SourceLocation* cursorLocation, SourceLocation* rangeStartLocation, SourceLocation* rangeEndLocation, CSANode* parent = 0);

    std::string content() const;
    std::string identifier() const;

private:
    std::string m_identifier;
};

inline std::string CSAConstructor::identifier() const{
    return m_identifier;
}

inline std::string CSAConstructor::content() const{
    return m_identifier;
}

}}// namespace

#endif // CSACONSTRUCTOR_HPP
