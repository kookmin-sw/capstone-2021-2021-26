#include "FileManager.h"

namespace Popeye
{
	FileManager::FileManager() { this->ScanFilesNFolders(); }
	FileManager::~FileManager() {}
	
	std::vector<std::string> FileManager::ScanFilesInDir()
	{
		std::vector<std::string> files;
		root = fs::current_path() / "Root";
		for (const auto& file : fs::directory_iterator(root))
		{
			//POPEYE_CORE_INFO(file.path().filename().string());
			files.push_back(file.path().filename().string());
		}

		return files;
	}

	void FileManager::ScanFilesNFolders()
	{
		root = fs::current_path() / "Root";
		for (const auto& file : fs::directory_iterator(root))
		{
			if (file.is_directory())
			{
				//POPEYE_CORE_INFO(file.path().filename().c_str());
				directories.push_back(file.path().filename().string());
			}
			else
			{
				files.push_back(Pfile());
			}
		}
	}
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
