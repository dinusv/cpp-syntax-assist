# Copy tests to release path
# --------------------------

TEST_DATA_DEPLOY_FROM = $$PWD/data

win32:CONFIG(debug, debug|release): TEST_DATA_DEPLOY_TO = $$OUT_PWD/../build/core
else:win32:CONFIG(release, debug|release): TEST_DATA_DEPLOY_TO = $$OUT_PWD/../build/core
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
INCLUDEPATH += $$PWD/../../modules/csa-lib/codebase
INCLUDEPATH += $$PWD/../../modules/csa-lib/script
INCLUDEPATH += $$PWD/../../modules/csa-lib/testing
DEPENDPATH  += $$PWD/../../modules/csa-lib/astnodes
DEPENDPATH  += $$PWD/../../modules/csa-lib/codebase
DEPENDPATH  += $$PWD/../../modules/csa-lib/script
DEPENDPATH  += $$PWD/../../modules/csa-lib/testing

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../../modules/csa-lib/release/ -lcsa
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../../modules/csa-lib/debug/ -lcsa
else:unix: LIBS += -L$$OUT_PWD/../../modules/build/ -lcsa

include($$PWD/src/integrationtests.pri)
include($$PWD/../../3rdparty/libclang.pro)

## Setup application
## -----------------

TARGET   = csa-test-integration
TEMPLATE = app

win32:CONFIG(release, debug|release): DESTDIR = $$OUT_PWD/../build
else:win32:CONFIG(debug, debug|release): DESTDIR = $$OUT_PWD/../build
else:unix: DESTDIR = $$OUT_PWD/../build

OTHER_FILES += \
    $$PWD/data/csa-assertions-test.js \
    $$PWD/data/csa-before-after-scenario-test.js \
    $$PWD/data/csa-invalid-params-test.js

DISTFILES += \
    data/csa-restore-test.js \
    data/csa-restore-test.in
