#include "qastsearch.h"

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

    bool isEscapeFlag      = false;
    QString currentSegment = "";
    for ( QString::const_iterator it = pattern.begin(); it != pattern.end(); ++it ){
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
                    m_searchSegments.append(currentSegment);
                    currentSegment.clear();
                }
            } else {
                currentSegment.append(c);
            }
            isEscapeFlag = false;
        }
    }

    if ( !currentSegment.isEmpty() )
        m_searchSegments.append(currentSegment);

    if ( !m_searchSegments.isEmpty() )
        if ( m_searchSegments.last().isEmpty() && m_searchSegments.size() > 1 )
            m_searchSegments.removeLast();
}

QASTSearch::~QASTSearch(){
}

bool QASTSearch::isFirst() const{
    return m_lastMatchPosition == 0;
}

bool QASTSearch::isLast() const{
    return m_lastMatchPosition == m_searchSegments.size() - 1;
}

QASTSearch::MatchResult QASTSearch::matchCurrentSegment(const QString& matchData) const{
    if ( matchSegment(m_searchSegments[m_lastMatchPosition], matchData) ){
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


bool QASTSearch::matchSegment(const QString& segment, const QString& matchData){
    QString::const_iterator expressionIt = segment.begin();
    QString::const_iterator matchIt      = matchData.begin();

    return matchHere(expressionIt, matchIt, segment.end(), matchData.end());

}

bool QASTSearch::isPattern(const QString& data){
    return data.indexOf('/') != -1;
}

bool QASTSearch::matchHere(
        const QString::const_iterator& segmentIt,
        const QString::const_iterator& matchDataIt,
        const QString::const_iterator& segmentItEnd,
        const QString::const_iterator& matchDataItEnd)
{
    if ( segmentIt == segmentItEnd && matchDataIt == matchDataItEnd )
        return true;

    const QChar& segmentC   = *segmentIt;
    const QChar& matchDataC = *matchDataIt;

    if ( segmentC == '*' ){

        if ( segmentIt + 1 == segmentItEnd )
            return true;

        for ( QString::const_iterator matchDataNextIt = matchDataIt;
              matchDataNextIt != matchDataItEnd;
              ++matchDataNextIt
        ){
            if ( matchHere(segmentIt + 1, matchDataNextIt, segmentItEnd, matchDataItEnd) )
                return true;
        }

        return false;

    } else if ( segmentC == matchDataC ){
        return matchHere(segmentIt + 1, matchDataIt + 1, segmentItEnd, matchDataItEnd);
    } else
        return false;
}

}// namespace
