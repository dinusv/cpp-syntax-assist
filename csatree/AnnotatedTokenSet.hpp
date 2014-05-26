#ifndef ANNOTATEDTOKENSET_HPP
#define ANNOTATEDTOKENSET_HPP

#include "clang-c/Index.h"
#include <vector>
#include <string>

namespace csa{

class AnnotatedTokenSet{

public:
    typedef std::vector<CXToken>        TokenVector;
    typedef TokenVector::iterator       Iterator;
    typedef TokenVector::const_iterator ConstIterator;

public:
    AnnotatedTokenSet(const CXCursor& cursor, const CXTranslationUnit& tu);

    void append(const CXToken& token);

    bool operator ==(const AnnotatedTokenSet& other);
    bool operator !=(const AnnotatedTokenSet& other);

    const CXTranslationUnit& translationUnit();
    const CXCursor& cursor() const;
    unsigned int  size() const;
    Iterator      begin();
    Iterator      end();
    ConstIterator begin() const;
    ConstIterator end() const;
    CXToken       last() const;

    void dump(std::string &str);

private:
    CXCursor             m_cursor;
    CXTranslationUnit    m_translationUnit;
    std::vector<CXToken> m_tokens;

};

inline const CXCursor &AnnotatedTokenSet::cursor() const{
    return m_cursor;
}

inline unsigned int AnnotatedTokenSet::size() const{
    return (unsigned int)m_tokens.size();
}

inline AnnotatedTokenSet::Iterator AnnotatedTokenSet::begin(){
    return m_tokens.begin();
}

inline AnnotatedTokenSet::Iterator AnnotatedTokenSet::end(){
    return m_tokens.end();
}

inline AnnotatedTokenSet::ConstIterator AnnotatedTokenSet::begin() const{
    return m_tokens.begin();
}

inline AnnotatedTokenSet::ConstIterator AnnotatedTokenSet::end() const{
    return m_tokens.end();
}

inline CXToken AnnotatedTokenSet::last() const{
    return m_tokens.back();
}

}// namespace

#endif // ANNOTATEDTOKENSET_HPP
