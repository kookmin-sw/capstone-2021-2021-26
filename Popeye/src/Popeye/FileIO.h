#pragma once

namespace Popeye {

	namespace fs = std::filesystem;
	
	class Scene;
	class GameObject;
	struct Accessor;

	struct Camera;
	struct MeshRenderer;
	struct Light;

	struct BoxCollider;
	struct Rigidbody;

	struct UIFrame;
	struct Text;
	struct Button;

	enum class FileType 
	{ 
		DIR		=	0,	//	directory(folder)
		SOURCE	=	1,	//	.cpp, .h, .hpp
		IMAGE	=	2,	//	.png, .jpg ...
		MODEL	=	3,	//	.fbx, .obj ...
		SCENE	=	4,	// .pop
		RESRC	=	5,
		TEXT	=	6	//	or else just text
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

		// -------------------- Save Scene -----------------------
		std::string WriteScene(std::string name);
		std::string WriteScene(std::string name, int nextID, int focusedCamID, std::queue<int>& reuseableID, std::vector <std::vector<Accessor>>& keysToAccessComponent, std::vector <GameObject*>& gameObjects);
		std::string WriteAddressor(std::vector <std::vector<Accessor>>& keysToAccessComponent);
		std::string WriteRecycleQ(std::queue<int> reuseableID);
		std::string WriteGameObject(int id, std::string name, glm::vec3 position = glm::vec3(0.0f), glm::vec3 rotation = glm::vec3(0.0f), glm::vec3 scale = glm::vec3(1.0f));

		std::string WriteComponents();
		std::string WriteCameraComponent(std::vector<Camera>& cameras);
		std::string WriteLightComponent(std::vector<Light>& lights);
		std::string WriteMeshRendererComponent(std::vector<MeshRenderer>& meshRenderers);

		std::string	WriteBoxColliderComponent(std::vector<BoxCollider>& boxcols);
		std::string	WriteRigidbodyComponent(std::vector<Rigidbody>& rigbodies);

		std::string	WriteUIFrameComponent(std::vector<UIFrame>& rigbodies);
		std::string	WriteTextComponent(std::vector<Text>& rigbodies);

		// -------------------- Load Scene -----------------------
		std::queue<int>						ReadRecycleQ(std::ifstream& in);
		std::vector<GameObject>				ReadGameObjects(std::ifstream& in);
		std::vector<std::vector<Accessor>>	ReadAddressor(std::ifstream& in);

		std::vector<Camera>					ReadCameraComponent(std::ifstream& in);
		std::vector<Light>					ReadLightComponent(std::ifstream& in);
		std::vector<MeshRenderer>			ReadMeshRendererComponent(std::ifstream& in);
		
		std::vector<BoxCollider>			ReadBoxColliderComponent(std::ifstream& in);
		std::vector<Rigidbody>				ReadRigidbodyComponent(std::ifstream& in);

		std::vector<UIFrame>				ReadUIFrameComponent(std::ifstream& in);
		std::vector<Text>					ReadTextComponent(std::ifstream& in);
		
	public:
		fs::path root;
		fs::path resource;
		fs::path curr_focused_path;

		FileIO();
		~FileIO();
		
		void Init();
		void InitShader(fs::path shaderpath);
		void DefaultSetting();

		int ShowFilesAtDir(std::vector<FileData>& dirs, std::vector<FileData>& files, fs::path currPath = fs::current_path() / "Root");
		int ShowDirAtDir(std::vector<DirectoryData>& dirDats, fs::path currPath = fs::current_path() / "Root");

		void WriteDataToFile(std::string datfile, std::string dattablefile, FileData filedata);
		unsigned char* FileDataBuffer(fs::path path);
		void InitProject(fs::path path);
		
		// TODO :: Change it (use Boost or make reflection)
		void SaveScene();
		// TODO :: Change it too
		void SaveMaterial();
		void LoadMaterial();

		void LoadScene(fs::path path);
	};
}

