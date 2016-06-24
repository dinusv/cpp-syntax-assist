#include "qastsearchtest.h"
#include "qastsearch.h"
#include "qastnode.h"

#include <QtTest/QtTest>

using namespace csa;
using namespace csa::ast;

// Helpers
// -------

namespace helpers{

class QASTNodeStub : public QASTNode{

public:
    QASTNodeStub(const QString& identifier, const QString& type, QASTNode* parent = 0);
    ~QASTNodeStub();
};

QASTNodeStub::QASTNodeStub(const QString& identifier, const QString& type, QASTNodeStub::QASTNode* parent)
    : QASTNode(type, 0, 0, 0, 0, parent)
{
    setIdentifier(identifier);
}

QASTNodeStub::~QASTNodeStub(){
}

int nodeSearch(QASTNode* root, const QString& pattern, const QString& type = "", const QString& identifier = ""){
    QList<QObject*> foundNodes = root->find(pattern, type);
    if ( identifier.isEmpty())
        return foundNodes.size();

    unsigned int nrNodes = 0;
    for ( QList<QObject*>::const_iterator it = foundNodes.begin(); it != foundNodes.end(); ++it ){
        QASTNode* node = qobject_cast<QASTNode*>(*it);
        if ( node )
            if ( node->identifier() == identifier )
                ++nrNodes;
    }
    return nrNodes;
}

} // namespace


using namespace helpers;

// QASTSearch Implementation
// -------------------------

Q_TEST_RUNNER_REGISTER(QASTSearchTest);

QASTSearchTest::QASTSearchTest(QObject *parent)
    : QObject(parent)
{
}

QASTSearchTest::~QASTSearchTest(){
}

void QASTSearchTest::initTestCase(){
}

void QASTSearchTest::segmentMatchTest(){
    QCOMPARE(QASTSearch::wildcardMatch("abc", "abc"), true);
    QCOMPARE(QASTSearch::wildcardMatch("abc2", "abc"), false);
    QCOMPARE(QASTSearch::wildcardMatch("abc23", "abc23"), true);
    QCOMPARE(QASTSearch::wildcardMatch("abc*", "abc23"), true);
    QCOMPARE(QASTSearch::wildcardMatch("abc*3", "abc23"), true);
    QCOMPARE(QASTSearch::wildcardMatch("*3", "abc23"), true);
    QCOMPARE(QASTSearch::wildcardMatch("*34", "abc23"), false);
    QCOMPARE(QASTSearch::wildcardMatch("2*23", "abc23"), false);
    QCOMPARE(QASTSearch::wildcardMatch("*", "abc23"), true);
    QCOMPARE(QASTSearch::wildcardMatch("", "abc23"), false);
    QCOMPARE(QASTSearch::wildcardMatch("*abc", "abc"), true);
    QCOMPARE(QASTSearch::wildcardMatch("*", ""), true);
    QCOMPARE(QASTSearch::wildcardMatch("*", "*"), true);
    QCOMPARE(QASTSearch::wildcardMatch("", "*"), false);
}

void QASTSearchTest::nodeSearchTest(){
    QASTNodeStub* node1 = new QASTNodeStub("Abc", "class", 0);
    QASTNodeStub* node2 = new QASTNodeStub("AbcChild", "method", node1);

    QList<QASTNodeStub*> leafNodes;
    leafNodes.append(new QASTNodeStub("AbcChildChild", "method", node2));
    leafNodes.append(new QASTNodeStub("AbcChildChild2", "method", node2));

    // ASTSearch Enabling
    // ------------------

    QCOMPARE(helpers::nodeSearch(node1, "Ab*", "class", "Abc"), 0);
    QCOMPARE(helpers::nodeSearch(node1, "Ab*/", "class", "Abc"), 1);

    // AST Tree Search
    // ---------------

    QCOMPARE(helpers::nodeSearch(node1, "AbcChild", "", "AbcChild"), 1);
    QCOMPARE(helpers::nodeSearch(node1, "AbcChild/AbcChild*", "", ""), 2);
    QCOMPARE(helpers::nodeSearch(node1, "AbcChild/AbcChild*", "", "AbcChildChild"), 1);
    QCOMPARE(helpers::nodeSearch(node1, "AbcChild/AbcChildChild*", "method", "AbcChildChild"), 1);
}
