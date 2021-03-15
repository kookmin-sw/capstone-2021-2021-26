#pragma once

namespace Popeye {

	namespace fs = std::filesystem;

	class FileManager 
	{
		fs::path root;
	public:
		FileManager();
		~FileManager();
		
		std::vector<std::string> AllFilesInDir();
	};
}

