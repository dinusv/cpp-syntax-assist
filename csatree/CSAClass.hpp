#ifndef CSACLASS_HPP
#define CSACLASS_HPP

#include "CSANode.hpp"

namespace csa{ namespace ast{

class CSAClass : public CSANode{

public:
    enum OffsetId{
        BEGIN = 0,
        END,
        QPROPERTY,
        PUBLIC,
        PROTECTED,
        PRIVATE,
        IMPLEMENTATION,
        CURSOR
    };

public:
    CSAClass(
            AnnotatedTokenSet* tokenSet,
            SourceLocation* cursorLocation,
            SourceLocation* rangeStartLocation,
            SourceLocation* rangeEndLocation,
            CSANode* parent = 0);
    virtual ~CSAClass();

    virtual std::string content() const;
    virtual std::string identifier() const;

    virtual const SourceLocation* location(const std::string& id) const;
    virtual const SourceLocation* location(int id) const;

    void setChildLocation(OffsetId id, const SourceLocation& location);

    virtual CSANode* propagateUserCursor(const SourceLocation &location);

private:
    std::string        m_identifier;
    CSANode::OffsetMap m_offsets;
    OffsetId           m_lastSet;
    bool               m_offsetsBeginSet;

    static const CSANode::OffsetKeyMap OFFSET_KEYS;
    static const CSANode::OffsetKeyMap createOffsetKeys();
};

inline std::string CSAClass::content() const{
    return typeString() + m_identifier;
}

inline std::string CSAClass::identifier() const{
    return m_identifier;
}

}}// namespace

#endif // CSACLASS_HPP
