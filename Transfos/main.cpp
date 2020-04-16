
#include <QApplication>
#include <QGLViewer/simple_viewer.h>
#include <matrices.h>
#include <primitives.h>


const Vec3 ROUGE   = {1,0,0};
const Vec3 VERT    = {0,1,0};
const Vec3 BLEU    = {0,0,1};
const Vec3 JAUNE   = {1,1,0};
const Vec3 CYAN    = {0,1,1};
const Vec3 MAGENTA = {1,0,1};
const Vec3 BLANC   = {1,1,1};
const Vec3 GRIS    = {0.5,0.5,0.5};
const Vec3 NOIR    = {0,0,0};

/// primitives(sphere/cube/cylindre/cone)
Primitives prim;

/// compteur (incremente dans animate)
int compteur = 0;




void draw_repere(const Mat4& tr)
{
}



void articulations(const Mat4& transfo)
{
}



int main(int argc, char *argv[])
{
	// init du viewer
	QApplication a(argc, argv);
	SimpleViewer::init_gl();
	// ouvre une fenetre QGLViewer avec fond noir et une scene de rayon 10
	SimpleViewer viewer(NOIR,10);

	// GL init
	viewer.f_init = [&] ()
	{
		prim.gl_init();
	};

	// drawing
	viewer.f_draw = [&] ()
	{
		prim.set_matrices(viewer.getCurrentModelViewMatrix(), viewer.getCurrentProjectionMatrix());

		prim.draw_sphere(Mat4(), BLANC);
		prim.draw_cube(translate(2,0,0), ROUGE);
		prim.draw_cone(translate(0,2,0), VERT);
		prim.draw_cylinder(translate(0,0,2), BLEU);

	//	draw_repere(Mat4());

	//	articulations(Mat4());
	};


	// to do when key pressed
	viewer.f_keyPress = [&] (int key, Qt::KeyboardModifiers /*mod*/)
	{
		switch(key)
		{
			case Qt::Key_A: // touche 'a'
				if (viewer.animationIsStarted())
					viewer.stopAnimation();
				else
					viewer.startAnimation();
				break;
			default:
				break;
		}
	};

	// to do every 50th of sec
	viewer.f_animate = [&] ()
	{
		compteur++;
		std::cout << "animate :" << compteur<< std::endl;
	};


	viewer.show();
	return a.exec();
}
