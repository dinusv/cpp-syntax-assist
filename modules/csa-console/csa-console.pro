TEMPLATE = app
TARGET   = csa
CONFIG += console
CONFIG -= app_bundle
QT     += qml quick script

# Configure Dependencies
# ----------------------

INCLUDEPATH += $$PWD/../csa/astnodes
INCLUDEPATH += $$PWD/../csa/codebase
INCLUDEPATH += $$PWD/../csa/script
DEPENDPATH  += $$PWD/../csa/astnodes
DEPENDPATH  += $$PWD/../csa/codebase
DEPENDPATH  += $$PWD/../csa/script

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../csa/release/ -lcsa
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../csa/debug/ -lcsa
else:unix: LIBS += -L$$OUT_PWD/../build/ -lcsa

# Add source files
# ----------------

include(linenoise/cpplinenoise.pri)
include(src/csa-console.pri)

# Add Deployment
# --------------

win32:CONFIG(release, debug|release): DESTDIR = $$OUT_PWD/../build/release
else:win32:CONFIG(debug, debug|release): DESTDIR = $$OUT_PWD/../build/debug
else:unix: DESTDIR = $$OUT_PWD/../build

include(deployment.pri)
qtcAddDeployment()

