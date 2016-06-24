#ifndef QASTSEARCH_HPP
#define QASTSEARCH_HPP

#include "qcsaglobal.h"
#include <QList>

namespace csa{

namespace ast{
class QASTNode;
}

class Q_CSA_EXPORT QASTSearch{

public:
    class SearchSegment{
    public:
        SearchSegment(const QString& searchPattern);
        bool match(ast::QASTNode* node) const;

        QString identifier;
        QString declaration;
        QString property;
        QString propertyValue;
    };

    enum MatchResult{
        FullMatch = 0,
        SegmentMatch,
        NoMatch
    };

public:
    explicit QASTSearch(const QString& searchPattern);
    QASTSearch(const QASTSearch& other);
    ~QASTSearch();

    bool isFirst() const;
    bool isLast() const;

    MatchResult matchCurrentSegment(ast::QASTNode* node) const;
    QASTSearch& nextPosition();

    static bool wildcardMatch(const QString& wildcard, const QString& matchData);
    static bool isPattern(const QString& data);

private:
    void initSearchPattern(const QString& pattern);

    static bool matchHere(
        const QString::const_iterator& segmentIt,
        const QString::const_iterator& matchDataIt,
        const QString::const_iterator& segmentItEnd,
        const QString::const_iterator& matchDataItEnd
    );

    QList<SearchSegment> m_searchSegments;
    int                  m_lastMatchPosition;

};

}// namespace

#endif // QASTSEARCH_HPP
