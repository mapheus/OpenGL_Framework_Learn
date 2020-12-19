#pragma once
#include "MarchingCubes.h"
#include "glm/glm.hpp"

enum TerrainType {
	MARCHING = 0
};

class Terrain {
public:
	Terrain(TerrainType type, glm::vec3 size);
	void SetChunkSize(unsigned int x, unsigned int z);
	void SetPosition(glm::vec3 pos);
	void Draw(glm::mat4 proj);
private:
	void UpdateChunks();

	TerrainType _type;
	std::vector<std::vector<MarchingCubes>> _chunks;
	int _xAmount = 1;
	int _zAmount = 1;
	glm::vec3 _terrainSize = glm::vec3(40.f, 40.f, 40.f);
	glm::vec2 _chunkLength = glm::vec2(40.f, 40.f);

	glm::vec3 _pos = glm::vec3(0, 0, 0);
};