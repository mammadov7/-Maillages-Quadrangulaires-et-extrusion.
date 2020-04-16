TEMPLATE = subdirs

SUBDIRS = QGLViewer OGLRender Transfos Revolution Projet_modeling

 # what subproject depends on others
Transfos.depends = QGLViewer OGLRender
Revolution.depends = QGLViewer OGLRender
Projet_modeling.depends = QGLViewer OGLRender
