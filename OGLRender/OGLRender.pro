
QT += core gui opengl xml widgets
TARGET = OGLRender
TEMPLATE = lib
CONFIG += dynamiclib shared create_prl


# include path for glm
INCLUDEPATH += ..

DESTDIR =$$_PRO_FILE_PWD_/../bin

# define path for shaders
QMAKE_CXXFLAGS += -std=c++11 -DSHADERPATH=$$_PRO_FILE_PWD_ -DOGLRENDER_DLL_EXPORT -DGLEW_BUILD
QMAKE_CFLAGS += -DGLEW_BUILD

macx {
QMAKE_LFLAGS_SONAME += -Wl,-install_name,@rpath/lib$${TARGET}.dylib
}

win32 {
CONFIG += embed_manifest_dll
LIBS += -lopengl32
}

SOURCES += shader.cpp shaderprogram.cpp shaderprogramcolor.cpp shaderprogramflat.cpp shaderprogramphong.cpp glew.c \
    grid2d.cpp

HEADERS  += shaderprogram.h shader.h shaderprogramcolor.h shaderprogramflat.h shaderprogramphong.h \
    grid2d.h

DISTFILES += colorshader.vert colorshader.frag flatshader.vert flatshader.frag phongshader.vert phongshader.frag
