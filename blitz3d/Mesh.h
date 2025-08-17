#ifndef MESH_H
#define MESH_H

#include "MD_Math.h"
#include <iostream>
#include <vector>

#include "Shader.h"

struct Vertex
{
    MD_Math::VECTOR3 Position;
    MD_Math::VECTOR3 Normals;
    MD_Math::VECTOR2 TexCoords;
    MD_Math::VECTOR3 Tangent;   
    MD_Math::VECTOR3 Bitangent; 
    MD_Math::IVECTOR4 BoneIDs;       
    MD_Math::VECTOR4 Weights;    
};

struct Texture {
    unsigned int id;
    std::string type;
    std::string path;
};

class Mesh
{
private:
    unsigned int VAO, VBO, EBO;
    std::vector<Vertex> vertices;
    std::vector<Texture> textures;
    unsigned int ind;

    void ComputeTangents(std::vector<Vertex>& vertices, 
                      const std::vector<unsigned int>& indices);
          
public:
    Mesh(){};
    Mesh(std::vector<MD_Math::VECTOR3> pos, 
        std::vector<MD_Math::VECTOR2> texc, 
        std::vector<Texture> texs,
        std::vector<unsigned int> indices);

    Mesh(
        std::vector<Vertex> vers,
        std::vector<Texture> texs,
        std::vector<unsigned int> indices);    

    ~Mesh();

    void Draw(Shader& shader);
    void Free();

    unsigned int texCount;
};

class Cube
{
private:
    unsigned int VAO, VBO;
public:
    Cube();
    ~Cube();

    void Draw(Shader& shader);
};

class Quad
{
private:
    unsigned int VAO, VBO;
public:
    Quad();
    ~Quad();

    void Draw(Shader& shader);
};

class Sphere
{
private:
    unsigned int VAO, VBO, EBO;
    struct Vertex {
        MD_Math::VECTOR3 position;
        MD_Math::VECTOR3 normal;
        MD_Math::VECTOR2 texCoords;
    };

    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;

    const float radius = 1.0f;
    const int sectors = 36;
    const int stacks = 18;

public:
    Sphere();
    ~Sphere();

    void Draw(Shader& shader);
};

#endif