#include "FileManager.h"

namespace Popeye
{
	FileManager::FileManager() {};
	FileManager::~FileManager() {};
	
	std::vector<std::string> FileManager::AllFilesInDir() 
	{
		std::vector<std::string> files;
		root = fs::current_path() / "Root";
		for (const auto& file : fs::directory_iterator(root))
		{
			POPEYE_CORE_INFO(file.path().filename().string());
			files.push_back(file.path().filename().string());
		}

		return files;
	};
}



/*std::ifstream data("texture/test.jpg", std::ifstream::binary);
		if (data)
		{
			data.seekg(0, data.end);
			int length = (int)data.tellg();
			data.seekg(0, data.beg);

			unsigned char* buffer = (unsigned char*)malloc(length);

			data.read((char*)buffer, length);
			data.close();


			std::ofstream writedata;
			writedata.open("data.dat", std::ios::out | std::ios::binary);

			if (writedata.is_open())
			{
				writedata.write((const char*)buffer, length);
				writedata.close();
			}

		}*/
