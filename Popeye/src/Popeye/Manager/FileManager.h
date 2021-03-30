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

	class FileManager 
	{
	private:
		std::thread t;
		bool HaveSubDir(fs::path _path);
		void ReadModel(fs::path _path);
		void ReadImage(fs::path _path);
	public:
		fs::path root;
		fs::path curr_focused_path;

		FileManager();
		~FileManager();

		//std::vector<fs::path> ShowAllDirs(fs::path directory = fs::current_path() / "Root");
		int ShowFilesAtDir(std::vector<FileData>& dirs, std::vector<FileData>& files, fs::path currPath = fs::current_path() / "Root");
		int ShowDirAtDir(std::vector<DirectoryData>& dirDats, fs::path currPath = fs::current_path() / "Root");

		void ReadFile(FileData filedata);
	};
}

