#pragma once

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <GL/glew.h>

#include <vector>
#include <tuple>

#include "vertex.h"
#include "texture.h"
#include "mathematics.h"

class Mesh
{
public:
	Mesh();

	~Mesh();

	bool loadMesh(const std::string& fileName, int meshNumber_, bool transparency_);

	void render(vec3 camPos_, mat4 transform_);
private:
	bool initFromScene(const aiScene* pScene, const std::string& fileName);
	void initMesh(unsigned index, const aiMesh* paiMesh);
	bool initMaterials(const aiScene* pScene, const std::string& fileName);
	void clear();

	struct MeshEntry {
		MeshEntry();

		~MeshEntry();

		bool init(const std::vector <Vertex>& vertices, const std::vector <unsigned int>& indices);

		GLuint VB;
		GLuint IB;
		unsigned int numIndices;
		unsigned int materialIndex;
	};

	std::vector <MeshEntry> entries;
	std::vector <Texture*> textures;
	int meshNumber;
	bool transparency;
	std::vector<Vertex> vertices_list;
	std::vector<std::tuple<unsigned, unsigned, unsigned> > indices_list;
	vec3 camPos;
	mat4 transform;
};

bool cmp(std::tuple<unsigned, unsigned, unsigned> t1, std::tuple<unsigned, unsigned, unsigned> t2);