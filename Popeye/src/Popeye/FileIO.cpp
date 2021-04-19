#include "FileIO.h"

namespace Popeye
{
	FileData::FileData() {}
	FileData::FileData(FileType _type, fs::path _path) : type(_type), path(_path) {}

	DirectoryData::DirectoryData(fs::path _path, bool _hasSubDir) : path(_path), hasSubDir(_hasSubDir) {}

	FileIO::FileIO() { root = fs::current_path() / "Root"; resource = fs::current_path() / "Resource"; curr_focused_path = root; }
	FileIO::~FileIO() {}

	void FileIO::Init()
	{
		if (!fs::exists(resource / "Resource.dat"))			{ std::ofstream out(resource / "Resource.dat");			out.close();}
		if (!fs::exists(resource / "ResourceTable.dat"))	{ std::ofstream out(resource / "ResourceTable.dat");	out.close();}
	}

	int FileIO::ShowFilesAtDir(std::vector<FileData>& dirs, std::vector<FileData>& files, fs::path currPath)
	{
		int size = 0;
		for (const auto& file : fs::directory_iterator(currPath))
		{
			FileType type = FileType::DIR;
			if (!file.is_directory())
			{
				std::string extension = file.path().filename().extension().string();
				if (!extension.compare(".cpp") || !extension.compare(".h") || !extension.compare(".hpp"))
				{
					type = FileType::SOURCE;
				}
				else if (!extension.compare(".png") || !extension.compare(".jpg"))
				{
					type = FileType::IMAGE;
				}
				else if (!extension.compare(".fbx") || !extension.compare(".obj"))
				{
					type = FileType::MODEL;
				}
				else
				{
					type = FileType::TEXT;
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
	int FileIO::ShowDirAtDir(std::vector<DirectoryData>& dirDats, fs::path currPath)
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
	
	bool FileIO::HaveSubDir(fs::path _path)
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

	//void FileIO::ReadModel(fs::path _path)
	//{
	//	/*Assimp::Importer importer;
	//	std::string path = _path.string();
	//	const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_FlipUVs | aiProcess_CalcTangentSpace);

	//	for (int i = 0; i < scene->mNumMeshes; i++)
	//	{
	//		POPEYE_CORE_INFO("Current Model Number {0}. Name : {1}", i, scene->mMeshes[i]->mName.C_Str());
	//	}*/

	//	t.detach();
	//}

	//void FileIO::ReadImage(fs::path _path)
	//{


	//	t.detach();
	//}

	//void FileIO::ReadFile(FileData filedata)
	//{
	//	//static std::thread t;
	//	if (filedata.type == FileType::MODEL)
	//	{
	//		//t = std::thread(&FileIO::ReadModel, this, filedata.path);
	//	}
	//	else if (filedata.type == FileType::IMAGE)
	//	{
	//		//t = std::thread(&FileIO::ReadImage, this, filedata.path);
	//	}
	//}

	void FileIO::WriteDataToFile(std::string datfile, std::string dattablefile, FileData filedata)
	{
		std::ifstream data(filedata.path, std::ifstream::binary);
		std::ifstream checkaddress(resource / datfile, std::ifstream::binary);
		if (data)
		{
			checkaddress.seekg(0, checkaddress.end);
			unsigned int address = (unsigned int)checkaddress.tellg();
			
			checkaddress.seekg(0, checkaddress.beg);
			
			data.seekg(0, data.end);
			unsigned int length = (unsigned int)data.tellg();
			data.seekg(0, data.beg);
			unsigned char* buffer = (unsigned char*)malloc(length);

			data.read((char*)buffer, length);
			data.close();


			std::ofstream writedata;
			writedata.open(resource / datfile, std::ios::app | std::ios::binary);

			if (writedata.is_open())
			{
				writedata.write((const char*)buffer, length);
				writedata.close();
			}

			writedata.open(resource / dattablefile, std::ios::app | std::ios::binary);
			{
				writedata.write((char*)&filedata.type, sizeof(int));
				writedata.write((char*)&address, sizeof(address));
				writedata.close();
			}

			free(buffer);
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
