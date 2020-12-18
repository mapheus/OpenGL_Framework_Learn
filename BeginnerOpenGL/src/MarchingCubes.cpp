#include "MarchingCubes.h"
#include "PerlinNoise.hpp"

#include "glm/glm.hpp"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <math.h>
#include <array>


MarchingCubes::MarchingCubes(const char* compute, const char* draw)
{
	_computeProgram = new Shader(compute, true);
	_drawProgram = new Shader(draw, false);
	rock = new Texture("res/textures/rock.jpg", "diffuse_texture");
	grass = new Texture("res/textures/grass.jpg", "grass_texture");
	sand = new Texture("res/textures/sand.jpg", "sand_texture");
	snow = new Texture("res/textures/snow.jpg", "snow_texture");
}

void MarchingCubes::UpdateTransform(float x, float y, float z, float new_cubesize)
{
	x_grid_min = x - new_cubesize /2.f;
	x_grid_max = x + new_cubesize / 2.f;

	y_grid_min = y - new_cubesize / 2.f;
	y_grid_max = y + new_cubesize / 2.f;

	z_grid_min = z - new_cubesize / 2.f;
	z_grid_max = z + new_cubesize / 2.f;
	cubesize = new_cubesize;
	CalcValues();
	printf("cybesize update: %f\n", x_grid_min);
	chunkVaos.clear();
	for (int i = 0; i < res-1; i++) {
		Update(i);
	}
}

void MarchingCubes::Update(int z)
{
	const float x_step_size = (x_grid_max - x_grid_min) / (res - 1);
	const float y_step_size = (y_grid_max - y_grid_min) / (res - 1);
	const float z_step_size = (z_grid_max - z_grid_min) / (res - 1);

	GLsizei width = static_cast<GLsizei>(res) - 1;
	GLsizei height = static_cast<GLsizei>(res) - 1;

	// res2 always 1 higher than res
	const int res2 = 33;
	glm::vec4 input_tex0_data[res2 * res2 * 2];


	int dimension = 6;

	size_t out0_size = sizeof(GLfloat**) * 3 * width * height * dimension;
	size_t out1_size = sizeof(GLfloat**) * 3 * width * height * dimension;
	size_t out2_size = sizeof(GLfloat**) * 3 * width * height * dimension;
	size_t out3_size = sizeof(GLfloat**) * 3 * width * height * dimension;
	size_t out4_size = sizeof(GLfloat**) * 3 * width * height * dimension;


	GLuint output_tex0, output_tex1, output_tex2, output_tex3, output_tex4;


	size_t index = 0;

	grid_cell temp_cube;
	for (size_t x = 0; x < width; x++)
	{
		for (size_t y = 0; y < height; y++)
		{

			size_t x_offset = 0;
			size_t y_offset = 0;
			size_t z_offset = 0;

			// Setup vertex 0
			x_offset = 0;
			y_offset = 0;
			z_offset = 0;
			temp_cube.vertex[0].x = x_grid_min + ((x + x_offset) * x_step_size);
			temp_cube.vertex[0].y = y_grid_min + ((y + y_offset) * y_step_size);
			temp_cube.vertex[0].z = z_grid_min + ((z + z_offset) * z_step_size);


			// Setup value for vertex 0
			temp_cube.value[0] = values[x + x_offset][y + y_offset][z + z_offset];

			// Setup vertex 1
			x_offset = 1;
			y_offset = 0;
			z_offset = 0;
			temp_cube.vertex[1].x = x_grid_min + ((x + x_offset) * x_step_size);
			temp_cube.vertex[1].y = y_grid_min + ((y + y_offset) * y_step_size);
			temp_cube.vertex[1].z = z_grid_min + ((z + z_offset) * z_step_size);

			// Setup value for vertex 0
			temp_cube.value[1] = values[x + x_offset][y + y_offset][z + z_offset];

			// Setup vertex 2
			x_offset = 1;
			y_offset = 0;
			z_offset = 1;
			temp_cube.vertex[2].x = x_grid_min + ((x + x_offset) * x_step_size);
			temp_cube.vertex[2].y = y_grid_min + ((y + y_offset) * y_step_size);
			temp_cube.vertex[2].z = z_grid_min + ((z + z_offset) * z_step_size);

			// Setup value for vertex 0
			temp_cube.value[2] = values[x + x_offset][y + y_offset][z + z_offset];

			// Setup vertex 3
			x_offset = 0;
			y_offset = 0;
			z_offset = 1;
			temp_cube.vertex[3].x = x_grid_min + ((x + x_offset) * x_step_size);
			temp_cube.vertex[3].y = y_grid_min + ((y + y_offset) * y_step_size);
			temp_cube.vertex[3].z = z_grid_min + ((z + z_offset) * z_step_size);

			// Setup value for vertex 0
			temp_cube.value[3] = values[x + x_offset][y + y_offset][z + z_offset];



			// Setup vertex 4
			x_offset = 0;
			y_offset = 1;
			z_offset = 0;
			temp_cube.vertex[4].x = x_grid_min + ((x + x_offset) * x_step_size);
			temp_cube.vertex[4].y = y_grid_min + ((y + y_offset) * y_step_size);
			temp_cube.vertex[4].z = z_grid_min + ((z + z_offset) * z_step_size);

			// Setup value for vertex 0
			temp_cube.value[4] = values[x + x_offset][y + y_offset][z + z_offset];

			// Setup vertex 5
			x_offset = 1;
			y_offset = 1;
			z_offset = 0;
			temp_cube.vertex[5].x = x_grid_min + ((x + x_offset) * x_step_size);
			temp_cube.vertex[5].y = y_grid_min + ((y + y_offset) * y_step_size);
			temp_cube.vertex[5].z = z_grid_min + ((z + z_offset) * z_step_size);

			// Setup value for vertex 0
			temp_cube.value[5] = values[x + x_offset][y + y_offset][z + z_offset];

			// Setup vertex 6
			x_offset = 1;
			y_offset = 1;
			z_offset = 1;
			temp_cube.vertex[6].x = x_grid_min + ((x + x_offset) * x_step_size);
			temp_cube.vertex[6].y = y_grid_min + ((y + y_offset) * y_step_size);
			temp_cube.vertex[6].z = z_grid_min + ((z + z_offset) * z_step_size);

			// Setup value for vertex 0
			temp_cube.value[6] = values[x + x_offset][y + y_offset][z + z_offset];

			// Setup vertex 7
			x_offset = 0;
			y_offset = 1;
			z_offset = 1;
			temp_cube.vertex[7].x = x_grid_min + ((x + x_offset) * x_step_size);
			temp_cube.vertex[7].y = y_grid_min + ((y + y_offset) * y_step_size);
			temp_cube.vertex[7].z = z_grid_min + ((z + z_offset) * z_step_size);

			// Setup value for vertex 0
			temp_cube.value[7] = values[x + x_offset][y + y_offset][z + z_offset];

			input_tex0_data[x + res2 * y + res2 * res2 * 0] = glm::vec4(temp_cube.vertex[0], temp_cube.value[0]);
			input_tex0_data[x + 1 + res2 * y + res2 * res2 * 0] = glm::vec4(temp_cube.vertex[1], temp_cube.value[1]);
			input_tex0_data[x + 1 + res2 * y + res2 * res2 * 1] = glm::vec4(temp_cube.vertex[2], temp_cube.value[2]);
			input_tex0_data[x + res2 * y + res2 * res2 * 1] = glm::vec4(temp_cube.vertex[3], temp_cube.value[3]);
			input_tex0_data[x + res2 * (y + 1) + res2 * res2 * 0] = glm::vec4(temp_cube.vertex[4], temp_cube.value[4]);
			input_tex0_data[x + 1 + res2 * (y + 1) + res2 * res2 * 0] = glm::vec4(temp_cube.vertex[5], temp_cube.value[5]);
			input_tex0_data[x + 1 + res2 * (y + 1) + res2 * res2 * 1] = glm::vec4(temp_cube.vertex[6], temp_cube.value[6]);
			input_tex0_data[x + res2 * (y + 1) + res2 * res2 * 1] = glm::vec4(temp_cube.vertex[7], temp_cube.value[7]);

			index += 1;
		}
	}

	_computeProgram->Bind();

	GLuint block_index = glGetProgramResourceIndex(_computeProgram->GetShaderID(), GL_SHADER_STORAGE_BLOCK, "MC_TriTable");
	unsigned int uboExampleBlock;
	glGenBuffers(1, &uboExampleBlock);
	GLuint binding_index = 7;
	glShaderStorageBlockBinding(_computeProgram->GetShaderID(), block_index, binding_index);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, uboExampleBlock);
	glBufferData(GL_SHADER_STORAGE_BUFFER, sizeof(MC_TriTable), &MC_TriTable, GL_STREAM_COPY); // allocate 152 bytes of memory
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 7, uboExampleBlock);


	block_index = glGetProgramResourceIndex(_computeProgram->GetShaderID(), GL_SHADER_STORAGE_BLOCK, "input0");
	GLuint input_tex0;
	glGenBuffers(1, &input_tex0);
	binding_index = 0;
	glShaderStorageBlockBinding(_computeProgram->GetShaderID(), block_index, binding_index);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, input_tex0);
	glBufferData(GL_SHADER_STORAGE_BUFFER, res2* res2 * 2 * sizeof(glm::vec4), &input_tex0_data, GL_DYNAMIC_DRAW);
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, binding_index, input_tex0);

	block_index = glGetProgramResourceIndex(_computeProgram->GetShaderID(), GL_SHADER_STORAGE_BLOCK, "tri_count");
	GLuint triangle_number;
	glGenBuffers(1, &triangle_number);
	binding_index = 1;
	glShaderStorageBlockBinding(_computeProgram->GetShaderID(), block_index, binding_index);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, triangle_number);
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, binding_index, triangle_number);
	glBufferData(GL_SHADER_STORAGE_BUFFER, width * height * sizeof(GLfloat), NULL, GL_STATIC_DRAW);

	block_index = glGetProgramResourceIndex(_computeProgram->GetShaderID(), GL_SHADER_STORAGE_BLOCK, "output0");
	GLuint output0;
	glGenBuffers(1, &output0);
	binding_index = 2;
	glShaderStorageBlockBinding(_computeProgram->GetShaderID(), block_index, binding_index);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, output0);
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, binding_index, output0);
	glBufferData(GL_SHADER_STORAGE_BUFFER, out0_size, NULL, GL_STATIC_DRAW);



	block_index = glGetProgramResourceIndex(_computeProgram->GetShaderID(), GL_SHADER_STORAGE_BLOCK, "output1");
	GLuint output1;
	glGenBuffers(1, &output1);
	binding_index = 3;
	glShaderStorageBlockBinding(_computeProgram->GetShaderID(), block_index, binding_index);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, output1);
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, binding_index, output1);
	glBufferData(GL_SHADER_STORAGE_BUFFER, out1_size, NULL, GL_STATIC_DRAW);

	block_index = glGetProgramResourceIndex(_computeProgram->GetShaderID(), GL_SHADER_STORAGE_BLOCK, "output2");
	GLuint output2;
	glGenBuffers(1, &output2);
	binding_index = 4;
	glShaderStorageBlockBinding(_computeProgram->GetShaderID(), block_index, binding_index);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, output2);
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, binding_index, output2);
	glBufferData(GL_SHADER_STORAGE_BUFFER, out2_size, NULL, GL_STATIC_DRAW);

	block_index = glGetProgramResourceIndex(_computeProgram->GetShaderID(), GL_SHADER_STORAGE_BLOCK, "output3");
	GLuint output3;
	glGenBuffers(1, &output3);
	binding_index = 5;
	glShaderStorageBlockBinding(_computeProgram->GetShaderID(), block_index, binding_index);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, output3);
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, binding_index, output3);
	glBufferData(GL_SHADER_STORAGE_BUFFER, out3_size, NULL, GL_STATIC_DRAW);

	block_index = glGetProgramResourceIndex(_computeProgram->GetShaderID(), GL_SHADER_STORAGE_BLOCK, "output4");
	GLuint output4;
	glGenBuffers(1, &output4);
	binding_index = 6;
	glShaderStorageBlockBinding(_computeProgram->GetShaderID(), block_index, binding_index);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, output4);
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, binding_index, output4);
	glBufferData(GL_SHADER_STORAGE_BUFFER, out4_size, NULL, GL_STATIC_DRAW);


	_computeProgram->Bind();
	glDispatchCompute((GLuint)width/1, (GLuint)height/1, 1);
	glMemoryBarrier(GL_SHADER_STORAGE_BUFFER);

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

	
	VAO;
	//Generate the vertex array object
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	//Use buffer as a set of vertices
	glMemoryBarrier(GL_VERTEX_ATTRIB_ARRAY_BARRIER_BIT);
	glBindBuffer(GL_ARRAY_BUFFER, output1);

	//Enable the vertex array
	glVertexAttribPointer(0, 3, GL_FLOAT, 0, 6 * sizeof(GLfloat), (void*)0);
	glEnableVertexAttribArray(0);

	//Enable the vertex array
	glVertexAttribPointer(1, 3, GL_FLOAT, 0, 6 * sizeof(GLfloat), reinterpret_cast<GLvoid const*>(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);

	chunkVaos[VAO] = 2;

	VAO;
	//Generate the vertex array object
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	//Use buffer as a set of vertices
	glMemoryBarrier(GL_VERTEX_ATTRIB_ARRAY_BARRIER_BIT);
	glBindBuffer(GL_ARRAY_BUFFER, output2);

	//Enable the vertex array
	glVertexAttribPointer(0, 3, GL_FLOAT, 0, 6 * sizeof(GLfloat), (void*)0);
	glEnableVertexAttribArray(0);

	
	//Enable the vertex array
	glVertexAttribPointer(1, 3, GL_FLOAT, 0, 6 * sizeof(GLfloat), reinterpret_cast<GLvoid const*>(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);
	

	chunkVaos[VAO] = 3;

	VAO;
	//Generate the vertex array object
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	//Use buffer as a set of vertices
	glMemoryBarrier(GL_VERTEX_ATTRIB_ARRAY_BARRIER_BIT);
	glBindBuffer(GL_ARRAY_BUFFER, output3);

	//Enable the vertex array
	glVertexAttribPointer(0, 3, GL_FLOAT, 0, 6 * sizeof(GLfloat), (void*)0);
	glEnableVertexAttribArray(0);

	//Enable the vertex array
	glVertexAttribPointer(1, 3, GL_FLOAT, 0, 6 * sizeof(GLfloat), reinterpret_cast<GLvoid const*>(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);

	chunkVaos[VAO] = 4;

	VAO;
	//Generate the vertex array object
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	//Use buffer as a set of vertices
	glMemoryBarrier(GL_VERTEX_ATTRIB_ARRAY_BARRIER_BIT);
	glBindBuffer(GL_ARRAY_BUFFER, output4);

	//Enable the vertex array
	glVertexAttribPointer(0, 3, GL_FLOAT, 0, 6 * sizeof(GLfloat), (void*)0);
	glEnableVertexAttribArray(0);

	//Enable the vertex array
	glVertexAttribPointer(1, 3, GL_FLOAT, 0, 6 * sizeof(GLfloat), reinterpret_cast<GLvoid const*>(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);

	chunkVaos[VAO] = 5;
	
}


void MarchingCubes::Draw(glm::mat4 proj, glm::mat4 view)
{
	_drawProgram->Bind();
	_drawProgram->SetUniformMatrix4("u_VP", proj * view);
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
		//printf("%i\n", i.second );
		glDrawArrays(GL_TRIANGLES, 0, res*res*3);
	}
	
}

void MarchingCubes::CalcValues()
{
	values.clear();
	FastNoiseLite noise;
	noise.SetNoiseType(FastNoiseLite::NoiseType_OpenSimplex2S);

	double step = cubesize / (res - 1);

	float v = 0;
	for (std::int32_t x = 0; x < res; x++) {
		values.push_back(std::vector<std::vector<float>>());

		for (std::int32_t y = 0; y < res; y++) {
			values[x].push_back(std::vector<float>());
			for (std::int32_t z = 0; z < res; z++) {
				v = noise.GetNoise((float)(x_grid_min + x * step) * scale, (float)(y_grid_min + y * step) * scale , (float)(z_grid_min + z * step) * scale);
				v *= 25;
				
				if (y < res / 2) {
					v -= 5+fabs(y - res / 2) * 2;
				}
				else {
					v += fabs(y - res / 2) * 5;
				}

				//printf("%f\n", v);
				values[x][y].push_back(v);
				//values[x][y].push_back(rand() % 10 - 5);
			}
		}
	}
}
