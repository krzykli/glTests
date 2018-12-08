#include <GL\glew.h>
#include "typedefs.h"

struct Mesh
{
    GLuint vao = 0;
    GLuint vbo = 0;
    GLuint ibo = 0;
    GLsizei indexCount = 0;
};

void CreateMesh(Mesh *m, GLfloat *vertices, u32 *indices, u32 numOfVertices, u32 numOfIndices)
{
    m->indexCount = numOfIndices;

    glGenVertexArrays(1, &m->vao);
    glBindVertexArray(m->vao);

    glGenBuffers(1, &m->ibo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m->ibo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices[0]) * numOfIndices,
                 indices, GL_STATIC_DRAW);

    glGenBuffers(1, &m->vbo);
    glBindBuffer(GL_ARRAY_BUFFER, m->vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices[0]) * numOfVertices, vertices, GL_STATIC_DRAW);

    // location layout, vertex data lenght, type of data, normalize, stride, offset
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(0); // layout index

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    glBindVertexArray(0);
}

void RenderMesh(Mesh *m)
{
    glBindVertexArray(m->vao);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m->ibo);
    glDrawElements(GL_TRIANGLES, m->indexCount, GL_UNSIGNED_INT, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

void ClearMesh(Mesh *m)
{

    if (m->ibo != 0)
    {
         glDeleteBuffers(1, &m->ibo);
         m->ibo = 0;
    }

    if (m->vbo != 0)
    {
         glDeleteBuffers(1, &m->vbo);
         m->vbo= 0;
    }

    if (m->vao != 0)
    {
         glDeleteBuffers(1, &m->vao);
         m->vao= 0;
    }

    m->indexCount = 0;
}
