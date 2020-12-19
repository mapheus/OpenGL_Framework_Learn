#include "MarchingCubes.h"
#include "PerlinNoise.hpp"

#include "glm/glm.hpp"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <math.h>
#include <array>


MarchingCubes::MarchingCubes(const char* compute, const char* compValues, const char* draw)
{
	_computeProgram = new Shader(compute, true);
	_computeValuesProgram = new Shader(compValues, true);
	_drawProgram = new Shader(draw, false);
	rock = new Texture("res/textures/rock.jpg", "diffuse_texture");
	grass = new Texture("res/textures/grass.jpg", "grass_texture");
	sand = new Texture("res/textures/sand.jpg", "sand_texture");
	snow = new Texture("res/textures/snow.jpg", "snow_texture");
}

void MarchingCubes::UpdateTransform(glm::vec3 pos, glm::vec3 cubesize)
{
	this->x_grid_min = (double)pos.x - (double)cubesize.x /2.f;
	this->x_grid_max = (double)pos.x + (double)cubesize.x / 2.f;

	this->y_grid_min = (double)pos.y - (double)cubesize.y / 2.f;
	this->y_grid_max = (double)pos.y + (double)cubesize.y / 2.f;

	this->z_grid_min = (double)pos.z - (double)cubesize.z / 2.f;
	this->z_grid_max = (double)pos.z + (double)cubesize.z / 2.f;
	_cubesize = cubesize;
	//CalcValues();
	chunkVaos.clear();
	for (int i = 0; i < res-1; i++) {
		Update(i);
	}
}

void MarchingCubes::Update(int z)
{
	const float x_step_size = (float)(x_grid_max - x_grid_min) / (float)(res - 1);
	const float y_step_size = (float)(y_grid_max - y_grid_min) / (float)(res - 1);
	const float z_step_size = (float)(z_grid_max - z_grid_min) / (float)(res - 1);

	GLsizei width = static_cast<GLsizei>(res) - 1;
	GLsizei height = static_cast<GLsizei>(res) - 1;

	// Amount of elements before input to marching compute
	const int element_offset = 2;

	int dimension = 6;

	
	std::vector<GLfloat> data_to_values;
	data_to_values.push_back(res);
	data_to_values.push_back(surface_level);
	data_to_values.push_back(x_grid_min);
	data_to_values.push_back(x_step_size);
	data_to_values.push_back(y_grid_min);
	data_to_values.push_back(y_step_size);
	data_to_values.push_back(z_grid_min);
	data_to_values.push_back(z_step_size);
	data_to_values.push_back(z);
	int res_plusone = res + 1;
	out_to_marching_size = element_offset + sizeof(glm::vec4) * 2 * res_plusone * res_plusone;
	out0_size = sizeof(GLfloat**) * 3 * width * height * dimension*5;

	// -------- CALCULATE THE VALUES ----------------
	_computeValuesProgram->Bind();
	glGenBuffers(1, &inp_to_values);
	glGenBuffers(1, &out_to_marching);
	glGenBuffers(1, &tri_table_input);
	glGenBuffers(1, &output0);


	GLuint block_index = glGetProgramResourceIndex(_computeValuesProgram->GetShaderID(), GL_SHADER_STORAGE_BLOCK, "in0");

	GLuint binding_index = 1;
	//glShaderStorageBlockBinding(_computeValuesProgram->GetShaderID(), block_index, binding_index);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, inp_to_values);
	glBufferData(GL_SHADER_STORAGE_BUFFER, amount_elements_to_values * sizeof(GLfloat), data_to_values.data(), GL_DYNAMIC_DRAW);
	//glBufferSubData(GL_SHADER_STORAGE_BUFFER, 0, amount_elements_to_values * sizeof(GLfloat), data_to_values.data());
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, binding_index, inp_to_values);

	block_index = glGetProgramResourceIndex(_computeProgram->GetShaderID(), GL_SHADER_STORAGE_BLOCK, "out_to_marching");


	binding_index = 0;
	//glShaderStorageBlockBinding(_computeProgram->GetShaderID(), block_index, binding_index);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, out_to_marching);
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, binding_index, out_to_marching);
	glBufferData(GL_SHADER_STORAGE_BUFFER, out_to_marching_size, NULL, GL_STATIC_DRAW);

	glDispatchCompute((GLuint)width / 1, (GLuint)height / 1, 1);
	glMemoryBarrier(GL_SHADER_STORAGE_BUFFER);

	glBindBuffer(GL_SHADER_STORAGE_BUFFER, out_to_marching);

	// ----------- SEND TO MARCHING SHADER AND MAKE TRIANGLES -------------------
	_computeProgram->Bind();

	block_index = glGetProgramResourceIndex(_computeProgram->GetShaderID(), GL_SHADER_STORAGE_BLOCK, "input0");

	binding_index = 0;
	//glShaderStorageBlockBinding(_computeProgram->GetShaderID(), block_index, binding_index);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, out_to_marching);
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, binding_index, out_to_marching);


	block_index = glGetProgramResourceIndex(_computeProgram->GetShaderID(), GL_SHADER_STORAGE_BLOCK, "MC_TriTable");

	
	binding_index = 1;
	//glShaderStorageBlockBinding(_computeProgram->GetShaderID(), block_index, binding_index);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, tri_table_input);
	glBufferData(GL_SHADER_STORAGE_BUFFER, sizeof(MC_TriTable), &MC_TriTable, GL_STREAM_COPY); // allocate 152 bytes of memory
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, binding_index, tri_table_input);

	block_index = glGetProgramResourceIndex(_computeProgram->GetShaderID(), GL_SHADER_STORAGE_BLOCK, "output0");

	
	binding_index = 2;
	glShaderStorageBlockBinding(_computeProgram->GetShaderID(), block_index, binding_index);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, output0);
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, binding_index, output0);
	glBufferData(GL_SHADER_STORAGE_BUFFER, out0_size, NULL, GL_STATIC_DRAW);



	_computeProgram->Bind();
	glDispatchCompute((GLuint)width/1, (GLuint)height/1, 1);
	glMemoryBarrier(GL_SHADER_STORAGE_BUFFER);

	// --------- ADD TO VERTEX BUFFER ---------
	GLuint VAO;
	//Generate the vertex array object
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	//Use buffer as a set of vertices
	glMemoryBarrier(GL_VERTEX_ATTRIB_ARRAY_BARRIER_BIT);
	glBindBuffer(GL_ARRAY_BUFFER, output0);

	//Enable the vertex array
	glVertexAttribPointer(0, 3, GL_FLOAT, 0, 6 * sizeof(GLfloat), (void*)0);
	glEnableVertexAttribArray(0);
	
	//Enable the vertex array
	glVertexAttribPointer(1, 3, GL_FLOAT, 0, 6 * sizeof(GLfloat), reinterpret_cast<GLvoid const*>(3*sizeof(GLfloat)));
	glEnableVertexAttribArray(1);
	
	chunkVaos[VAO] = 1;

	/*
	glDeleteBuffers(1, &output0);
	*/
	glDeleteBuffers(1, &inp_to_values);
	glDeleteBuffers(1, &out_to_marching);
	glDeleteBuffers(1, &tri_table_input);
}


void MarchingCubes::Draw(glm::mat4 view)
{
	_drawProgram->Bind();
	_drawProgram->SetUniformMatrix4("u_VP", view);
	_drawProgram->SetUniformMatrix4("u_Model",
		glm::translate(glm::mat4(1), glm::vec3(0, -3, 0))
		* glm::rotate(glm::mat4(1.0f), glm::radians(90.f), glm::vec3(0, 1, 0))
		* glm::scale(glm::mat4(1.0f), glm::vec3(5)));
	rock->Bind();
	_drawProgram->SetUniform1i("diffuse_texture", 0);
	sand->Bind(1);
	_drawProgram->SetUniform1i("grass_texture", 1);
	snow->Bind(2);
	_drawProgram->SetUniform1i("sand_texture", 3);
	grass->Bind(3);
	_drawProgram->SetUniform1i("snow_texture", 2);
	
	//Draw it
	for (auto& i : chunkVaos) {
		glBindVertexArray(i.first);
		glDrawArrays(GL_TRIANGLES, 0, res*res*3*5);
	}
	
}
