#include "ResourceManager.h"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>        
#include <assimp/postprocess.h>

#include "stb_image.h"

namespace Popeye 
{
	ResourceManager::ResourceManager()
	{
		materials.push_back(Material());
	}

	ResourceManager::~ResourceManager()
	{

	}

	void ResourceManager::SetResources()
	{
		std::ifstream writedata;
		writedata.open("Resource/ResourceTable.dat", std::ios::in | std::ios::binary);
		std::vector<unsigned int> resource_address;
		std::vector<unsigned int> resource_type;
		if (writedata.is_open())
		{
			unsigned int address = 0;
			int type = 0;
			while (true)
			{
				writedata.read((char*)&type, sizeof(int));
				writedata.read((char*)&address, sizeof(int));
				if (writedata.eof())
					break;

				resource_address.push_back(address);
				resource_type.push_back(type);
			}
			writedata.close();
		}

		writedata.open("Resource/Resource.dat", std::ios::in | std::ios::binary);
		if (writedata.is_open())
		{
			writedata.seekg(0, writedata.end);
			unsigned int length = (unsigned int)writedata.tellg();
			writedata.seekg(0, writedata.beg);

			unsigned char* buffer = (unsigned char*)malloc(length);

			writedata.read((char*)buffer, length);

			int resource_size = resource_address.size();

			// MUST FIX HERE FIRTS WHEN CHANGE RESOURCE MANAGER !!
			// MUST FIX HERE FIRTS WHEN CHANGE RESOURCE MANAGER !!
			for (int i = 0; i < resource_size; i++)
			{
				if (i >= offsets.size())
				{
					unsigned int len = i + 1 == resource_size ? sizeof(char) * length - resource_address[i] : resource_address[i + 1] - resource_address[i];
					switch (resource_type[i])
					{
					case 2:
						SetTexture(buffer, resource_address[i], len);
						break;
					case 3:
						SetModel(buffer, resource_address[i], len);
						break;
					}
				}
			}

			if (resource_size >= offsets.size())
			{
				int adding_size = resource_size - offsets.size();
				for (int i = 0; i < adding_size; i++)
				{
					offsets.push_back(resource_address[i]);
				}

			}


			writedata.close();
			free(buffer);
		}
	}


	void ResourceManager::SetTexture(unsigned char* buffer, unsigned int offset, unsigned int length)
	{
		Texture texture;
		unsigned char* data = stbi_load_from_memory(buffer + offset, length, &texture.width, &texture.height, &texture.nrChannel, 0);

		glGenTextures(1, &texture.id);
		glBindTexture(GL_TEXTURE_2D, texture.id);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, texture.width, texture.height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);

		textures.push_back(texture);

		stbi_image_free(data);
	}

	void ResourceManager::SetModel(unsigned char* buffer, unsigned int offset, unsigned int length)
	{
		Assimp::Importer importer;
		const aiScene* scene = importer.ReadFileFromMemory(buffer + offset, length, aiProcess_Triangulate | aiProcess_FlipUVs);
		Model model;
		SetMesh(scene);
		SetModelRecursively(scene->mRootNode, scene, model);
		aiMesh* mesh = scene->mMeshes[0];
		model.name = mesh->mName.C_Str();
		model.SetBoundBox();
		models.push_back(model);
	}

	void ResourceManager::SetModelRecursively(aiNode* node, const aiScene* scene, Model& model)
	{
		
		for (unsigned int i = 0; i < node->mNumMeshes; i++)
		{
			aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
			int mesh_size = meshes.size();
			SetMesh(mesh, model);
		}

		for (unsigned int i = 0; i < node->mNumChildren; i++)
		{
			SetModelRecursively(node->mChildren[i], scene, model);
		}
	}
	
	/*bool ResourceManager::CheckMesh(float *id, int &index)
	{
		int size = meshes.size();
		for (int i = 0; i < size; i++)
		{
			if (id == meshes[i].id)
			{
				index = i;
				return true;
			}
		}
		return false;
	}*/

	void ResourceManager::SetMesh(aiMesh* _aimesh, Model& model)
	{
		aiMesh* aimesh = _aimesh;
		std::string meshName = aimesh->mName.C_Str();

		unsigned int vertNum = aimesh->mNumVertices;
		unsigned int faceNum = aimesh->mNumFaces;
		aiVector3D temp = aimesh->mVertices[0];
		float *id = &aimesh->mVertices[0].x;
		//int index = _index;

		std::vector<unsigned int> indices;
		std::vector<float> vert;

		BoundBox bounbox;
		bounbox.maxPos = glm::vec3(temp.x, temp.y, temp.z);
		bounbox.minPos = glm::vec3(temp.x, temp.y, temp.z);


		for (unsigned int j = 0; j < vertNum; j++)
		{
			aiVector3D vertex = aimesh->mVertices[j];
			vert.push_back(vertex.x); vert.push_back(vertex.y); vert.push_back(vertex.z);

			if (aimesh->mNormals != NULL)
			{
				aiVector3D normal = aimesh->mNormals[j];
				vert.push_back(normal.x); vert.push_back(normal.y); vert.push_back(normal.z);
			}
			else
			{
				vert.push_back(0); vert.push_back(0); vert.push_back(0);
			}

			if (aimesh->mTextureCoords[0] != NULL)
			{
				aiVector3D texcoord = aimesh->mTextureCoords[0][j];
				vert.push_back(texcoord.x); vert.push_back(texcoord.y);
			}
			else
			{
				vert.push_back(0); vert.push_back(0);
			}

			if (bounbox.maxPos.x < vertex.x)
				bounbox.maxPos.x = vertex.x;
			if (bounbox.minPos.x > vertex.x)
				bounbox.minPos.x = vertex.x;

			if (bounbox.maxPos.y < vertex.y)
				bounbox.maxPos.y = vertex.y;
			if (bounbox.minPos.y > vertex.y)
				bounbox.minPos.y = vertex.y;

			if (bounbox.maxPos.z < vertex.z)
				bounbox.maxPos.z = vertex.z;
			if (bounbox.minPos.z > vertex.z)
				bounbox.minPos.z = vertex.z;

		}

		for (unsigned int j = 0; j < faceNum; j++)
		{
			aiFace aiface = aimesh->mFaces[j];
			for (int k = 0; k < aiface.mNumIndices; k++)
			{
				indices.push_back(aiface.mIndices[k]);
			}
		}
		Mesh mesh(vert, indices, bounbox);
		mesh.name = meshName;
		mesh.id = id;
		model.meshes.push_back(mesh);
	}

	void ResourceManager::SetMesh(const aiScene* scene)
	{
		int mesh_size = scene->mNumMeshes;
		for (int i = 0; i < mesh_size; i++)
		{
			aiMesh* aimesh = scene->mMeshes[i];
			std::string meshName = aimesh->mName.C_Str();

			unsigned int vertNum = aimesh->mNumVertices;
			unsigned int faceNum = aimesh->mNumFaces;
			aiVector3D temp = aimesh->mVertices[0];
			std::vector<unsigned int> indices;
			std::vector<float> vert;

			BoundBox bounbox;
			bounbox.maxPos = glm::vec3(temp.x, temp.y, temp.z);
			bounbox.minPos = glm::vec3(temp.x, temp.y, temp.z);


			for (unsigned int j = 0; j < vertNum; j++)
			{
				aiVector3D vertex = aimesh->mVertices[j];
				vert.push_back(vertex.x); vert.push_back(vertex.y); vert.push_back(vertex.z);

				if (aimesh->mNormals != NULL)
				{
					aiVector3D normal = aimesh->mNormals[j];
					vert.push_back(normal.x); vert.push_back(normal.y); vert.push_back(normal.z);
				}
				else
				{
					vert.push_back(0); vert.push_back(0); vert.push_back(0);
				}

				if (aimesh->mTextureCoords[0] != NULL)
				{
					aiVector3D texcoord = aimesh->mTextureCoords[0][j];
					vert.push_back(texcoord.x); vert.push_back(texcoord.y);
				}
				else
				{
					vert.push_back(0); vert.push_back(0);
				}

				if (bounbox.maxPos.x < vertex.x)
					bounbox.maxPos.x = vertex.x;
				if (bounbox.minPos.x > vertex.x)
					bounbox.minPos.x = vertex.x;

				if (bounbox.maxPos.y < vertex.y)
					bounbox.maxPos.y = vertex.y;
				if (bounbox.minPos.y > vertex.y)
					bounbox.minPos.y = vertex.y;

				if (bounbox.maxPos.z < vertex.z)
					bounbox.maxPos.z = vertex.z;
				if (bounbox.minPos.z > vertex.z)
					bounbox.minPos.z = vertex.z;

			}

			for (unsigned int j = 0; j < faceNum; j++)
			{
				aiFace aiface = aimesh->mFaces[j];
				for (int k = 0; k < aiface.mNumIndices; k++)
				{
					indices.push_back(aiface.mIndices[k]);
				}
			}
			Mesh mesh(vert, indices, bounbox);
			mesh.name = meshName;
			meshes.push_back(mesh);
		}
	}
	
	void ResourceManager::SetMesh(unsigned char* buffer, unsigned int offset, unsigned int length)
	{
		Assimp::Importer importer;
		const aiScene* scene = importer.ReadFileFromMemory(buffer + offset, length, aiProcess_Triangulate | aiProcess_FlipUVs);

		for (int i = 0; i < scene->mNumMeshes; i++)
		{
			aiMesh* aimesh = scene->mMeshes[i];

			std::string meshName = aimesh->mName.C_Str();

			unsigned int vertNum = aimesh->mNumVertices;
			unsigned int faceNum = aimesh->mNumFaces;
			aiVector3D temp = aimesh->mVertices[0];
			std::vector<unsigned int> indices;
			std::vector<float> vert;

			BoundBox bounbox;
			bounbox.maxPos = glm::vec3(temp.x, temp.y, temp.z);
			bounbox.minPos = glm::vec3(temp.x, temp.y, temp.z);


			for (unsigned int j = 0; j < vertNum; j++)
			{
				aiVector3D vertex = aimesh->mVertices[j];
				vert.push_back(vertex.x); vert.push_back(vertex.y); vert.push_back(vertex.z);

				if (aimesh->mNormals != NULL)
				{
					aiVector3D normal = aimesh->mNormals[j];
					vert.push_back(normal.x); vert.push_back(normal.y); vert.push_back(normal.z);
				}
				else
				{
					vert.push_back(0); vert.push_back(0); vert.push_back(0);
				}

				if (aimesh->mTextureCoords[0] != NULL)
				{
					aiVector3D texcoord = aimesh->mTextureCoords[0][j];
					vert.push_back(texcoord.x); vert.push_back(texcoord.y);
				}
				else
				{
					vert.push_back(0); vert.push_back(0);
				}

				if (bounbox.maxPos.x < vertex.x)
					bounbox.maxPos.x = vertex.x;
				if (bounbox.minPos.x > vertex.x)
					bounbox.minPos.x = vertex.x;

				if (bounbox.maxPos.y < vertex.y)
					bounbox.maxPos.y = vertex.y;
				if (bounbox.minPos.y > vertex.y)
					bounbox.minPos.y = vertex.y;

				if (bounbox.maxPos.z < vertex.z)
					bounbox.maxPos.z = vertex.z;
				if (bounbox.minPos.z > vertex.z)
					bounbox.minPos.z = vertex.z;

			}

			for (unsigned int j = 0; j < faceNum; j++)
			{
				aiFace aiface = aimesh->mFaces[j];
				for (int k = 0; k < aiface.mNumIndices; k++)
				{
					indices.push_back(aiface.mIndices[k]);
				}
			}
			Mesh mesh(vert, indices, bounbox);
			mesh.name = meshName;
			meshes.push_back(mesh);
		}
	}
	
	void ResourceManager::SetMaterial(unsigned char* buffer, unsigned int offset, unsigned int length)
	{
		Material material;

	}

	void ResourceManager::AddMaterial()
	{
		materials.push_back(Material("material" + std::to_string(materials.size())));
	}
}
