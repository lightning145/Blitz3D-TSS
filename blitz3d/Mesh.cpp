#include "Mesh.h"

#include <glad/glad.h>
#include "stb_image.h"

#include <string>

Mesh::Mesh(std::vector<MD_Math::VECTOR3> pos, 
           std::vector<MD_Math::VECTOR2> texc, 
           std::vector<Texture> texs,
            std::vector<unsigned int> indices)
{
    Vertex temp = {
        MD_Math::VECTOR3(0.0f, 0.0f, 0.0f), 
        MD_Math::VECTOR3(0.0f, 0.0f, 0.0f), 
        MD_Math::VECTOR2(0.0f, 0.0f), 
        MD_Math::VECTOR3(0.0f, 0.0f, 0.0f), 
        MD_Math::VECTOR3(0.0f, 0.0f, 0.0f),
        MD_Math::IVECTOR4(0, 0, 0, 0),
        MD_Math::VECTOR4(0.0f, 0.0f, 0.0f, 0.0f)};

    MD_Math::VECTOR3 normal(0.0f, 0.0f, 0.0f);
    
    int j = 0, c = 0;
    for(int i = 0; i < pos.size(); i++)
    {
        if(i % 4 == 0 && i != 0)
        {
            j += 1; 
            c += 4;
        }
            
        temp.Position = pos[i];
        normal = -Vector3Normalized(
            VectorCross(
                pos[c + 1] - pos[c],
                pos[c + 2] - pos[c]));

        temp.Normals = normal;
        temp.TexCoords = texc[i];
        vertices.push_back(temp);
    }

    ComputeTangents(vertices, indices);

    textures = texs;
    texCount = texs.size();

    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);

    glGenBuffers(1, &EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Normals));
    glEnableVertexAttribArray(1);

    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, TexCoords));
    glEnableVertexAttribArray(2);

    glEnableVertexAttribArray(3);
    glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Tangent));

    glEnableVertexAttribArray(4);
    glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Bitangent));

    glEnableVertexAttribArray(5);
    glVertexAttribIPointer(5, 4, GL_INT, sizeof(Vertex), (void*)offsetof(Vertex, BoneIDs));
        
    glEnableVertexAttribArray(6);
    glVertexAttribPointer(6, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Weights));

    ind = indices.size();
}

Mesh::Mesh(std::vector<Vertex> vers,
        std::vector<Texture> texs,
        std::vector<unsigned int> indices)
{
    Vertex temp = {
        MD_Math::VECTOR3(0.0f, 0.0f, 0.0f), 
        MD_Math::VECTOR3(0.0f, 0.0f, 0.0f), 
        MD_Math::VECTOR2(0.0f, 0.0f), 
        MD_Math::VECTOR3(0.0f, 0.0f, 0.0f), 
        MD_Math::VECTOR3(0.0f, 0.0f, 0.0f),
        MD_Math::IVECTOR4(0, 0, 0, 0),
        MD_Math::VECTOR4(0.0f, 0.0f, 0.0f, 0.0f)};
    
    for(int i = 0; i < vers.size(); i++)
    {
        temp.Position = vers[i].Position;
        temp.Normals = vers[i].Normals;
        temp.TexCoords = vers[i].TexCoords;
        temp.Tangent = vers[i].Tangent;
        temp.Bitangent = vers[i].Bitangent;
        temp.BoneIDs = vers[i].BoneIDs;
        temp.Weights = vers[i].Weights;
        vertices.push_back(temp);
    }
    
    textures = texs;
    texCount = texs.size();
    
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);

    glGenBuffers(1, &EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Normals));
    glEnableVertexAttribArray(1);

    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, TexCoords));
    glEnableVertexAttribArray(2);

    glEnableVertexAttribArray(3);
    glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Tangent));

    glEnableVertexAttribArray(4);
    glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Bitangent));

    glEnableVertexAttribArray(5);
    glVertexAttribIPointer(5, 4, GL_INT, sizeof(Vertex), (void*)offsetof(Vertex, BoneIDs));
        
    glEnableVertexAttribArray(6);
    glVertexAttribPointer(6, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Weights));

    ind = indices.size();
}

Mesh::~Mesh()
{
   
}

void Mesh::Free()
{
   glDeleteBuffers(1, &VBO);
   glDeleteBuffers(1, &EBO);
   glDeleteVertexArrays(1, &VAO);

   for(unsigned int i = 0; i < textures.size(); i++)
   {
        glDeleteTextures(1, &textures[i].id);
   }
}

void Mesh::Draw(Shader& shader)
{
    unsigned int diffuseNr = 1;
    unsigned int specularNr = 1;
    for(unsigned int i = 0; i < textures.size(); i++)
    {
        glActiveTexture(GL_TEXTURE0 + i);
        std::string number;
        std::string name = textures[i].type;
        if(name == "texture_diffuse")
            number = std::to_string(diffuseNr++);
        else if(name == "texture_specular")
            number = std::to_string(specularNr++);

        shader.SetInt((name + number).c_str(), i);
        glBindTexture(GL_TEXTURE_2D, textures[i].id);
    }
    glActiveTexture(GL_TEXTURE0);

    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, ind, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}

void Mesh::ComputeTangents(std::vector<Vertex>& vertices, 
                      const std::vector<unsigned int>& indices)
{
    for (size_t i = 0; i < indices.size(); i += 3) {
        Vertex& v0 = vertices[indices[i]];
        Vertex& v1 = vertices[indices[i+1]];
        Vertex& v2 = vertices[indices[i+2]];
        
        MD_Math::VECTOR3 edge1 = v1.Position - v0.Position;
        MD_Math::VECTOR3 edge2 = v2.Position - v0.Position;
        
        MD_Math::VECTOR2 deltaUV1 = v1.TexCoords - v0.TexCoords;
        MD_Math::VECTOR2 deltaUV2 = v2.TexCoords - v0.TexCoords;
        
        float f = 1.0f / (deltaUV1.x * deltaUV2.y - deltaUV2.x * deltaUV1.y);
        
        MD_Math::VECTOR3 tangent(0.0f, 0.0f, 0.0f);
        MD_Math::VECTOR3 bitangent(0.0f, 0.0f, 0.0f);
        
        tangent.x = f * (deltaUV2.y * edge1.x - deltaUV1.y * edge2.x);
        tangent.y = f * (deltaUV2.y * edge1.y - deltaUV1.y * edge2.y);
        tangent.z = f * (deltaUV2.y * edge1.z - deltaUV1.y * edge2.z);
        
        bitangent.x = f * (-deltaUV2.x * edge1.x + deltaUV1.x * edge2.x);
        bitangent.y = f * (-deltaUV2.x * edge1.y + deltaUV1.x * edge2.y);
        bitangent.z = f * (-deltaUV2.x * edge1.z + deltaUV1.x * edge2.z);
        
        v0.Tangent += tangent;
        v1.Tangent += tangent;
        v2.Tangent += tangent;
        
        v0.Bitangent += bitangent;
        v1.Bitangent += bitangent;
        v2.Bitangent += bitangent;
    }
    
    for (auto& vertex : vertices) {
        vertex.Tangent = MD_Math::Vector3Normalized(vertex.Tangent);
        vertex.Bitangent = MD_Math::Vector3Normalized(vertex.Bitangent);
    }
}

Cube::Cube()
{
    float cubeVertices[] = {
    // positions          
    -1.0f,  1.0f, -1.0f,
    -1.0f, -1.0f, -1.0f,
    1.0f, -1.0f, -1.0f,
    1.0f, -1.0f, -1.0f,
    1.0f,  1.0f, -1.0f,
    -1.0f,  1.0f, -1.0f,

    -1.0f, -1.0f,  1.0f,
    -1.0f, -1.0f, -1.0f,
    -1.0f,  1.0f, -1.0f,
    -1.0f,  1.0f, -1.0f,
    -1.0f,  1.0f,  1.0f,
    -1.0f, -1.0f,  1.0f,

    1.0f, -1.0f, -1.0f,
    1.0f, -1.0f,  1.0f,
    1.0f,  1.0f,  1.0f,
    1.0f,  1.0f,  1.0f,
    1.0f,  1.0f, -1.0f,
    1.0f, -1.0f, -1.0f,

    -1.0f, -1.0f,  1.0f,
    -1.0f,  1.0f,  1.0f,
    1.0f,  1.0f,  1.0f,
    1.0f,  1.0f,  1.0f,
    1.0f, -1.0f,  1.0f,
    -1.0f, -1.0f,  1.0f,

    -1.0f,  1.0f, -1.0f,
    1.0f,  1.0f, -1.0f,
    1.0f,  1.0f,  1.0f,
    1.0f,  1.0f,  1.0f,
    -1.0f,  1.0f,  1.0f,
    -1.0f,  1.0f, -1.0f,

    -1.0f, -1.0f, -1.0f,
    -1.0f, -1.0f,  1.0f,
    1.0f, -1.0f, -1.0f,
    1.0f, -1.0f, -1.0f,
    -1.0f, -1.0f,  1.0f,
    1.0f, -1.0f,  1.0f
};

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(cubeVertices), &cubeVertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
}

Cube::~Cube()
{
    glDeleteBuffers(1, &VBO);
    glDeleteVertexArrays(1, &VAO);
}

void Cube::Draw(Shader& shader)
{
    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLES, 0, 36);
    glBindVertexArray(0);
}

Quad::Quad()
{
    float quadVertices[] = {
        // positions        // texture Coords
        -1.0f,  1.0f, 0.0f, 0.0f, 1.0f,
        -1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
        1.0f,  1.0f, 0.0f, 1.0f, 1.0f,
        1.0f, -1.0f, 0.0f, 1.0f, 0.0f,
    };
    // setup plane VAO
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
}

Quad::~Quad()
{
    glDeleteBuffers(1, &VBO);
    glDeleteVertexArrays(1, &VAO);
}

void Quad::Draw(Shader& shader)
{
    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    glBindVertexArray(0);
}

Sphere::Sphere()
{
    for (int i = 0; i <= stacks; ++i) {
        float stackAngle = MD_MATH_PI / 2 - i * (MD_MATH_PI / stacks);
        float xy = radius * MD_Math::Cos(stackAngle);          
        float z = radius * MD_Math::Sin(stackAngle);           

        for (int j = 0; j <= sectors; ++j) {
            float sectorAngle = j * (2 * MD_MATH_PI / sectors);

            Vertex vertex = {
                MD_Math::VECTOR3(0.0f, 0.0f, 0.0f),
                MD_Math::VECTOR3(0.0f, 0.0f, 0.0f),
                MD_Math::VECTOR2(0.0f, 0.0f)
            };
    
            vertex.position.x = xy * MD_Math::Cos(sectorAngle);
            vertex.position.y = xy * MD_Math::Sin(sectorAngle);
            vertex.position.z = z;
        
            vertex.normal = MD_Math::Vector3Normalized(vertex.position);
        
            vertex.texCoords.x = (float)j / sectors;
            vertex.texCoords.y = (float)i / stacks;
        
            vertices.push_back(vertex);
        }
    }

    for (int i = 0; i < stacks; ++i) {
        int k1 = i * (sectors + 1);
        int k2 = k1 + sectors + 1;

        for (int j = 0; j < sectors; ++j, ++k1, ++k2) {
            if (i != 0) {
                indices.push_back(k1);
                indices.push_back(k2);
                indices.push_back(k1 + 1);
            }
    
            if (i != (stacks - 1)) {
                indices.push_back(k1 + 1);
                indices.push_back(k2);
                indices.push_back(k2 + 1);
            }
        }
    }

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);
    
    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));
    glEnableVertexAttribArray(1);

    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, texCoords));
    glEnableVertexAttribArray(2);

    glBindVertexArray(0);
}

Sphere::~Sphere()
{
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
    glDeleteVertexArrays(1, &VAO);
}

void Sphere::Draw(Shader& shader)
{
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
}