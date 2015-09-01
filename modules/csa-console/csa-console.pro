TEMPLATE = app
TARGET   = csa
CONFIG += console
CONFIG -= app_bundle
QT     += qml quick script

# Configure Dependencies
# ----------------------

INCLUDEPATH += $$PWD/../csa-lib/astnodes
INCLUDEPATH += $$PWD/../csa-lib/codebase
INCLUDEPATH += $$PWD/../csa-lib/script
DEPENDPATH  += $$PWD/../csa-lib/astnodes
DEPENDPATH  += $$PWD/../csa-lib/codebase
DEPENDPATH  += $$PWD/../csa-lib/script

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../csa-lib/release/ -lcsa
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../csa-lib/debug/ -lcsa
else:unix: LIBS += -L$$OUT_PWD/../build/ -lcsa

# Add source files
# ----------------

include(linenoise/cpplinenoise.pri)
include(src/csa-console.pri)
include($$PWD/../../3rdparty/libclang.pro)

# Add Deployment
# --------------

win32:CONFIG(release, debug|release): DESTDIR = $$OUT_PWD/../build/release
else:win32:CONFIG(debug, debug|release): DESTDIR = $$OUT_PWD/../build/debug
else:unix: DESTDIR = $$OUT_PWD/../build

include(deployment.pri)
qtcAddDeployment()

