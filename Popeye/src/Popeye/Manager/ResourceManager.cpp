#include "ResourceManager.h"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>        
#include <assimp/postprocess.h>

#include "../Resource/Mesh.h"
#include "../Resource/Texture.h"
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
		SetTextureResources();
		Set3DModelResources();
	}


	void ResourceManager::SetTextureResources()
	{
		std::ifstream writedata;
		writedata.open("Resource/Texturestable.dat", std::ios::in | std::ios::binary);
		std::vector<unsigned int> texture_adress;
		if (writedata.is_open())
		{
			unsigned int n = 0;

			while (true)
			{
				writedata.read((char*)&n, sizeof(int));
				if (writedata.eof())
					break;

				texture_adress.push_back(n);
				POPEYE_CORE_INFO("offset : {0}", n);
			}
			writedata.close();
		}

		writedata.open("Resource/Textures.dat", std::ios::in | std::ios::binary);
		if (writedata.is_open())
		{
			writedata.seekg(0, writedata.end);
			unsigned int length = (unsigned int)writedata.tellg();
			writedata.seekg(0, writedata.beg);

			unsigned char* buffer = (unsigned char*)malloc(length);

			writedata.read((char*)buffer, length);

			int texture_size = texture_adress.size();

			for (int i = 0; i < texture_size; i++)
			{
				Texture texture;
				unsigned int len = i + 1 == texture_size ? sizeof(char) * length - texture_adress[i] : texture_adress[i + 1] - texture_adress[i];
				unsigned char* data = stbi_load_from_memory(buffer + texture_adress[i], len, &texture.width, &texture.height, &texture.nrChannel, 0);

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

			writedata.close();
			free(buffer);
		}
	}

	void ResourceManager::Set3DModelResources() 
	{
		std::ifstream writedata;
		writedata.open("Resource/Modelstable.dat", std::ios::in | std::ios::binary);
		std::vector<unsigned int> models_adress;
		if (writedata.is_open())
		{
			unsigned int n = 0;

			while (true)
			{
				writedata.read((char*)&n, sizeof(int));
				if (writedata.eof())
					break;

				models_adress.push_back(n);
				POPEYE_CORE_INFO("offset : {0}", n);
			}
			writedata.close();
		}

		writedata.open("Resource/Models.dat", std::ios::in | std::ios::binary);
		if (writedata.is_open())
		{
			writedata.seekg(0, writedata.end);
			unsigned int length = (unsigned int)writedata.tellg();
			writedata.seekg(0, writedata.beg);

			unsigned char* buffer = (unsigned char*)malloc(length);

			writedata.read((char*)buffer, length);

			int model_size = models_adress.size();

			for (int i = 0; i < model_size; i++)
			{
				unsigned int len = i + 1 == model_size ? sizeof(char) * length -models_adress[i] : models_adress[i + 1] - models_adress[i];

				Assimp::Importer importer;
				const aiScene* scene = importer.ReadFileFromMemory(buffer + models_adress[i], len, aiProcess_Triangulate | aiProcess_FlipUVs);
				
				for (int i = 0; i < scene->mNumMeshes; i++)
				{
					aiMesh* aimesh = scene->mMeshes[i];
					std::string meshName = aimesh->mName.C_Str();
					unsigned int vertNum = aimesh->mNumVertices;
					unsigned int faceNum = aimesh->mNumFaces;

					std::vector<unsigned int> indices;
					std::vector<float> vert;
					for (unsigned int j = 0; j < vertNum; j++)
					{

						aiVector3D vertex = aimesh->mVertices[j];
						vert.push_back(vertex.x); vert.push_back(vertex.y); vert.push_back(vertex.z);

						if (aimesh->mNormals != NULL)
						{
							aiVector3D normal = aimesh->mNormals[j];
							vert.push_back(normal.x); vert.push_back(normal.y); vert.push_back(normal.z);
							//vert.pos_norm_Tex[3] = normal.x;	vert.pos_norm_Tex[4] = normal.y;	vert.pos_norm_Tex[5] = normal.z;
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

					}

					for (unsigned int j = 0; j < faceNum; j++)
					{
						aiFace aiface = aimesh->mFaces[j];
						for (int k = 0; k < aiface.mNumIndices; k++)
						{
							indices.push_back(aiface.mIndices[k]);
						}
					}
					Mesh mesh(vert, indices);

					mesh.name = meshName;
					meshes.push_back(mesh);
				}
			}

			writedata.close();
			free(buffer);
		}

	}
}
