#include "qastsearch.h"
#include "qcsaconsole.h"
#include "qastnode.h"

namespace csa{

QASTSearch::QASTSearch(const QString& searchPattern)
    : m_lastMatchPosition(0)
{
    initSearchPattern(searchPattern);
}

QASTSearch::QASTSearch(const QASTSearch& other)
    : m_searchSegments(other.m_searchSegments)
    , m_lastMatchPosition(other.m_lastMatchPosition)
{
}

void QASTSearch::initSearchPattern(const QString &pattern){
    m_searchSegments.clear();
    m_isAbsolute = false;

    bool isEscapeFlag      = false;
    QString currentSegment = "";
    QString::const_iterator it = pattern.begin();
    if ( it == pattern.end() )
        return;
    if ( *it == QChar('/') ){
        ++it;
        m_isAbsolute = true;
    }

    while( it != pattern.end()){
        const QChar& c = *it;
        if ( c == QChar('\\') ){
            if (isEscapeFlag)
                currentSegment.append('\\');
            isEscapeFlag = !isEscapeFlag;
        } else {
            if ( c == QChar('/') ){
                if ( isEscapeFlag ){
                    currentSegment.append('/');
                } else {
                    m_searchSegments.append(SearchSegment(currentSegment));
                    currentSegment.clear();
                }
            } else {
                if ( isEscapeFlag )
                    currentSegment.append('\\');
                currentSegment.append(c);
            }
            isEscapeFlag = false;
        }

        ++it;
    }

    if ( !currentSegment.isEmpty() )
        m_searchSegments.append(SearchSegment(currentSegment));
}

QASTSearch::~QASTSearch(){
}

bool QASTSearch::isFirst() const{
    return m_lastMatchPosition == 0;
}

bool QASTSearch::isLast() const{
    return m_lastMatchPosition == m_searchSegments.size() - 1;
}

QASTSearch::MatchResult QASTSearch::matchCurrentSegment(ast::QASTNode *node) const{
    if ( m_searchSegments[m_lastMatchPosition].match(node) ){
        if ( m_lastMatchPosition == m_searchSegments.size() - 1 )
            return QASTSearch::FullMatch;
        else
            return QASTSearch::SegmentMatch;
    }
    return QASTSearch::NoMatch;
}

QASTSearch& QASTSearch::nextPosition(){
    if ( m_lastMatchPosition < m_searchSegments.size() - 1)
        ++m_lastMatchPosition;

    return *this;
}


bool QASTSearch::wildcardMatch(const QString& wildcard, const QString& matchData){
    QString::const_iterator expressionIt = wildcard.begin();
    QString::const_iterator matchIt      = matchData.begin();

    return matchHere(expressionIt, matchIt, wildcard.end(), matchData.end());

}

bool QASTSearch::isPattern(const QString& data){
    return data.indexOf('/') != -1 || data.indexOf('#') != -1;
}

QString QASTSearch::escape(const QString &text){
    return QString(text).replace('/', "\\/").replace(':', "\\:").replace('*', "\\*");
}

bool QASTSearch::matchHere(
        const QString::const_iterator& wildcardIt,
        const QString::const_iterator& matchDataIt,
        const QString::const_iterator& wildcardItEnd,
        const QString::const_iterator& matchDataItEnd)
{
    if ( wildcardIt == wildcardItEnd && matchDataIt == matchDataItEnd )
        return true;

    const QChar& segmentC   = *wildcardIt;
    const QChar& matchDataC = *matchDataIt;

    if ( segmentC == '*' ){

        if ( wildcardIt + 1 == wildcardItEnd )
            return true;

        for ( QString::const_iterator matchDataNextIt = matchDataIt;
              matchDataNextIt != matchDataItEnd;
              ++matchDataNextIt
        ){
            if ( matchHere(wildcardIt + 1, matchDataNextIt, wildcardItEnd, matchDataItEnd) )
                return true;
        }

        return false;

    } else if ( segmentC == matchDataC ){
        return matchHere(wildcardIt + 1, matchDataIt + 1, wildcardItEnd, matchDataItEnd);
    } else
        return false;
}

QASTSearch::SearchSegment::SearchSegment(const QString &searchPattern){
    QString* accumulator = &declaration;
    bool escapeFlag      = false;
    for ( QString::const_iterator it = searchPattern.begin(); it != searchPattern.end(); ++it ){
        const QChar& c = *it;
        if ( c == QChar('\\') ){
            if ( escapeFlag ){
                *accumulator += QChar('\\');
                escapeFlag = false;
            } else
                escapeFlag = true;
        } else if ( c == QChar('#') ){
            if ( escapeFlag ){
                *accumulator += c;
                escapeFlag = false;
            } else {
                if ( !identifier.isEmpty() ){
                    QCSAConsole::log(
                        QCSAConsole::Warning,
                        "Multiple \'#\' symbols defined in pattern \"" + searchPattern + "\". Search might not work."
                    );
                }
                identifier  = "";
                accumulator = &identifier;
            }
        } else if ( c == QChar(':') ){
            if ( escapeFlag ){
                *accumulator += c;
                escapeFlag = false;
            } else {
                accumulator = &property;
                if ( !property.isEmpty() ){
                    QCSAConsole::log(
                        QCSAConsole::Warning,
                        "Multiple \':\' symbols defined in pattern \"" + searchPattern + "\". Search might not work."
                    );
                }
                property    = "";
                accumulator = &property;
            }
        } else if ( c == QChar('[') ){
            if ( escapeFlag ){
                *accumulator += c;
                escapeFlag = false;
            } else {
                if ( !propertyValue.isEmpty() ){
                    QCSAConsole::log(
                        QCSAConsole::Warning,
                        "Multiple \'[\' symbols defined in pattern \"" + searchPattern + "\". Search might not work."
                    );
                }
                propertyValue = "";
                accumulator   = &propertyValue;
            }
        } else if ( c == QChar(']') ){
            if ( escapeFlag ){
                *accumulator += c;
                escapeFlag = false;
            } else {
                accumulator = &declaration;
            }
        } else {
            if ( escapeFlag ){
                *accumulator += QChar('\\');
                escapeFlag = false;
            }
            *accumulator += c;
        }
    }
}

bool QASTSearch::SearchSegment::match(ast::QASTNode *node) const{
    bool match = false;
    if ( !identifier.isEmpty() ){
        match = QASTSearch::wildcardMatch(identifier, node->identifier());
        if ( !match )
            return false;
    }
    if ( !declaration.isEmpty() ){
        match = QASTSearch::wildcardMatch(declaration, node->declaration());
        if ( !match )
            return false;
    }
    if ( !property.isEmpty() ){
        match = (node->prop(property) == propertyValue);
        if ( !match )
            return false;
    }
    return match;
}

}// namespace
