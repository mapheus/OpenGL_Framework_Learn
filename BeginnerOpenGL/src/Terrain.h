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
	void SetTerrainSize(glm::vec3 size);
	void Draw(glm::mat4 proj);
	void Update();

	float* GetIsovalue() { return &isovalue; }
	float* GetScale() { return &scale; }
	float* GetGain() { return &gain; }
	float* GetLacunarity() { return &lacunarity; }
	float* GetAmplitude() { return &amplitude; }
	float* GetAirDensity() { return &air_density; }
	float* GetGroundDensity() { return &ground_density; }
	float* GetSurfaceLevel() { return &surface_level; }
	int* GetOctaves() { return &octaves; }
	int* GetResolution() { return &res; }

	glm::vec3 GetTerrainPosition();
private:
	void UpdateChunks();

	TerrainType _type;
	std::vector<std::vector<MarchingCubes>> _chunks;
	int _xAmount = 1;
	int _zAmount = 1;
	glm::vec3 _terrainSize = glm::vec3(40.f, 40.f, 40.f);
	glm::vec2 _chunkLength = glm::vec2(40.f, 40.f);

	glm::vec3 _pos = glm::vec3(0, 0, 0);



	// MARCHING STUFF
	int res = 32;
	float isovalue = 0;
	float surface_level = 0;
	float scale = 0.3f;
	float gain = 0.6f;
	float lacunarity = 1.3f;
	float amplitude = 2.f;
	float air_density = 3.f;
	float ground_density = 4.f;
	int octaves = 1;
};