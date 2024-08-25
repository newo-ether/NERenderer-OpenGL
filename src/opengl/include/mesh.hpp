// mesh.hpp

#pragma once

class Mesh {
    public:
        Mesh(float *vertices,
             unsigned int verticesLength,
             unsigned int *faces,
             unsigned int facesLength);

        ~Mesh();
        unsigned int GetVAO() const;
        unsigned int GetVerticesLength() const;
        unsigned int GetFacesLength() const;

    private:
        unsigned int verticesLength;
        unsigned int facesLength;
        unsigned int VAO, VBO, EBO;
};
