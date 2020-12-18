#pragma once

#include <string>
#include <vector>

#include "glm/glm.hpp"


struct Vertex {
	glm::vec3 Position;
	glm::vec3 Normal;
	glm::vec2 TexCoords;
	glm::vec3 Tangent;
	glm::vec3 Bitangent;
};


class Mesh
{
public:
	Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<class Texture*> &textures);
	~Mesh();
	void Draw(class Shader& shader);
	unsigned int VAO;

private:
	class VertexArray* m_Vao;
	class VertexBuffer* m_Vbo;
	class IndexBuffer* m_Ibo;
	class VertexBufferLayout* m_Layout;
	std::vector<Texture> m_Tex;
	unsigned int VBO, EBO;

	// mesh Data
	std::vector<Vertex>       vertices;
	std::vector<unsigned int> indices;
	std::vector<Texture*> textures;

	void SetupMesh();
};