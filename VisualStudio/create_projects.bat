
mkdir OGLRender
cd OGLRender
qmake -spec win32-msvc2015 -tp vc ..\..\OGLRender
cd ..

mkdir QGLViewer
cd QGLViewer
qmake -spec win32-msvc2015 -tp vc ..\..\QGLViewer
cd ..

mkdir Transfos
cd Transfos
qmake -spec win32-msvc2015 -tp vc ..\..\Transfos
cd ..

mkdir Revolution
cd Revolution
qmake -spec win32-msvc2015 -tp vc ..\..\Revolution
cd ..

mkdir Projet_modeling
cd Projet_modeling
qmake -spec win32-msvc2015 -tp vc ..\..\Projet_modeling
cd ..


qmake -spec win32-msvc2015 -tp vc ..\

