# Copy tests to release path
# --------------------------

TEST_DATA_DEPLOY_FROM = $$PWD/coretests

win32:CONFIG(debug, debug|release): TEST_DATA_DEPLOY_TO = $$OUT_PWD/../build/coretests
else:win32:CONFIG(release, debug|release): TEST_DATA_DEPLOY_TO = $$OUT_PWD/../build/coretests
else:unix: TEST_DATA_DEPLOY_TO = $$OUT_PWD

win32:TEST_DATA_DEPLOY_TO ~= s,/,\\,g
win32:TEST_DATA_DEPLOY_FROM ~= s,/,\\,g

testdatacopy.commands = $(COPY_DIR) \"$$TEST_DATA_DEPLOY_FROM\" \"$$TEST_DATA_DEPLOY_TO\"
first.depends = $(first) testdatacopy
export(first.depends)
export(testdatacopy.commands)
QMAKE_EXTRA_TARGETS += first testdatacopy


# Enable javascript and console output
# ------------------------------------

QT     += qml quick script testlib
CONFIG += console testcase

## Configure Dependencies
## ----------------------

INCLUDEPATH += $$PWD/../../modules/csa-lib/astnodes
DEPENDPATH  += $$PWD/../../modules/csa-lib/astnodes
INCLUDEPATH += $$PWD/../../modules/csa-lib/codebase
DEPENDPATH  += $$PWD/../../modules/csa-lib/codebase
INCLUDEPATH += $$PWD/../../modules/csa-lib/script
DEPENDPATH  += $$PWD/../../modules/csa-lib/script
INCLUDEPATH += $$PWD/../../modules/csa-lib/testing
DEPENDPATH  += $$PWD/../../modules/csa-lib/testing

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../../modules/csa-lib/release/ -lcsa
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../../modules/csa-lib/debug/ -lcsa
else:unix: LIBS += -L$$OUT_PWD/../../modules/build/ -lcsa

include($$PWD/src/functionaltests.pri)
include($$PWD/../../3rdparty/libclang.pro)

TARGET   = csa-test-script
TEMPLATE = app

win32:CONFIG(release, debug|release): DESTDIR = $$OUT_PWD/../build
else:win32:CONFIG(debug, debug|release): DESTDIR = $$OUT_PWD/../build
else:unix: DESTDIR = $$OUT_PWD/../build

DISTFILES += \
    $$PWD/coretests/astfile-test.js \
    $$PWD/coretests/astnode-test.js \
    $$PWD/coretests/nodecollection-test.js \
    $$PWD/coretests/token-test.js \
    $$PWD/coretests/config-test.js \
    $$PWD/coretests/codebase-test.js \
    $$PWD/coretests/codebase-test.in \
    $$PWD/coretests/codebase-test.insrc \
    $$PWD/coretests/astnode-test.in \
    $$PWD/coretests/nodecollection-test.in \
    $$PWD/coretests/token-test.in \
    $$PWD/coretests/require-test.js \
    $$PWD/coretests/require-in.js \
    $$PWD/coretests/require-loop-in.js \
    $$PWD/coretests/require-loop-multi-1-in.js \
    $$PWD/coretests/require-loop-multi-2-in.js
