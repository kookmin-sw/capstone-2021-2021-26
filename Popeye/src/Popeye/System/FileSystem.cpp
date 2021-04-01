#include "FileSystem.h"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>        
#include <assimp/postprocess.h>

namespace Popeye
{
	FileData::FileData() {}
	FileData::FileData(FileType _type, fs::path _path) : type(_type), path(_path) {}

	DirectoryData::DirectoryData(fs::path _path, bool _hasSubDir) : path(_path), hasSubDir(_hasSubDir) {}

	FileSystem::FileSystem() { root = fs::current_path() / "Root"; resource = fs::current_path() / "Resource"; curr_focused_path = root; }
	FileSystem::~FileSystem() {}

	void FileSystem::Init()
	{
		if (!fs::exists(resource / "Models.dat"))			{ std::ofstream out(resource / "Models.dat");			out.close();}
		if (!fs::exists(resource / "Modelstable.dat"))		{ std::ofstream out(resource / "Modelstable.dat");		out.close();}
		if (!fs::exists(resource / "Textures.dat"))			{ std::ofstream out(resource / "Textures.dat");			out.close();}
		if (!fs::exists(resource / "Texturestable.dat"))	{ std::ofstream out(resource / "Texturestable.dat");	out.close();}
		if (!fs::exists(resource / "Materials.dat"))		{ std::ofstream out(resource / "Materials.dat");		out.close();}
		if (!fs::exists(resource / "Materialstable.dat"))	{ std::ofstream out(resource / "Materialstable.dat");	out.close();}
	}

	//scan all files locate current dir return size
	int FileSystem::ShowFilesAtDir(std::vector<FileData>& dirs, std::vector<FileData>& files, fs::path currPath)
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
	int FileSystem::ShowDirAtDir(std::vector<DirectoryData>& dirDats, fs::path currPath)
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
	
	bool FileSystem::HaveSubDir(fs::path _path)
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

	void FileSystem::ReadModel(fs::path _path)
	{
		/*Assimp::Importer importer;
		std::string path = _path.string();
		const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_FlipUVs | aiProcess_CalcTangentSpace);

		for (int i = 0; i < scene->mNumMeshes; i++)
		{
			POPEYE_CORE_INFO("Current Model Number {0}. Name : {1}", i, scene->mMeshes[i]->mName.C_Str());
		}*/

		t.detach();
	}

	void FileSystem::ReadImage(fs::path _path)
	{


		t.detach();
	}

	void FileSystem::ReadFile(FileData filedata)
	{
		//static std::thread t;
		if (filedata.type == FileType::MODEL)
		{
			t = std::thread(&FileSystem::ReadModel, this, filedata.path);
		}
		else if (filedata.type == FileType::IMAGE)
		{
			t = std::thread(&FileSystem::ReadImage, this, filedata.path);
		}
	}

	void FileSystem::WriteDataToFile(std::string datfile, std::string dattablefile, fs::path filepath)
	{
		std::ifstream data(filepath, std::ifstream::binary);
		std::ifstream checkaddress(resource / datfile, std::ifstream::binary);
		if (data)
		{
			checkaddress.seekg(0, checkaddress.end);
			unsigned int address = (unsigned int)checkaddress.tellg();
			
			checkaddress.seekg(0, checkaddress.beg);
			POPEYE_CORE_INFO("address : {0}", address);
			
			data.seekg(0, data.end);
			unsigned int length = (unsigned int)data.tellg();
			data.seekg(0, data.beg);
			POPEYE_CORE_INFO(length);
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
