#ifndef QASTSEARCH_HPP
#define QASTSEARCH_HPP

#include "qcsaglobal.h"
#include <QStringList>

namespace csa{

class Q_CSA_EXPORT QASTSearch{

public:
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

    MatchResult matchCurrentSegment(const QString& matchData) const;
    QASTSearch& nextPosition();

    static bool matchSegment(const QString& segment, const QString& matchData);
    static bool isPattern(const QString& data);

private:
    void initSearchPattern(const QString& pattern);

    static bool matchHere(
        const QString::const_iterator& segmentIt,
        const QString::const_iterator& matchDataIt,
        const QString::const_iterator& segmentItEnd,
        const QString::const_iterator& matchDataItEnd
    );

    QStringList m_searchSegments;
    int         m_lastMatchPosition;

};

}// namespace

#endif // QASTSEARCH_HPP
