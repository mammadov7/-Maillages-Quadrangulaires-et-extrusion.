#		    l i b Q G L V i e w e r
#	C o m p i l a t i o n    c o n f i g u r a t i o n

# Run "qmake; make; make install" to compile and install the library on Unix systems.
# Optional arguments can tune install paths (as in "qmake PREFIX=$HOME"). See doc/download.html for details.

TEMPLATE = lib
TARGET = QGLViewer33
CONFIG *= qt opengl warn_on shared thread create_prl rtti no_keywords

QGL_HEADERS = qglviewer.h \
	  camera.h \
	  manipulatedFrame.h \
	  manipulatedCameraFrame.h \
	  frame.h \
	  constraint.h \
	  keyFrameInterpolator.h \
	  mouseGrabber.h \
	  quaternion.h \
	  vec.h \
	  domUtils.h \
      config.h \
      simple_viewer.h

SOURCES = qglviewer.cpp \
	  camera.cpp \
	  manipulatedFrame.cpp \
	  manipulatedCameraFrame.cpp \
	  frame.cpp \
	  saveSnapshot.cpp \
	  constraint.cpp \
	  keyFrameInterpolator.cpp \
	  mouseGrabber.cpp \
	  quaternion.cpp \
	  vec.cpp

HEADERS *= $${QGL_HEADERS}
DISTFILES *= qglviewer-icon.xpm
DESTDIR =$$_PRO_FILE_PWD_/../bin

TRANSLATIONS = qglviewer_fr.ts

QT *= xml opengl

contains ( $$[QT_VERSION], "^5.*" ) {
	QT *= gui widgets
}

!isEmpty( QGLVIEWER_STATIC ) {
  CONFIG *= staticlib
}

FORMS *= ImageInterface.ui

DEFINES *= NO_VECTORIAL_RENDER

# ---------------
# --  U n i x  --
# ---------------
unix {
	CONFIG -= debug debug_and_release
	CONFIG *= release
        QMAKE_CXXFLAGS += -std=c++11
	macx|darwin-g++ {
		# GLU is part of the OpenGL framework
	} else {
		QMAKE_LIBS_OPENGL *= -lGLU
	}

	MOC_DIR = compil_moc
	OBJECTS_DIR = compil_obj

	# Make much smaller libraries (and packages) by removing debugging informations
	QMAKE_CFLAGS_RELEASE -= -g
	QMAKE_CXXFLAGS_RELEASE -= -g
}


# -------------------
# --  M a c O S X  --
# -------------------
macx|darwin-g++ {

	QMAKE_LFLAGS_SONAME += -Wl,-install_name,@rpath/lib$${TARGET}.dylib
	include.files *= qglviewer.icns
}


# ---------------------
# --  W i n d o w s  --
# ---------------------
win32 {
	# Windows requires a debug lib version to link against debug applications
#	CONFIG *= debug_and_release build_all

	# Needed by Intel C++, (icl.exe) so that WINGDIAPI is a defined symbol in gl.h.
	DEFINES *= WIN32

	staticlib {
		DEFINES *= QGLVIEWER_STATIC
	} else {
		DEFINES *= CREATE_QGLVIEWER_DLL
	}

	CONFIG *= embed_manifest_dll

	# Use native OpenGL drivers with Qt5.5
	# No longer implicit since the ANGLE driver is now an alternative
	LIBS += -lopengl32 -lglu32

	# TP : C++ source code
	# GR : Enables run-time type information (RTTI).
	# Zi : Generates complete debugging information (removed)
	# EHs : The exception-handling model that catches C++ exceptions only and tells the
	#       compiler to assume that functions declared as extern "C" may throw an exception.
	# FS : Enable parallel compilation
	# Any feedback on these flags is welcome.
	!win32-g++ {
		QMAKE_CXXFLAGS *= -TP -GR
		DEFINES += NOMINMAX
		win32-msvc {
			QMAKE_CXXFLAGS *= -EH -FS
		} else {
			QMAKE_CXXFLAGS *= -EHs
		}
	}
}
