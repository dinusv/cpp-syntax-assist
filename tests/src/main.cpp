#include <QCoreApplication>
#include <QTest>

#include "QTestRunner.hpp"
#include "QASTParsingTest.hpp"

int main(int argc, char *argv[])
{
    QCoreApplication app(argc, argv);
    app.setAttribute(Qt::AA_Use96Dpi, true);

    return QTestRunner::runTests(argc, argv);
}
