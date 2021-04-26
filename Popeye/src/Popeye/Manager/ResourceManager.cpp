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

			for (int i = 0; i < resource_size; i++)
			{
				unsigned int len = i + 1 == resource_size ? sizeof(char) * length - resource_address[i] : resource_address[i + 1] - resource_address[i];
				//images
				if (resource_type[i] == 2)
				{
					Texture texture;
					unsigned char* data = stbi_load_from_memory(buffer + resource_address[i], len, &texture.width, &texture.height, &texture.nrChannel, 0);

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
				//models
				else if (resource_type[i] == 3)
				{
					Assimp::Importer importer;
					const aiScene* scene = importer.ReadFileFromMemory(buffer + resource_address[i], len, aiProcess_Triangulate | aiProcess_FlipUVs);

					for (int i = 0; i < scene->mNumMeshes; i++)
					{
						aiMesh* aimesh			= scene->mMeshes[i];
						
						std::string meshName	= aimesh->mName.C_Str();
						unsigned int vertNum	= aimesh->mNumVertices;
						unsigned int faceNum	= aimesh->mNumFaces;
						aiVector3D temp = aimesh->mVertices[0];
						std::vector<unsigned int> indices;
						std::vector<float> vert;
						
						BoundBox _bounbox;
						_bounbox.maxX = temp.x;
						_bounbox.minX = temp.x;

						_bounbox.maxY = temp.y;
						_bounbox.minY = temp.y;

						_bounbox.maxZ = temp.z;
						_bounbox.minZ = temp.z;


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

							if (_bounbox.maxX < vertex.x)
								_bounbox.maxX = vertex.x;
							if (_bounbox.minX > vertex.x)
								_bounbox.minX = vertex.x;

							if (_bounbox.maxY < vertex.y)
								_bounbox.maxY = vertex.y;
							if (_bounbox.minY > vertex.y)
								_bounbox.minY = vertex.y;
							
							if (_bounbox.maxZ < vertex.z)
								_bounbox.maxZ = vertex.z;
							if (_bounbox.minZ > vertex.z)
								_bounbox.minZ = vertex.z;

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
						mesh.boundbox = _bounbox;
						POPEYE_CORE_INFO("bound size x : {0}, bound size y : {1}, bound size z : {2}", _bounbox.maxX - _bounbox.minX, _bounbox.maxY - _bounbox.minY, _bounbox.maxZ - _bounbox.minZ );
						meshes.push_back(mesh);
					}

				}

			}

			writedata.close();
			free(buffer);
		}
	}
}
