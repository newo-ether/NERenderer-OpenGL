// mesh.cpp

#include "include/mesh.hpp"

#include "../../third-party/glad/include/glad/glad.h"

using namespace std;

Mesh::Mesh(float *vertices,
           unsigned int verticesLength,
           unsigned int *faces,
           unsigned int facesLength):

           verticesLength(verticesLength),
           facesLength(facesLength) {

    // Generate VAO, VBO, EBO
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    // Bind VAO
    glBindVertexArray(VAO);

    // Bind VBO and Copy Data
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, verticesLength * sizeof(float), vertices, GL_STATIC_DRAW);

    // Bind EBO and Copy Data
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, facesLength * sizeof(unsigned int), faces, GL_STATIC_DRAW);

    // Set Vertex Attribute Pointer
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // Unbind VAO
    glBindVertexArray(0); 
}

Mesh::~Mesh() {
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
}

unsigned int Mesh::GetVAO() const {
    return VAO;
}

unsigned int Mesh::GetVerticesLength() const {
    return verticesLength;
}

unsigned int Mesh::GetFacesLength() const {
    return facesLength;
}
