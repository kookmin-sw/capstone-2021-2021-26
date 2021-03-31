#include "ResourceManager.h"

#include "../Resource/Mesh.h"
#include "../Resource/Texture.h"
#include "stb_image.h"


namespace Popeye 
{
	void ResourceManager::SetResources()
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
				Textre texture;
				unsigned int len = i + 1 == texture_size ? sizeof(buffer) * length : texture_adress[i + 1] - texture_adress[i];
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


		//writedata.open("Resource/Textures.dat",  std::ios::binary);

	}
}
