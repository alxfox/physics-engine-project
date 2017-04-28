#include "ShapeLibrary.h"
#include <cmath>
#include <cstring>
#include <cassert>

void gp::graphics::ShapeLibrary::setGeometry( Shape shape,
                                              GLfloat* vertex_positions,
                                              size_t vertex_positions_size,
                                              GLfloat* vertex_normals,
                                              size_t vertex_normals_size,
                                              GLushort* triangles,
                                              size_t triangles_size )
{
  // Bind cube buffers
  glBindVertexArray(m_vao[shape]);
  // Cube vertex positions
  glBindBuffer(GL_ARRAY_BUFFER, m_vbo[shape][Position]);
  glBufferData(GL_ARRAY_BUFFER, vertex_positions_size, vertex_positions, GL_STATIC_DRAW);
  glEnableVertexAttribArray(Position);
  glVertexAttribPointer(Position, 3, GL_FLOAT, GL_FALSE, 3*sizeof(GLfloat), 0);  
  // Cube vertex normals
  glBindBuffer(GL_ARRAY_BUFFER, m_vbo[shape][Normal]);
  glBufferData(GL_ARRAY_BUFFER, vertex_normals_size, vertex_normals, GL_STATIC_DRAW);  
  glEnableVertexAttribArray(Normal);
  glVertexAttribPointer(Normal, 3, GL_FLOAT, GL_FALSE, 3*sizeof(GLfloat), 0);
  // Cube elements
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ebo[shape]);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, triangles_size, triangles, GL_STATIC_DRAW);
  m_nIndices[shape] = triangles_size / sizeof(GLushort);
}

gp::graphics::ShapeLibrary::~ShapeLibrary()
{
  for (unsigned shape = 0; shape < NUMBER_OF_SHAPES; ++shape) {
    glDeleteBuffers(NUMBER_OF_BUFFER_TYPES, m_vbo[shape]);
  }
  glDeleteBuffers(NUMBER_OF_SHAPES, m_ebo);
  glDeleteVertexArrays(NUMBER_OF_SHAPES, m_vao);
}

void gp::graphics::ShapeLibrary::bind(Shape shape)
{
  glBindVertexArray(m_vao[shape]);
}

void gp::graphics::ShapeLibrary::draw(Shape shape)
{
  glDrawElements(GL_TRIANGLES, m_nIndices[shape], GL_UNSIGNED_SHORT, 0);
}

gp::graphics::ShapeLibrary::ShapeLibrary()
{
  // Generate OpenGL buffers
  for (unsigned shape = 0; shape < NUMBER_OF_SHAPES; ++shape) {
    glGenBuffers(NUMBER_OF_BUFFER_TYPES, m_vbo[shape]);
  }
  glGenBuffers(NUMBER_OF_SHAPES, m_ebo);
  glGenVertexArrays(NUMBER_OF_SHAPES, m_vao);

  // Shape definitions
  GLfloat cube_vertex_positions[] = {
    -1.0f, -1.0f,  1.0f,
     1.0f, -1.0f,  1.0f,
     1.0f,  1.0f,  1.0f,
    -1.0f,  1.0f,  1.0f,
    -1.0f, -1.0f, -1.0f,
     1.0f, -1.0f, -1.0f,
     1.0f,  1.0f, -1.0f,
    -1.0f,  1.0f, -1.0f
  };
  
  GLfloat cube_vertex_normals[] = {
     0.0f,  0.0f,  1.0f,
     1.0f,  0.0f,  0.0f,
     0.0f,  1.0f,  0.0f,
    -1.0f,  0.0f,  0.0f,
     0.0f, -1.0f,  0.0f,
     0.0f,  0.0f, -1.0f,
     1.0f,  0.0f,  0.0f,
     1.0f,  0.0f,  0.0f
  };
  
  GLushort cube_triangles[] = {
    0, 1, 2,
    0, 2, 3,
    1, 5, 6,
    1, 6, 2,
    2, 6, 7,
    2, 7, 3,
    3, 4, 0,
    3, 7, 4,
    4, 5, 1,
    4, 1, 0,
    5, 7, 6,
    5, 4, 7
  };
  
  GLfloat icosahedron_vertex_positions[] = {
     0.000000f, -1.000000f,  0.000000f,
     0.723600f, -0.447215f,  0.525720f,
    -0.276385f, -0.447215f,  0.850640f,
    -0.894425f, -0.447215f,  0.000000f,
    -0.276385f, -0.447215f, -0.850640f,
     0.723600f, -0.447215f, -0.525720f,
     0.276385f,  0.447215f,  0.850640f,
    -0.723600f,  0.447215f,  0.525720f,
    -0.723600f,  0.447215f, -0.525720f,
     0.276385f,  0.447215f, -0.850640f,
     0.894425f,  0.447215f,  0.000000f,
     0.000000f,  1.000000f,  0.000000f
  };
  
  GLushort icosahedron_triangles[] = {
    0, 1, 2,
    1, 0, 5,
    0, 2, 3,
    0, 3, 4,
    0, 4, 5,
    1, 5, 10,
    2, 1, 6,
    3, 2, 7,
    4, 3, 8,
    5, 4, 9,
    1, 10, 6,
    2, 6, 7,
    3, 7, 8,
    4, 8, 9,
    5, 9, 10,
    6, 10, 11,
    7, 6, 11,
    8, 7, 11,
    9, 8, 11,
    10, 9, 11
  };
  
  GLushort sphere_nvertices, sphere_ntriangles;
  GLfloat* sphere_vertex_positions;
  GLushort* sphere_triangles;
  refineIcosahedron(  icosahedron_vertex_positions,
                      sizeof(icosahedron_vertex_positions) / (3*sizeof(GLfloat)),
                      icosahedron_triangles,
                      sizeof(icosahedron_triangles) / (3*sizeof(GLushort)),
                      sphere_vertex_positions,
                      sphere_nvertices,
                      sphere_triangles,
                      sphere_ntriangles,
                      3 );
  
  setGeometry(Cube, cube_vertex_positions, sizeof(cube_vertex_positions), cube_vertex_normals, sizeof(cube_vertex_normals), cube_triangles, sizeof(cube_triangles));
  setGeometry(Sphere, sphere_vertex_positions, 3*sphere_nvertices*sizeof(GLfloat), sphere_vertex_positions, 3*sphere_nvertices*sizeof(GLfloat), sphere_triangles, 3*sphere_ntriangles*sizeof(GLushort));
  
  delete[] sphere_vertex_positions;
  delete[] sphere_triangles;
  
  // Unbind buffers
  glBindVertexArray(0);
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

/** Refine an icosahedron (creating an icosphere). 
 * 
 * Each triangle of the original icosahedron is refined in the following way:
 *     /\
 *    /__\
 *   /\  /\
 *  /__\/__\
 * Creating 3 additional vertices and 4 new triangles per original triangle.
 * The new vertices are moved to the unit sphere.
 * (Otherwise one would obtain a refined icosahedron instead of an icosphere.)
 */
void gp::graphics::ShapeLibrary::refineIcosahedron( GLfloat*    icosahedron_vertex_positions,
                                                    GLushort    icosahedron_nvertices,
                                                    GLushort*   icosahedron_triangles,
                                                    GLushort    icosahedron_ntriangles,
                                                    GLfloat*&   sphere_vertex_positions,
                                                    GLushort&   sphere_nvertices,
                                                    GLushort*&  sphere_triangles,
                                                    GLushort&   sphere_ntriangles,
                                                    GLushort    nsubdivisions )
{
  // New vertices from subdivison of a single triangle: 3 * sum_{i=0}^{nsubdivisions-1} 4^i = 3 * (1-4^nsubdivisions) / (1-4) = 4^nsubdivisions - 1
  sphere_nvertices = icosahedron_nvertices + icosahedron_ntriangles * ((1 << 2*nsubdivisions) - 1);
  sphere_ntriangles = icosahedron_ntriangles * (1 << 2*nsubdivisions);
  sphere_vertex_positions = new GLfloat[3 * sphere_nvertices];
  sphere_triangles = new GLushort[3 * sphere_ntriangles];
  GLushort* temp_triangles = new GLushort[3 * sphere_ntriangles];
  
  memcpy(sphere_vertex_positions, icosahedron_vertex_positions, 3 * icosahedron_nvertices * sizeof(GLfloat));  
  GLushort* original_triangles = icosahedron_triangles;
  GLushort nverts = icosahedron_nvertices;
  GLushort ntris_original = icosahedron_ntriangles;
  GLushort ntris;
  while (nsubdivisions--) {
    ntris = 0;
    memcpy(temp_triangles, original_triangles, 3 * ntris_original * sizeof(GLushort));
    for (unsigned tri = 0; tri < ntris_original; ++tri) {
      GLfloat* x[3];
      for (unsigned v = 0; v < 3; ++v) {
        x[v] = &sphere_vertex_positions[ 3 * temp_triangles[3*tri + v] ];
      }
      // Introduce a new vertex on each edge
      for (unsigned e = 0; e < 3; ++e) {
        GLfloat* midpoint = &sphere_vertex_positions[3 * nverts++];
        for (unsigned v = 0; v < 3; ++v) {
          midpoint[v] = 0.5f * (x[e][v] + x[(e+1)%3][v]);
        }
        // Move vertex to unit sphere
        GLfloat length = 0.0f;
        for (unsigned v = 0; v < 3; ++v) {
          length += midpoint[v]*midpoint[v];
        }
        length = sqrt(length);
        for (unsigned v = 0; v < 3; ++v) {
          midpoint[v] /= length;
        }
      }
      // Add triangles containing original vertices
      for (unsigned t = 0; t < 3; ++t) {
        sphere_triangles[3 * ntris + 0] = temp_triangles[3*tri + t];
        sphere_triangles[3 * ntris + 1] = nverts - 3 + t;
        sphere_triangles[3 * ntris + 2] = nverts - 3 + (t+2)%3;
        ++ntris;
      }
      // Add centre triangle
      sphere_triangles[3 * ntris + 0] = nverts - 3;
      sphere_triangles[3 * ntris + 1] = nverts - 2;
      sphere_triangles[3 * ntris + 2] = nverts - 1;
      ++ntris;
    }
    original_triangles = sphere_triangles;
    ntris_original = ntris;
  }
  
  assert(sphere_ntriangles == ntris);
  assert(sphere_nvertices == nverts);
  
  delete[] temp_triangles;  
}
