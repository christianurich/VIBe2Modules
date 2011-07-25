TEMPLATE = lib
CONFIG += dll
QT -= gui
SOURCES += modules.cpp \
    examplemodule.cpp \
    testmodule.cpp \
    ../core/rasterdata.cpp \
    ../core/moduleregistry.cpp \
    ../core/module.cpp \
    emdata.cpp \
    marker.cpp \
    muparser/src/muParserTokenReader.cpp \
    muparser/src/muParserTest.cpp \
    muparser/src/muParserInt.cpp \
    muparser/src/muParserError.cpp \
    muparser/src/muParserDLL.cpp \
    muparser/src/muParserComplex.cpp \
    muparser/src/muParserCallback.cpp \
    muparser/src/muParserBytecode.cpp \
    muparser/src/muParserBase.cpp \
    muparser/src/muParser.cpp \
    ../core/vectordata.cpp \
    cellularautomata.cpp \
    copyrastermap.cpp \
    randommarks.cpp \
    randompoints.cpp \
    userdefinedfunctions.cpp \
    exportrasterdata.cpp \
    simpleagentmodel.cpp \
    createemptyrastermap.cpp \
    simpleagent.cpp \
    simpleagentrule.cpp \
    importrasterdata.cpp \
    importdxf.cpp
HEADERS += examplemodule.h \
    testmodule.h \
    emdata.h \
    marker.h \
    muparser/include/muParserTokenReader.h \
    muparser/include/muParserToken.h \
    muparser/include/muParserTest.h \
    muparser/include/muParserStack.h \
    muparser/include/muParserInt.h \
    muparser/include/muParserFixes.h \
    muparser/include/muParserError.h \
    muparser/include/muParserDLL.h \
    muparser/include/muParserDef.h \
    muparser/include/muParserComplex.h \
    muparser/include/muParserCallback.h \
    muparser/include/muParserBytecode.h \
    muparser/include/muParserBase.h \
    muparser/include/muParser.h \
    ../core/vectordata.h \
    cellularautomata.h \
    copyrastermap.h \
    randommarks.h \
    randompoints.h \
    userdefinedfunctions.h \
    exportrasterdata.h \
    simpleagentmodel.h \
    createemptyrastermap.h \
    simpleagent.h \
    simpleagentrule.h \
    importrasterdata.h \
    importdxf.h
LIBS += -lgomp
INCLUDEPATH += ../core/
INCLUDEPATH += muparser/include
OTHER_FILES += anttarilmodel
