//
// Created by sneeuwpop on 28-6-19.
//

#pragma once

#include <glad/glad.h>
#include <vector>
#include <memory>


class Mesh;
typedef std::shared_ptr<Mesh> SharedMesh;

class Mesh
{
    bool useIndicies = true;

public:
    // delegating constructor
    Mesh(unsigned int nrOfVerts, unsigned int nrOfIndices) : nrOfVerts(nrOfVerts), nrOfIndices(nrOfIndices) {};
    Mesh(unsigned int nrOfVerts, unsigned int nrOfIndices, bool useIndicies) : Mesh(nrOfVerts, nrOfIndices) { this->useIndicies = useIndicies; };

    std::vector<float> vertices = {};
    std::vector<unsigned short> indicies = {};

    GLenum renderMode = GL_TRIANGLES;

    unsigned int nrOfVerts = 0;
    unsigned int nrOfIndices = 0;

    void render();
};
