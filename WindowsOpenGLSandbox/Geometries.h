#ifndef GEOMETRIES_H
#define GEOMETRIES_H

#include <glad/glad.h>
#include <glm/glm.hpp>

#include <algorithm>


struct Vertex
{
  Vertex(const glm::vec3& pPosition, const glm::vec4& pColor) : position(pPosition), color(pColor) {}

  glm::vec3 position;
  glm::vec4 color;
};

struct ShapeData
{
  ShapeData() {}

  GLsizeiptr getVertexBufferSize()
  {
    return numVertices * sizeof(Vertex);
  }

  GLsizeiptr getIndexBufferSize()
  {
    return numIndices * sizeof(GLuint);
  }

  Vertex* vertices;
  GLuint* indices;
  GLuint  numIndices;
  GLuint  numVertices;
};

class ShapeGenerator
{
public:
  static ShapeData generateTriangle()
  {
    ShapeData vShapeData = {};
    
    //set num vertices
    GLuint vNumVertices = 3;

    //create vertices
    Vertex* vVertices = new Vertex[vNumVertices]
    {
      { glm::vec3(-1.0F, -1.0F, 0.0F),    glm::vec4(0.0F, 1.0F, 0.0F, 1.0F) },
      { glm::vec3(1.0F, -1.0F, 0.0F),     glm::vec4(0.0F, 0.0F, 1.0F, 1.0F) },
      { glm::vec3(0.0F,  1.0F, 0.0F),     glm::vec4(1.0F, 0.0F, 0.0F, 1.0F) }
    };

    //create indices
    GLuint vNumIndices = 3;

    //set indices
    GLuint* vIndices = new GLuint[vNumIndices];

    for(unsigned int vIndex = 0; vIndex < vNumIndices; ++vIndex)
    {
      vIndices[vIndex] = vIndex;
    }

    vShapeData.vertices    = vVertices;
    vShapeData.indices     = vIndices;
    vShapeData.numVertices = vNumVertices;
    vShapeData.numIndices  = vNumIndices;

    return vShapeData;
  }
};

#endif

