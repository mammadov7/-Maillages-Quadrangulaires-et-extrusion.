#include <QApplication>
#include <QGLViewer/simple_viewer.h>
#include <matrices.h>
#include <OGLRender/grid2d.h>
#include <meshtri.h>
#include <polygon.h>


const Vec3 ROUGE   = {1,0,0};
const Vec3 VERT    = {0,1,0};
const Vec3 BLEU    = {0,0,1};
const Vec3 JAUNE   = {1,1,0};
const Vec3 CYAN    = {0,1,1};
const Vec3 MAGENTA = {1,0,1};
const Vec3 BLANC   = {1,1,1};
const Vec3 GRIS    = {0.5,0.5,0.5};
const Vec3 NOIR    = {0,0,0};

int main(int argc, char *argv[])
{
	// init du viewer
	QApplication a(argc, argv);
	SimpleViewer::init_gl();
	int render_mode = 0;

	// maillage de l'objet 3D
	MeshTri mesh;
	// polygone avec methode d'édition
	PolygonEditor polyg;

	// Fenetre de l'objet 3D
	SimpleViewer viewer3D(NOIR,2);

	// GL init
	viewer3D.f_init = [&] ()
	{
		mesh.gl_init();
	};

	// drawing
	viewer3D.f_draw = [&] ()
	{
		mesh.set_matrices(viewer3D.getCurrentModelViewMatrix(),viewer3D.getCurrentProjectionMatrix());
		if ((render_mode==0) && mesh.has_lines())
			mesh.draw_lines(JAUNE);
		if ((render_mode==1) && mesh.has_faces())
			mesh.draw_flat(ROUGE);
		if ((render_mode==2) && mesh.has_faces() && mesh.has_normals())
			mesh.draw_smooth(ROUGE);
	};


	// to do when key pressed
	viewer3D.f_keyPress = [&] (int key, Qt::KeyboardModifiers /*mod*/)
	{
		switch(key)
		{
			case Qt::Key_F:
				glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
				break;
			case Qt::Key_G:
				glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
				break;
			case Qt::Key_P:
				mesh.create_pyramide();
				break;
			case Qt::Key_A:
				mesh.anneau();
				break;
			case Qt::Key_S:
				mesh.spirale();
				break;
			case Qt::Key_R:
				mesh.revolution(polyg.vertices());
				break;
			case Qt::Key_M:
				render_mode = (render_mode+1)%3;
				break;
			default: break;
		}
		viewer3D.update();
	};

	viewer3D.clearShortcuts();
	viewer3D.setShortcut(QGLViewer::EXIT_VIEWER,Qt::Key_Y);

	// Fenetre de saisie du polygone
	SimpleViewer viewer2D({0,0,0},1);
	// croix
	Grid2d grid;

	viewer2D.f_init = [&] ()
	{
		glDisable(GL_DEPTH_TEST);
		grid.gl_init();
		polyg.gl_init();
	};

	// drawing
	viewer2D.f_draw = [&] ()
	{
		grid.draw(GRIS);
		polyg.draw(JAUNE);
	};

	// x,y coordonnées souris [-1, 1]
	viewer2D.f_mousePress = [&] (Qt::MouseButton b, float x, float y)
	{
		switch(b)
		{
			case Qt::LeftButton:
				polyg.add_vertex(x,y);
				break;
			case Qt::RightButton:
				polyg.remove_last();
				break;
			default: break;
		}
	};

	viewer2D.f_keyPress = [&] (int key, Qt::KeyboardModifiers /*mod*/)
	{
		switch(key)
		{
			case Qt::Key_L: // touche 'l'
				polyg.lisse();
				break;
			case Qt::Key_C: // touche 'c'
				polyg.clear();
				break;
			default:
				break;
		}
		viewer2D.update();
	};

	viewer2D.setMinimumSize(500,500);
	viewer2D.setMaximumSize(500,500);
	viewer2D.setWindowTitle("Revolution: Objet 3D");
	viewer2D.show();
	viewer3D.setGeometry(10,10,800,600);
	viewer3D.setWindowTitle("Revolution: Polygone");
	viewer3D.show();

	return a.exec();
}


