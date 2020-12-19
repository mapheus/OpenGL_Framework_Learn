#include "Terrain.h"

#pragma once

Terrain::Terrain(TerrainType type, glm::vec3 size)
	: _type(type), _terrainSize(size)
{
}

void Terrain::SetChunkSize(unsigned int x, unsigned int z)
{
	_xAmount = x;
	_zAmount = z;
	_chunkLength = glm::vec2((float)_terrainSize.x / (float)_xAmount, _terrainSize.z / (float)_zAmount);
	_chunks.clear();
	for (int i = 0; i < _xAmount; i++) {
		_chunks.push_back(std::vector<MarchingCubes>());
		for (int j = 0; j < _zAmount; j++)
		{
			_chunks[i].emplace_back("res/shaders/ctest2.comp", "res/shaders/calcValues.comp", "res/shaders/marching.shader");
		}
	}
	UpdateChunks();
}

void Terrain::SetPosition(glm::vec3 pos)
{
	_pos = pos;
	UpdateChunks();
}

void Terrain::UpdateChunks()
{
	for (int i = 0; i < _xAmount; i++) {
		for (int j = 0; j < _zAmount; j++) {
			float x =(_pos.x - ((float)(_xAmount - 1) / 2)*_chunkLength.x) + _chunkLength.x * i;
			float z = (_pos.z - ((float)(_zAmount - 1) / 2) * _chunkLength.y) + _chunkLength.y * j;
			//printf("x : z           : %f   :   %f", x, z);
			_chunks[i][j].UpdateTransform(glm::vec3(x, _pos.y, z), glm::vec3(_chunkLength.x, _terrainSize.y, _chunkLength.y));
		}
	}
}

void Terrain::Draw(glm::mat4 proj)
{
	for (int i = 0; i < _xAmount; i++) {
		for (int j = 0; j < _zAmount; j++) {

			_chunks[i][j].Draw(proj);
		}
	}
}