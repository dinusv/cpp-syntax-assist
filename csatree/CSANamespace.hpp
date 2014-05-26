#ifndef CSANAMESPACE_HPP
#define CSANAMESPACE_HPP

#include "CSANode.hpp"

namespace csa{
class AnnotatedTokenSet;

namespace ast{

class CSANamespace : public CSANode{

public:
    enum OffsetId{
        BEGIN = 0,
        END,
        CURSOR
    };

public:
    CSANamespace(
            AnnotatedTokenSet* tokenSet,
            SourceLocation* cursorLocation,
            SourceLocation* rangeStartLocation,
            SourceLocation* rangeEndLocation,
            CSANode* parent = 0);
    virtual ~CSANamespace();

    virtual std::string content() const;
    virtual std::string identifier() const;

    virtual const SourceLocation* location(const std::string& id) const;
    virtual const SourceLocation* location(int id) const;

    virtual CSANode* propagateUserCursor(const SourceLocation &location);

private:
    std::string        m_identifier;
    CSANode::OffsetMap m_offsets;

    static const CSANode::OffsetKeyMap OFFSET_KEYS;
    static const CSANode::OffsetKeyMap createOffsetKeys();
};

inline std::string CSANamespace::content() const{
    return typeString() + identifier();
}

inline std::string CSANamespace::identifier() const{
    return m_identifier;
}


}}// namespace

#endif // CSANAMESPACE_HPP
