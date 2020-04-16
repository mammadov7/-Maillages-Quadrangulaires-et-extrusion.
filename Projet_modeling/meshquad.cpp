#include "meshquad.h"
#include "matrices.h"
#include <map>


void MeshQuad::clear()
{
    m_points.clear();
    m_quad_indices.clear();
    // initialisation de nombre des edges
    m_nb_ind_edges = 0;
}

int MeshQuad::add_vertex(const Vec3& P)
{
    // ajouter P à m_points
    m_points.push_back(P);
    // renvoyer l'indice du point
    return m_points.size()-1;
}


void MeshQuad::add_quad(int i1, int i2, int i3, int i4)
{
    m_quad_indices.push_back(i1);
    m_quad_indices.push_back(i2);
    m_quad_indices.push_back(i3);
    m_quad_indices.push_back(i4);

}

void MeshQuad::convert_quads_to_tris(const std::vector<int>& quads, std::vector<int>& tris)
{
    int quads_size = quads.size();
    tris.clear();
    tris.reserve(3*quads_size/2);
    // Pour chaque quad on genere 2 triangles
    // Attention a repecter l'orientation des triangles
    /*      quad            tris1         tris2
           1*   2*         1*  2*         1*
                      ->             +
           4*   3*             3*         4*  3*
    */

    for( int i = 0; i < quads_size; i += 4 ){
        tris.push_back(quads[i]);
        tris.push_back(quads[i+1]);
        tris.push_back(quads[i+2]);
        tris.push_back(quads[i]);
        tris.push_back(quads[i+2]);
        tris.push_back(quads[i+3]);
    }

}

void MeshQuad::convert_quads_to_edges(const std::vector<int>& quads, std::vector<int>& edges)
{
    int quads_size = quads.size();
    edges.clear();
    edges.reserve(quads_size); // ( *2 /2 !)
    // Pour chaque quad on genere 4 aretes, 1 arete = 2 indices.
    // Mais chaque arete est commune a 2 quads voisins !
    // Comment n'avoir qu'une seule fois chaque arete ?

    // two dimensional key
    std::map<int, std::map<int, int> > m;
    for (int i = 0; i < quads_size; i += 4){
        // first line
        if ( m[quads[i+1]][quads[i]] != 1 )
            m[quads[i]][quads[i+1]] = 1;
        if ( m[quads[i+2]][quads[i+1]] != 1)
            m[quads[i+1]][quads[i+2]] = 1;
        if ( m[quads[i+3]][quads[i+2]] != 1)
            m[quads[i+2]][quads[i+3]] = 1;
        if ( m[quads[i]][quads[i+3]] != 1 )
            m[quads[i+3]][quads[i]] = 1;
    } //end for

    // for accessing outer map
    std::map<int, std::map<int, int> >::iterator itr;
    // for accessing inner map
    std::map<int, int>::iterator ptr;
    for (itr = m.begin(); itr != m.end(); itr++) {
        for (ptr = itr->second.begin(); ptr != itr->second.end(); ptr++) {
            if( ptr->second == 1 ){
                edges.push_back(itr->first);
                edges.push_back(ptr->first);
             } // fin if
        } // fin for
    }// fin for
}

float MeshQuad::dist( const Vec3& A, const Vec3& B ){
    return sqrtf((A.x - B.x)*(A.x - B.x) + (A.y - B.y)*(A.y - B.y) + (A.z - B.z)*(A.z - B.z));
}

void MeshQuad::bounding_sphere(Vec3& C, float& R)
{
    int m_points_size = m_points.size();
    int max_dist = 0;
    int indice_max1, indice_max2;
    for (int i = 0; i < m_points_size; i++ ){
        for (int j = i+1; j < m_points_size; j++ ){
            if ( dist( m_points[i], m_points[j] ) > max_dist ){
                indice_max1 = i;
                indice_max2 = j;
                max_dist = dist( m_points[i], m_points[j] );
            }
        }
    }
    /* C = ( A + B) / 2    */
    C = vec_sum(m_points[indice_max1],m_points[indice_max2]);
    C = vec_div( C, 2 );
    R = dist( m_points[indice_max1], m_points[indice_max2] ) / 2;
}

void MeshQuad::create_cube()
{
    clear();
    int s[8]; //sommets
    // ajouter 8 sommets (-1 +1)
    s[0] = add_vertex(Vec3(-1,-1,-1));
    s[1] = add_vertex(Vec3(-1,1,-1));
    s[2] = add_vertex(Vec3(1,1,-1));
    s[3] = add_vertex(Vec3(1,-1,-1));
    s[4] = add_vertex(Vec3(1,-1,1));
    s[5] = add_vertex(Vec3(-1,-1,1));
    s[6] = add_vertex(Vec3(-1,1,1));
    s[7] = add_vertex(Vec3(1,1,1));
    // ajouter 6 faces (sens trigo)
    add_quad(s[0],s[3],s[4],s[5]);
    add_quad(s[5],s[6],s[1],s[0]);
    add_quad(s[4],s[7],s[6],s[5]);
    add_quad(s[3],s[2],s[7],s[4]);
    add_quad(s[0],s[1],s[2],s[3]);
    add_quad(s[1],s[6],s[7],s[2]);

    gl_update();
}


Vec3 MeshQuad::normal_of(const Vec3& A, const Vec3& B, const Vec3& C)
{
	// Attention a l'ordre des points !
	// le produit vectoriel n'est pas commutatif U ^ V = - V ^ U
	// ne pas oublier de normaliser le resultat.
    Vec3 AB = vec_sub(A,B);
    Vec3 AC = vec_sub(A,C);
    return  vec_normalize( vec_cross(AB, AC) );
}

float MeshQuad::aire_tris( const Vec3& A, const Vec3& B, const Vec3& C  ){
    // Aire = || AB ^ AC || / 2
    Vec3 AB = vec_sub(A,B);
    Vec3 AC = vec_sub(A,C);
    return vec_length( vec_cross(AB,AC) ) / 2;
}

float MeshQuad::aire_quad( const Vec3& A, const Vec3& B, const Vec3& C, const Vec3& D ){

    // Dans un quad concave on peut obtenir de reponses different.
    // Cela depends de ordre des points.
    // C'est pourquoi on a besoine de verifier 2 cas et prendre le minimum
    // Dans tout le quad convex on va toujours obtenir la meme resultat
    float Aire1 = aire_tris(A,C,D) + aire_tris(A,B,C);
    float Aire2 = aire_tris(B,C,D) + aire_tris(B,A,D);
    if ( Aire1 <= Aire2 )
        return  Aire1;
    else
        return  Aire2;
}

float MeshQuad::angle( const Vec3& A, const Vec3& B, const Vec3& C ){
    Vec3 BA = vec_sub(B,A);
    Vec3 BC = vec_sub(B,C);
    return acosf( vec_dot(BA,BC) / vec_length(BA) / vec_length(BC));
}

bool MeshQuad::is_points_in_quad(const Vec3& P, const Vec3& A, const Vec3& B, const Vec3& C, const Vec3& D)
{
/*
          A *           Si le point P est dans le quad, la somme des angles
           / \          doit être 360 degrée:
          /   \         angle{APB} + angle{BPC} + angle{CPD} + angle{DPA} = 360
         /  *P \
        /       \
       /    * C  \
    D *           * B
*/
//    Deuxime methode: avec les aires
//    float sum_aires = aire_tris(A,P,B) + aire_tris(B,P,C) + aire_tris(C,P,D) + aire_tris(D,P,A);
//    float air_quad = aire_quad(A,B,C,D);
//    if( abs( sum_aires - aire_quad ) < 0.000001f )
    float sum_angles = angle(A,P,B) + angle(B,P,C) + angle(C,P,D) + angle(D,P,A);
    if( abs( sum_angles - 2*M_PI) <= 0.000001f ){ //approximation
        return true;
    }
    else
        return false;
}

bool MeshQuad::intersect_ray_quad(const Vec3& P, const Vec3& Dir, int q, Vec3& inter)
{
    /*  recuperation des indices de points
        recuperation des points
        q:          0  ,   1   ,   2    , ...
        indices: 0 .. 3, 4 .. 7, 8 .. 11, ...
    */
    Vec3 P1 = m_points[ m_quad_indices[ q * 4 ] ];
    Vec3 P2 = m_points[ m_quad_indices[ q * 4 + 1 ] ];
    Vec3 P3 = m_points[ m_quad_indices[ q * 4 + 2 ] ];
    Vec3 P4 = m_points[ m_quad_indices[ q * 4 + 3 ] ];

	// calcul de l'equation du plan (N+d)
    // n⃗ =P1P2×P1P4;  d = -n⃗ . P1
    Vec3 normal = normal_of( P1,P2,P4 );
    float d = vec_dot(normal,P1);
    // calcul de l'intersection rayon plan
    // alpha => calcul de I
    float alpha = ( d - vec_dot(P,normal)) / ( vec_dot( Dir,normal) );
    // I = P + alpha*Dir est dans le plan => calcul de alpha
    if ( alpha > 0 && alpha < 1 )
        inter = vec_sum( P, vec_mul(  Dir, alpha) );
    else return false;
    // I dans le quad ?
    return is_points_in_quad(inter,P1,P2,P3,P4);
}


int MeshQuad::intersected_closest(const Vec3& P, const Vec3& Dir)
{
    int inter = -1; // indice de quad plus proche
    Vec3 I; // vecteur d'Intersection
    int m_nb_quads = m_quad_indices.size()/4;
    // on parcours tous les quads
    int i = 0;
    float dist_min=0; // distance minimal

    // distence jusqu'a premier quad
    for ( ; i < m_nb_quads; i++  ){
        // on teste si il y a intersection avec le rayon
        if( intersect_ray_quad( P, Dir, i, I )){
                dist_min = dist(P,I);
                inter = i;
                break;
        }
    }
    //distance plus petite
    for ( ; i < m_nb_quads; i++  ){
        // on teste si il y a intersection avec le rayon
        if( intersect_ray_quad( P, Dir, i, I )){
            // on garde le plus proche (de P)
            if ( dist_min > dist( P, I ) ){
                dist_min = dist(P,I);
                inter = i;
            }
        }
     } // fin for
	return inter;
}


Mat4 MeshQuad::local_frame(int q)
{
	// Repere locale = Matrice de transfo avec
	// les trois premieres colones: X,Y,Z locaux
	// la derniere colonne l'origine du repere
	// ici Z = N et X = AB
	// Origine le centre de la face
	// longueur des axes : [AB]/2

    //  recuperation des indices de points
    //  recuperation des points
    Vec3 A = m_points[ m_quad_indices[ q * 4 ] ];
    Vec3 B = m_points[ m_quad_indices[ q * 4 + 1 ] ];
    Vec3 C = m_points[ m_quad_indices[ q * 4 + 2 ] ];
    Vec3 D = m_points[ m_quad_indices[ q * 4 + 3 ] ];
	// calcul de Z:N / X:AB -> Y
    Vec3 X = vec_normalize( vec_sub(A,B) );
    Vec3 Z = normal_of( A,B,C );
    Vec3 Y = vec_normalize( vec_cross(X, Z) );
    // calcul du centre = (A + B + C + D) / 4
    Vec3 centre = vec_div( vec_sum( vec_sum(A,B), vec_sum(C,D) ), 4.0 );
    // calcul de la taille
    float t = vec_length( vec_sub(A,B)) / 2;
    // calcul de la matrice
    Vec4 c1 ( X.x, X.y, X.z, 0);
    Vec4 c2 ( Y.x, Y.y, Y.z, 0);
    Vec4 c3 ( Z.x, Z.y, Z.z, 0);
    Vec4 c4 ( centre.x, centre.y, centre.z, 1);

    return Mat4(c1,c2,c3,c4)*scale(t)  ;
}

void MeshQuad::extrude_quad(int q)
{
	// recuperation des indices de points
    int i_A = m_quad_indices[ q * 4 ];
    int i_B = m_quad_indices[ q * 4 + 1 ];
    int i_C = m_quad_indices[ q * 4 + 2 ];
    int i_D = m_quad_indices[ q * 4 + 3 ];
	// recuperation des points
    Vec3 A = m_points[i_A];
    Vec3 B = m_points[i_B];
    Vec3 C = m_points[i_C];
    Vec3 D = m_points[i_D];

	// calcul de la normale
    Vec3 normal = normal_of( A,B,D );

    // calcul de la hauteur = racine carre de aire de quad ABCD( =   )
    float t = sqrtf( aire_quad(A,B,C,D));

    // calcul et ajout des 4 nouveaux points
    int i_A1 = add_vertex( vec_sum( A, vec_mul(normal,t) ) );
    int i_B1 = add_vertex( vec_sum( B, vec_mul(normal,t) ) );
    int i_C1 = add_vertex( vec_sum( C, vec_mul(normal,t) ) );
    int i_D1 = add_vertex( vec_sum( D, vec_mul(normal,t) ) );

	// on remplace le quad initial par le quad du dessu
    m_quad_indices[ q * 4     ] = i_A1;
    m_quad_indices[ q * 4 + 1 ] = i_B1;
    m_quad_indices[ q * 4 + 2 ] = i_C1;
    m_quad_indices[ q * 4 + 3 ] = i_D1;

    // on ajoute les 4 quads des cotes
    add_quad(i_B, i_C,  i_C1, i_B1);
    add_quad(i_D, i_D1, i_C1, i_C);
    add_quad(i_A, i_A1, i_D1, i_D);
    add_quad(i_B, i_B1, i_A1, i_A);

    gl_update();
}

void MeshQuad::transfo_quad(int q, const glm::mat4& tr)
{
	// recuperation des indices de points
	// recuperation des (references de) points
    Vec3& A = m_points[ m_quad_indices[ q * 4 ] ];
    Vec3& B = m_points[ m_quad_indices[ q * 4 + 1 ] ];
    Vec3& C = m_points[ m_quad_indices[ q * 4 + 2 ] ];
    Vec3& D = m_points[ m_quad_indices[ q * 4 + 3 ] ];

	// generation de la matrice de transfo globale:
	// indice utilisation de glm::inverse() et de local_frame
    Mat4 loc = local_frame(q);

    glm::mat4 global =  loc * tr * glm::inverse(loc)  ;

    // Application au 4 points du quad
    Vec4 A4 (A, 1); A4 = global * A4;
    Vec4 B4 (B, 1); B4 = global * B4;
    Vec4 C4 (C, 1); C4 = global * C4;
    Vec4 D4 (D, 1); D4 = global * D4;

    A = Vec3(A4.x , A4.y , A4.z );
    B = Vec3(B4.x , B4.y , B4.z );
    C = Vec3(C4.x , C4.y , C4.z );
    D = Vec3(D4.x , D4.y , D4.z );
}

void MeshQuad::decale_quad(int q, float d)
{
    transfo_quad(q, translate(0,0,d));
    gl_update();
}

void MeshQuad::shrink_quad(int q, float s)
{
    transfo_quad(q, scale( 1/s ));

    gl_update();
}

void MeshQuad::tourne_quad(int q, float a)
{
    transfo_quad(q, rotateZ(a));
    gl_update();
}

MeshQuad::MeshQuad():
	m_nb_ind_edges(0)
{}


void MeshQuad::gl_init()
{
	m_shader_flat = new ShaderProgramFlat();
	m_shader_color = new ShaderProgramColor();

	//VBO
	glGenBuffers(1, &m_vbo);

	//VAO
	glGenVertexArrays(1, &m_vao);
	glBindVertexArray(m_vao);
	glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
	glEnableVertexAttribArray(m_shader_flat->idOfVertexAttribute);
	glVertexAttribPointer(m_shader_flat->idOfVertexAttribute, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glBindVertexArray(0);

	glGenVertexArrays(1, &m_vao2);
	glBindVertexArray(m_vao2);
	glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
	glEnableVertexAttribArray(m_shader_color->idOfVertexAttribute);
	glVertexAttribPointer(m_shader_color->idOfVertexAttribute, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glBindVertexArray(0);

	//EBO indices
	glGenBuffers(1, &m_ebo);
	glGenBuffers(1, &m_ebo2);
}

void MeshQuad::gl_update()
{
	//VBO
	glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
	glBufferData(GL_ARRAY_BUFFER, 3 * m_points.size() * sizeof(GLfloat), &(m_points[0][0]), GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	std::vector<int> tri_indices;
	convert_quads_to_tris(m_quad_indices,tri_indices);

	//EBO indices
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER,tri_indices.size() * sizeof(int), &(tri_indices[0]), GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	std::vector<int> edge_indices;
	convert_quads_to_edges(m_quad_indices,edge_indices);
	m_nb_ind_edges = edge_indices.size();
//    printf("\n\nEdges: %d\n\n",m_nb_ind_edges);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ebo2);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER,m_nb_ind_edges * sizeof(int), &(edge_indices[0]), GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}



void MeshQuad::set_matrices(const Mat4& view, const Mat4& projection)
{
	viewMatrix = view;
	projectionMatrix = projection;
}

void MeshQuad::draw(const Vec3& color)
{
	glEnable(GL_CULL_FACE);
	glEnable(GL_POLYGON_OFFSET_FILL);
	glPolygonOffset(1.0f, 1.0f);

	m_shader_flat->startUseProgram();
	m_shader_flat->sendViewMatrix(viewMatrix);
	m_shader_flat->sendProjectionMatrix(projectionMatrix);
	glUniform3fv(m_shader_flat->idOfColorUniform, 1, glm::value_ptr(color));
	glBindVertexArray(m_vao);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,m_ebo);
	glDrawElements(GL_TRIANGLES, 3*m_quad_indices.size()/2,GL_UNSIGNED_INT,0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,0);
	glBindVertexArray(0);
	m_shader_flat->stopUseProgram();

	glDisable(GL_POLYGON_OFFSET_FILL);

	m_shader_color->startUseProgram();
	m_shader_color->sendViewMatrix(viewMatrix);
	m_shader_color->sendProjectionMatrix(projectionMatrix);
	glUniform3f(m_shader_color->idOfColorUniform, 0.0f,0.0f,0.0f);
	glBindVertexArray(m_vao2);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,m_ebo2);
	glDrawElements(GL_LINES, m_nb_ind_edges,GL_UNSIGNED_INT,0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,0);
	glBindVertexArray(0);
	m_shader_color->stopUseProgram();
	glDisable(GL_CULL_FACE);
}

