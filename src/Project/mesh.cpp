#include "mesh.h"

Vertex *vertices;
vec3 currCamPos;
mat4 currTransform;

Mesh::Mesh()
{
}

Mesh::~Mesh()
{
	clear();
}

bool Mesh::loadMesh(const std::string & fileName, int meshNumber_, bool transparency_)
{
	clear();
	transparency = transparency_;
	//std::cout << "Set " << transparency << std::endl;
	meshNumber = meshNumber_;

	bool ret = 0;
	Assimp::Importer Importer;

	const aiScene* pScene = Importer.ReadFile(fileName.c_str(), aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_FlipUVs);

	if (pScene) {
		ret = initFromScene(pScene, fileName);
	}
	else {
		printf("Error parsing '%s': '%s'\n", fileName.c_str(), Importer.GetErrorString());
	}

	return ret;
}

void Mesh::render(vec3 camPos_, mat4 transform_)
{
	camPos = camPos_;
	transform = transform_;
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);

	if (!transparency) {
		if (meshNumber == -1) {
			for (unsigned int i = 0; i < entries.size(); i++) {
				glBindBuffer(GL_ARRAY_BUFFER, entries[i].VB);
				glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);
				glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const GLvoid*)12);
				glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const GLvoid*)20);

				glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, entries[i].IB);

				unsigned materialIndex = entries[i].materialIndex;

				if (materialIndex < textures.size() && textures[materialIndex]) {
					textures[materialIndex]->bind(GL_TEXTURE0);
				}

				glDrawElements(GL_TRIANGLES, entries[i].numIndices, GL_UNSIGNED_INT, 0);
			}
		}
		else {
			glBindBuffer(GL_ARRAY_BUFFER, entries[meshNumber].VB);
			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);
			glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const GLvoid*)12);
			glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const GLvoid*)20);

			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, entries[meshNumber].IB);

			unsigned materialIndex = entries[meshNumber].materialIndex;

			if (materialIndex < textures.size() && textures[materialIndex]) {
				textures[materialIndex]->bind(GL_TEXTURE0);
			}

			glDrawElements(GL_TRIANGLES, entries[meshNumber].numIndices, GL_UNSIGNED_INT, 0);
		}
	}
	else {
		currCamPos = camPos;
		currTransform = transform;
		vertices = vertices_list.data();
		std::sort(indices_list.begin(), indices_list.end(), cmp);

		//std::cout << indices_list.size() << std::endl;

		GLuint VB_tmp;

		/*for (int i = 0; i < (int)vertices_list.size(); i++) {
			std::cout << vertices_list[i].position.x << ' ' << vertices_list[i].position.y << ' ' << vertices_list[i].position.z << std::endl;
		}*/

		//std::cout << indices[0] << ' ' << indices[1] << ' ' << indices[2] << std::endl << std::endl;

		std::vector <Vertex> vertices_curr;
		for (int i = 0; i < (int)indices_list.size(); i++) {
			vertices_curr.push_back(vertices_list[std::get<0>(indices_list[i])]);
			vertices_curr.push_back(vertices_list[std::get<1>(indices_list[i])]);
			vertices_curr.push_back(vertices_list[std::get<2>(indices_list[i])]);
		}

		glGenBuffers(1, &VB_tmp);
		glBindBuffer(GL_ARRAY_BUFFER, VB_tmp);
		glBufferData(GL_ARRAY_BUFFER, vertices_curr.size() * sizeof(Vertex), vertices_curr.data(), GL_STATIC_DRAW);

		glEnableVertexAttribArray(0);
		glEnableVertexAttribArray(2);
		glBindBuffer(GL_ARRAY_BUFFER, VB_tmp);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);
		glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const GLvoid*)20);
		glDrawArrays(GL_TRIANGLES, 0, vertices_curr.size());
		glDisableVertexAttribArray(0);
		glDisableVertexAttribArray(2);

		glDeleteBuffers(1, &VB_tmp);
	}
	//////

	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(2);
}

bool Mesh::initFromScene(const aiScene * pScene, const std::string & fileName)
{
	vertices_list.clear();
	indices_list.clear();
	entries.resize(pScene->mNumMeshes);
	textures.resize(pScene->mNumMaterials);

	for (unsigned i = 0; i < entries.size(); i++) {
		const aiMesh* paiMesh = pScene->mMeshes[i];
		initMesh(i, paiMesh);
	}

	return initMaterials(pScene, fileName);
}

void Mesh::initMesh(unsigned index, const aiMesh * paiMesh)
{
	int k = vertices_list.size();
	entries[index].materialIndex = paiMesh->mMaterialIndex;

	std::vector<Vertex> vertices;
	std::vector<unsigned> indices;

	const aiVector3D zero3D(0.0f, 0.0f, 0.0f);

	for (unsigned int i = 0; i < paiMesh->mNumVertices; i++) {
		const aiVector3D* pPos = &(paiMesh->mVertices[i]);
		const aiVector3D* pNormal = &(paiMesh->mNormals[i]);
		const aiVector3D* pTexCoord = paiMesh->HasTextureCoords(0) ? &(paiMesh->mTextureCoords[0][i]) : &zero3D;

		Vertex v(vec3(pPos->x, pPos->y, pPos->z), vec2(pTexCoord->x, pTexCoord->y), vec3(pNormal->x, pNormal->y, pNormal->z));

		vertices.push_back(v);
		if (transparency) {
			vertices_list.push_back(v);
		}
	}

	//std::cout << "initMesh" << std::endl;
	//std::cout << "Transparency " << transparency << std::endl;
	for (unsigned i = 0; i < paiMesh->mNumFaces; i++) {
		const aiFace& Face = paiMesh->mFaces[i];
		assert(Face.mNumIndices == 3);
		indices.push_back(Face.mIndices[0]);
		indices.push_back(Face.mIndices[1]);
		indices.push_back(Face.mIndices[2]);
		if (transparency) {
			indices_list.push_back(std::make_tuple(Face.mIndices[0] + k, Face.mIndices[1] + k, Face.mIndices[2] + k));
		}
	}

	entries[index].init(vertices, indices);
}

bool Mesh::initMaterials(const aiScene * pScene, const std::string & fileName)
{
	std::string::size_type slashIndex = fileName.find_last_of("/");
	std::string dir;

	if (slashIndex == std::string::npos) {
		dir = ".";
	}
	else if (slashIndex == 0) {
		dir = "/";
	}
	else {
		dir = fileName.substr(0, slashIndex);
	}

	bool ret = true;

	for (unsigned int i = 0; i < pScene->mNumMaterials; i++) {
		const aiMaterial* pMaterial = pScene->mMaterials[i];
		textures[i] = NULL;

		if (pMaterial->GetTextureCount(aiTextureType_DIFFUSE) > 0) {
			aiString path;

			if (pMaterial->GetTexture(aiTextureType_DIFFUSE, 0, &path, NULL, NULL, NULL, NULL, NULL) == AI_SUCCESS) {
				std::string fullPath = dir + "/" + path.data;
				textures[i] = new Texture(GL_TEXTURE_2D, fullPath.c_str());

				if (!textures[i]->load()) {
					printf("Error loading texture '%s'\n", fullPath.c_str());
					delete textures[i];
					textures[i] = NULL;
					ret = false;
				}
			}
		}
		if (!textures[i]) {
			textures[i] = new Texture(GL_TEXTURE_2D, "textures/white.bmp");
			ret = textures[i]->load();
		}
	}

	return ret;
}

void Mesh::clear()
{
	for (unsigned int i = 0; i < textures.size(); i++) {
		if (textures[i] != NULL) {
			delete textures[i];
		}
	}
}

bool cmp(std::tuple<unsigned, unsigned, unsigned> t1, std::tuple<unsigned, unsigned, unsigned> t2)
{
	vec4 p11, p12, p13;
	vec4 p21, p22, p23;

	p11 = currTransform * vec4(vertices[std::get<0>(t1)].position.x, vertices[std::get<0>(t1)].position.y, vertices[std::get<0>(t1)].position.z, 1.0);
	p12 = currTransform * vec4(vertices[std::get<1>(t1)].position.x, vertices[std::get<1>(t1)].position.y, vertices[std::get<1>(t1)].position.z, 1.0);
	p13 = currTransform * vec4(vertices[std::get<2>(t1)].position.x, vertices[std::get<2>(t1)].position.y, vertices[std::get<2>(t1)].position.z, 1.0);

	p21 = currTransform * vec4(vertices[std::get<0>(t2)].position.x, vertices[std::get<0>(t2)].position.y, vertices[std::get<0>(t2)].position.z, 1.0);
	p22 = currTransform * vec4(vertices[std::get<1>(t2)].position.x, vertices[std::get<1>(t2)].position.y, vertices[std::get<1>(t2)].position.z, 1.0);
	p23 = currTransform * vec4(vertices[std::get<2>(t2)].position.x, vertices[std::get<2>(t2)].position.y, vertices[std::get<2>(t2)].position.z, 1.0);

	vec4 m1_tmp = (p11 + p12 + p13) / 3;
	vec4 m2_tmp = (p21 + p22 + p23) / 3;

	vec3 m1 = vec3(m1_tmp.x, m1_tmp.y, m1_tmp.z);
	vec3 m2 = vec3(m2_tmp.x, m2_tmp.y, m2_tmp.z);

	return (m1 - currCamPos).length() > (m2 - currCamPos).length();
}

Mesh::MeshEntry::MeshEntry()
{
	VB = -1;
	IB = -1;
	numIndices = 0;
	materialIndex = -1;
}

Mesh::MeshEntry::~MeshEntry()
{
	if (VB != -1) {
		glDeleteBuffers(1, &VB);
	}

	if (IB != -1) {
		glDeleteBuffers(1, &IB);
	}
}

bool Mesh::MeshEntry::init(const std::vector<Vertex>& vertices, const std::vector<unsigned int>& indices)
{
	numIndices = indices.size();

	glGenBuffers(1, &VB);
	glBindBuffer(GL_ARRAY_BUFFER, VB);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex)*vertices.size(), &vertices[0], GL_STATIC_DRAW);

	glGenBuffers(1, &IB);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IB);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int)*numIndices, &indices[0], GL_STATIC_DRAW);

	return 1;
}
