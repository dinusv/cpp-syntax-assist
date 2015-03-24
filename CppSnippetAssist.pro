
TEMPLATE = subdirs
CONFIG  += ordered

#include($$PWD/modules/modules.pro)

SUBDIRS += $$PWD/modules/modules.pro
SUBDIRS += $$PWD/tests/tests.pro

OTHER_FILES += \
    $$PWD/plugins/destructor.js \
    $$PWD/plugins/namespace.js \
    $$PWD/plugins/preventcopy.js \
    $$PWD/plugins/property.js

