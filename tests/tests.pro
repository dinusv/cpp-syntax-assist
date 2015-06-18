# Enable javascript and console output
# ------------------------------------

QT     += qml quick script testlib
QT     -= gui
CONFIG += console testcase

# Configure Dependencies
# ----------------------

INCLUDEPATH += $$PWD/../modules/csa/astnodes
INCLUDEPATH += $$PWD/../modules/csa/codebase
INCLUDEPATH += $$PWD/../modules/csa/script
DEPENDPATH  += $$PWD/../modules/csa/astnodes
DEPENDPATH  += $$PWD/../modules/csa/codebase
DEPENDPATH  += $$PWD/../modules/csa/script

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../modules/csa/release/ -lcsa
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../modules/csa/debug/ -lcsa
else:unix: LIBS += -L$$OUT_PWD/../modules/application/ -lcsa

include($$PWD/src/tests.pri)

TARGET   = test_csa
TEMPLATE = app
