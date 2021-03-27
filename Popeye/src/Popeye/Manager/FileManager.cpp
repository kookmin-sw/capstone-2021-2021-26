#include "FileManager.h"

namespace Popeye
{
	FileData::FileData() {}
	FileData::FileData(FileType _type, fs::path _path) : type(_type), path(_path) {}

	DirectoryData::DirectoryData(fs::path _path, bool _hasSubDir) : path(_path), hasSubDir(_hasSubDir) {}

	FileManager::FileManager() { root = fs::current_path() / "Root"; curr_focused_path = root; }
	FileManager::~FileManager() {}

	//scan all files locate current dir return size
	int FileManager::ShowFilesAtDir(std::vector<FileData>& dirs, std::vector<FileData>& files, fs::path currPath)
	{
		int size = 0;
		for (const auto& file : fs::directory_iterator(currPath))
		{
			FileType type = FileType::DIR;
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

				files.push_back(FileData(type, file));
			}
			else
			{
				dirs.push_back(FileData(type, file));
			}
			size++;
		}
		return size;
	}

	//dir scan, return size
	int FileManager::ShowDirAtDir(std::vector<DirectoryData>& dirDats, fs::path currPath)
	{
		int size = 0;
		for (const auto& file : fs::directory_iterator(currPath))
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
