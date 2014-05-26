#ifndef CSAACCESSSPECIFIER_HPP
#define CSAACCESSSPECIFIER_HPP

#include "CSANode.hpp"

namespace csa{ namespace ast{

class CSAAccessSpecifier : public CSANode{

public:
    CSAAccessSpecifier(AnnotatedTokenSet* tokenSet, SourceLocation* cursorLocation, SourceLocation* rangeStartLocation, SourceLocation* rangeEndLocation, CSANode* parent = 0);

    virtual std::string content() const;
    virtual std::string identifier() const;

private:
    std::string m_typeValueName;
};

inline std::string CSAAccessSpecifier::content() const{
    return m_typeValueName + " :";
}

inline std::string CSAAccessSpecifier::identifier() const{
    return m_typeValueName;
}

}}// namespace

#endif // CSAACCESSSPECIFIER_HPP
