
#include <QApplication>
#include <QGLViewer/simple_viewer.h>
#include <matrices.h>
#include <primitives.h>
#include <meshquad.h>

const Vec3 ROUGE   = {1,0,0};
const Vec3 VERT    = {0,1,0};
const Vec3 BLEU    = {0,0,1};
const Vec3 JAUNE   = {1,1,0};
const Vec3 CYAN    = {0,1,1};
const Vec3 MAGENTA = {1,0,1};
const Vec3 BLANC   = {1,1,1};
const Vec3 GRIS    = {0.5,0.5,0.5};
const Vec3 NOIR    = {0,0,0};



void draw_repere(const Primitives& prim, const Mat4& tr)
{
    prim.draw_sphere(tr, BLANC);
    prim.draw_cone(tr*translate(0,0,3), BLEU);
    prim.draw_cylinder(tr*scale(0.5,0.5,3)*translate(0,0,0.5), BLEU);
    prim.draw_cone(tr*translate(0,3,0)*rotateX(-90), VERT);
    prim.draw_cylinder(tr*rotateX(90)*scale(0.5,0.5,3)*translate(0,0,-0.5), VERT);
    prim.draw_cone(tr*translate(3,0,0)*rotateY(90), ROUGE);
    prim.draw_cylinder(tr*rotateY(90)*scale(0.5,0.5,3)*translate(0,0,0.5), ROUGE);
}


void star(MeshQuad& m)
{
    m.create_cube();
    for ( int i = 0; i < 20; i++ ){
        for( int j = 0; j < 6; j++ ){
            m.extrude_quad(j);
            m.shrink_quad(j,1.1);
            m.tourne_quad(j,10);
        }
    }
}

void spiral( MeshQuad& m )
{
    m.create_cube();
    for( int i=0; i <300; i++ ){
        m.transfo_quad(2, rotateX( 11 )*translate(0.11,0,0)*scale(0.99));
        m.extrude_quad(2);
    }
}

int rec_wheel( MeshQuad& m, int n )
{
    m.extrude_quad(2);
    int nb = m.nb_quads();
    for(int i = 0; i< 20; i++){
        m.extrude_quad(nb-2);
        m.transfo_quad(nb-2, rotateY( 18 ));
    }
    m.transfo_quad(2, rotateX( 10 ));
    if( n == 0 ) return 0;
    return rec_wheel(m,n-1);
}

int rec1( MeshQuad& m, int n)
{
    m.extrude_quad(n);
    int nb = m.nb_quads();
    m.extrude_quad(nb-1);
    m.shrink_quad(nb-1,1.3);
    m.extrude_quad(nb-1);
    m.decale_quad(nb-1,2);

    m.extrude_quad(nb-2);
    m.shrink_quad(nb-2,1.3);
    m.extrude_quad(nb-2);
    m.decale_quad(nb-2,2);

    m.extrude_quad(nb-3);
    m.shrink_quad(nb-3,1.3);
    m.extrude_quad(nb-3);
    m.decale_quad(nb-3,2);

    m.extrude_quad(nb-4);
    m.shrink_quad(nb-4,1.3);
    m.extrude_quad(nb-4);
    m.decale_quad(nb-4,2);

    if(nb > 500) return 0;
    rec1(m,nb-1);
    rec1(m,nb-2);
    rec1(m,nb-3);
    rec1(m,nb-4);
}

int main(int argc, char *argv[])
{
	Primitives prim;
	int selected_quad = -1;
	glm::mat4 selected_frame;
	MeshQuad mesh;

	// init du viewer
	QApplication a(argc, argv);
	SimpleViewer::init_gl();
	SimpleViewer viewer({0.1,0.1,0.1},5);

	// GL init
	viewer.f_init = [&] ()
	{
		prim.gl_init();
		mesh.gl_init();
	};

	// drawing
	viewer.f_draw = [&] ()
	{
		mesh.set_matrices(viewer.getCurrentModelViewMatrix(),viewer.getCurrentProjectionMatrix());
		prim.set_matrices(viewer.getCurrentModelViewMatrix(),viewer.getCurrentProjectionMatrix());

		mesh.draw(CYAN);

		if (selected_quad>=0)
			draw_repere(prim,selected_frame);
	};

	// to do when key pressed
	viewer.f_keyPress = [&] (int key, Qt::KeyboardModifiers mod)
	{
		switch(key)
		{
			case Qt::Key_C:
                if (!(mod & Qt::ControlModifier))
					mesh.create_cube();
				break;
            // e extrusion
            case Qt::Key_E:
            if (!(mod & Qt::ControlModifier))
                if(  selected_quad != -1 )
                    mesh.extrude_quad(selected_quad);
            break;

            // + decale
            case Qt::Key_Plus:
                if (!(mod & Qt::ControlModifier))
                    if(  selected_quad != -1 )
                        mesh.decale_quad(selected_quad,1);
            break;

            // - decale
            case Qt::Key_Minus:
                printf("here\n\n");
                if (!(mod & Qt::ControlModifier))
                    if(  selected_quad != -1 )
                        mesh.decale_quad(selected_quad,-1);
            break;

            // z/Z shrink
            case Qt::Key_Z:
                if (!(mod & Qt::ControlModifier))
                    if(  selected_quad != -1 )
                        mesh.shrink_quad(selected_quad,2);
            break;

            // t/T tourne
            case Qt::Key_T:
                if (!(mod & Qt::ControlModifier))
                    if(  selected_quad != -1 )
                        mesh.tourne_quad(selected_quad,30);
            break;

            // Star
			case Qt::Key_S:
				star(mesh);
            break;

            // Spiral
            case Qt::Key_P:
                spiral(mesh);
            break;

            //Recursive1
            case Qt::Key_R:
               mesh.create_cube();
               rec1(mesh,5);
            break;

            //ring
            case Qt::Key_W:
                mesh.create_cube();
                rec_wheel(mesh,35);
            break;

			default:
				break;
		}

		Vec3 sc;
		float r;
		mesh.bounding_sphere(sc,r);
		viewer.setSceneCenter(qglviewer::Vec(sc[0],sc[1],sc[2]));
		viewer.setSceneRadius(r);
		viewer.camera()->centerScene();
		viewer.update();
	};

	// to do when mouse clicked (P + Dir = demi-droite (en espace objet) orthogonale à l'écran passant par le point cliqué
	viewer.f_mousePress3D = [&] (Qt::MouseButton /*b*/, const glm::vec3& P, const glm::vec3& Dir)
	{
		selected_quad = mesh.intersected_closest(P,Dir);
		if (selected_quad>=0)
			selected_frame = mesh.local_frame(selected_quad);
		std::cout << selected_quad << std::endl;
	};

	viewer.clearShortcuts();
	viewer.setShortcut(QGLViewer::EXIT_VIEWER,Qt::Key_Escape);
	viewer.show();
	return a.exec();
}
