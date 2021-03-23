#include "FileManager.h"

namespace Popeye
{
	FileData::FileData(FileType _type, std::string _filename) : type(_type), fileName(_filename) {}

	DirectoryData::DirectoryData(fs::path _path, bool _hasSubDir) : path(_path), hasSubDir(_hasSubDir) {}

	FileManager::FileManager() { root = fs::current_path() / "Root";}
	FileManager::~FileManager() {}
	
	std::vector<fs::path> FileManager::ShowAllDirs(fs::path directory)
	{
		std::vector<fs::path> dirs;
		for (const auto& file : fs::directory_iterator(directory))
		{
			if (file.is_directory())
			{
				dirs.push_back(file);
			}
		}

		return dirs;
	}

	//scan all files locate current dir
	int FileManager::ShowFilesAtDir(std::vector<FileData>& dirs, std::vector<FileData>& files)
	{
		std::vector<FileData> filedatas;
		int size = 0;
		for (const auto& file : fs::directory_iterator(root))
		{
			FileType type = FileType::DIR;
			std::string filename = file.path().filename().string();
			if (!file.is_directory())
			{
				std::string extension = file.path().filename().extension().string();

				if (extension.compare(".cpp") || extension.compare(".h") || extension.compare(".hpp"))
				{
					type = FileType::SOURCE;
				}
				else if (extension.compare(".png") || extension.compare(".jpg"))
				{
					type = FileType::IMAGE;
				}
				else if (extension.compare(".fbx") || extension.compare(".obj"))
				{
					type = FileType::MODEL;
				}

				files.push_back(FileData(type, filename));
			}
			else
			{
				dirs.push_back(FileData(type, filename));
			}
			size++;
		}
		return size;
	}

	int FileManager::ShowDirAtDir(std::vector<DirectoryData>& dirDats)
	{
		int size = 0;
		for (const auto& file : fs::directory_iterator(root))
		{
			if (file.is_directory())
			{
				bool subdir = HaveSubDir(file);
				dirDats.push_back(DirectoryData(file, subdir));
				size++;
			}
		}
		return size;
	}

	bool FileManager::HaveSubDir(fs::path _path)
	{
		for (const auto& file : fs::directory_iterator(_path))
		{
			if (file.is_directory())
			{
				return true;
			}
		}
		return false;
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
