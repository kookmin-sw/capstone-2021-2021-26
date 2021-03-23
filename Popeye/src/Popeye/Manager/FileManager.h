#pragma once

namespace Popeye {

	namespace fs = std::filesystem;

	enum class FileType 
	{ 
		DIR,	//	directory(folder)
		SOURCE,	//	.cpp, .h, .hpp
		IMAGE,	//	.png, .jpg ...
		MODEL,	//	.fbx, .obj ...
		TEXT,	//	or else just text
	};

	struct FileData
	{
		FileType type;
		std::string fileName;

		FileData(FileType _type, std::string _filename);
	};

	struct DirectoryData
	{
		fs::path path;
		bool hasSubDir;

		DirectoryData(fs::path _path, bool _hasSubDir);
	};

	class FileManager 
	{
		fs::path root;
		fs::path current;
	private:
		bool HaveSubDir(fs::path _path);
	public:
		FileManager();
		~FileManager();

		std::vector<fs::path> ShowAllDirs(fs::path directory = fs::current_path() / "Root");
		int ShowFilesAtDir(std::vector<FileData>& dirs, std::vector<FileData>& files);
		int ShowDirAtDir(std::vector<DirectoryData>& dirDats);
	};
}

