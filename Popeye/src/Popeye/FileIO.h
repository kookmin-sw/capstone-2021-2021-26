#pragma once

namespace Popeye {

	namespace fs = std::filesystem;

	enum class FileType 
	{ 
		DIR		=	0,	//	directory(folder)
		SOURCE	=	1,	//	.cpp, .h, .hpp
		IMAGE	=	2,	//	.png, .jpg ...
		MODEL	=	3,	//	.fbx, .obj ...
		TEXT	=	4	//	or else just text
	};

	struct FileData
	{
		fs::path path;
		FileType type;

		FileData();
		FileData(FileType _type, fs::path _path);
	};

	struct DirectoryData
	{
		fs::path path;
		bool hasSubDir;

		DirectoryData(fs::path _path, bool _hasSubDir);
	};

	class FileIO
	{
	private:
		bool HaveSubDir(fs::path _path);
	public:
		fs::path root;
		fs::path resource;
		fs::path curr_focused_path;

		FileIO();
		~FileIO();
		
		void Init();

		int ShowFilesAtDir(std::vector<FileData>& dirs, std::vector<FileData>& files, fs::path currPath = fs::current_path() / "Root");
		int ShowDirAtDir(std::vector<DirectoryData>& dirDats, fs::path currPath = fs::current_path() / "Root");

		void WriteDataToFile(std::string datfile, std::string dattablefile, FileData filedata);
	};
}

