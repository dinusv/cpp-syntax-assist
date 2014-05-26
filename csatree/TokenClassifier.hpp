#ifndef CURSORTOKENBASE_HPP
#define CURSORTOKENBASE_HPP

#include "clang-c/Index.h"
#include <vector>
#include <string>

namespace csa{

class AnnotatedTokenSet;
class TokenClassifier{

public:
    typedef std::vector<AnnotatedTokenSet*> TokenSetVector;
    typedef std::vector<AnnotatedTokenSet*>::iterator Iterator;
    typedef std::vector<AnnotatedTokenSet*>::const_iterator ConstIterator;

public:
    TokenClassifier(const CXTranslationUnit& transUnit, const char* fileName );
    ~TokenClassifier();

    AnnotatedTokenSet* findTokenSet(const CXCursor& cursor);

    Iterator tokenSetBegin();
    Iterator tokenSetEnd();
    ConstIterator tokenSetBegin() const;
    ConstIterator tokenSetEnd() const;

    void reparse();

    const CXTranslationUnit& translatinoUnit() const;
    const std::string& file() const;

    void dump(std::string& str);

    static unsigned int  getFileSize(const char* file);
    static CXSourceRange getFileRange(const CXTranslationUnit& tu, const char* file);

private:
    void initialize(const CXTranslationUnit& transUnit, const CXSourceRange& range);

    TokenSetVector    m_tokenSets;
    CXTranslationUnit m_translationUnit;
    CXToken*          m_tokens;
    unsigned int      m_tokensCount;

    std::string       m_file;

};

inline TokenClassifier::Iterator TokenClassifier::tokenSetBegin(){
    return m_tokenSets.begin();
}

inline TokenClassifier::Iterator TokenClassifier::tokenSetEnd(){
    return m_tokenSets.end();
}

inline TokenClassifier::ConstIterator TokenClassifier::tokenSetBegin() const{
    return m_tokenSets.begin();
}

inline TokenClassifier::ConstIterator TokenClassifier::tokenSetEnd() const{
    return m_tokenSets.end();
}

inline const CXTranslationUnit &TokenClassifier::translatinoUnit() const{
    return m_translationUnit;
}

inline const std::string &TokenClassifier::file() const{
    return m_file;
}


/*

      TokenClassifier tclassif = new TokenClassifier(transUnit, fileName);
      TokenClassifier tclassif = new TokenClassifier(transUnit, range);

      AnnotatedTokenSet* ats = tclassif->findTokenSet(const CXCursor& cursor);
      if ( ats )
        CXSourceLocation cxsl = ats->find('{', Token::Punctuation);
        if ( cxsl != null )
            SourceLocation sl(cxsl);


      TODO : Add position from offset

     */

}// namespace

#endif // CURSORTOKENBASE_HPP
