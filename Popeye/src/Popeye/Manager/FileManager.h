#pragma once

namespace Popeye {

	namespace fs = std::filesystem;

	enum class FileType 
	{ 
		DIR,	//	directory(folder)
		TEXT,	//	.txt
		SOURCE,	//	.cpp
		HEADER, //	.h, .hpp
		IMAGE,	//	.png, jpg ...
		MODEL	//	.fbx, obj ...
	};

	struct Pfile
	{
		FileType type;
		std::string pfile;

		Pfile();
	};

	class FileManager 
	{
		fs::path root;

		std::vector<fs::path> directories;
		std::vector<Pfile> files;
	public:
		FileManager();
		~FileManager();
		
		std::vector<std::string> ScanDirs();
		std::vector<std::string> ScanFilesInDir();
		//std::vector<Pfile> ScanFilesInDir();

		void ScanFilesNFolders();
	};
}

