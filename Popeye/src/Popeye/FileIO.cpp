#include "FileIO.h"

#include "./Scene/Scene.h"
#include "./Scene/GameObject.h"

#include "./Manager/ComponentManager.h"
#include "./Component/RenderingComponents.h"
namespace Popeye
{
	FileData::FileData() {}
	FileData::FileData(FileType _type, fs::path _path) : type(_type), path(_path) {}

	DirectoryData::DirectoryData(fs::path _path, bool _hasSubDir) : path(_path), hasSubDir(_hasSubDir) {}

	FileIO::FileIO() {}
	FileIO::~FileIO() {}

	void FileIO::Init()
	{
		//set path
		root = fs::current_path() / "Root"; resource = fs::current_path() / "Resource"; curr_focused_path = root;

		//make resource data folder if there isn't one
		if (!fs::exists(resource / "Resource.dat"))			{ std::ofstream out(resource / "Resource.dat");			out.close();}
		if (!fs::exists(resource / "ResourceTable.dat"))	{ std::ofstream out(resource / "ResourceTable.dat");	out.close();}

		if (!fs::exists(fs::current_path() / "popeye.info")) { 
			DefaultSetting(); 
		}

		InitProject(fs::current_path() / "popeye.info");
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
				else if (!extension.compare(".pop"))
				{
					type = FileType::SCENE;
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

	unsigned char* FileIO::FileDataBuffer(fs::path path)
	{
		unsigned char *buffer = nullptr;
		std::ifstream writedata;
		writedata.open(path, std::ios::in | std::ios::binary);
		if (writedata.is_open())
		{
			writedata.seekg(0, writedata.end);
			unsigned int length = (unsigned int)writedata.tellg();
			writedata.seekg(0, writedata.beg);

			buffer = (unsigned char*)malloc(length);

			writedata.read((char*)buffer, length);

			writedata.close();
		}

		return buffer;
	}

	void FileIO::DefaultSetting()
	{
		std::ofstream out;
		fs::create_directories(root / "scene");
		fs::create_directories(root / "resource");
		out.open(root / "scene" / "test.pop");
		out << WriteScene("test");
		out.close();

		out.open(fs::current_path() / "popeye.info");
		out << (root / "scene" / "test.pop").string() << '\n'		// current focused window
			<< 2.0f << ' ' << 2.0f << ' ' << 2.0f << '\n'
			<< -30.0f << ' ' << 45.30f << ' ' << 0.0f << '\n'
			<< 0;										// mod
		out.close();
	}

	void FileIO::InitProject(fs::path path)
	{
		std::ifstream writedata;
		writedata.open(path, std::ios::in | std::ios::binary);
		std::string str;
		glm::vec3 dd;
		if (writedata.is_open())
		{
			while (true)
			{
				writedata >> str >> dd.x >> dd.y >> dd.z;
				std::cout << str << '\n';

				if (writedata.eof())
					break;
			}
			writedata.close();
		}

	}
	
	void FileIO::ReadSceneFile(fs::path path)
	{
		std::ifstream writedata;
		writedata.open(path, std::ios::in | std::ios::binary);
		if (writedata.is_open())
		{
			while (true)
			{

				if (writedata.eof())
					break;
			}
			writedata.close();
		}

	}

	void FileIO::SaveScene(Scene* scene)
	{
		std::ofstream out;
		std::string scenedata = WriteScene(scene->GetName(), scene->GetNextID(), scene->focusedCamID, scene->GetReusableQueue(), scene->GetAccessors(), scene->gameObjects);
		
		out.open(root / "scene" / "test.pop");
		out << scenedata;
		out.close();
	}

	std::string FileIO::WriteScene(std::string name, int nextID, int focusedCamID, std::queue<int>& reuseableID, std::vector <std::vector<Accessor>>& keysToAccessComponent, std::vector <GameObject*>& gameObjects)
	{
		std::string scene;
		char enter = '\n', space = ' ';
		scene =
			name + enter
			+ WriteComponents()
			+ std::to_string(nextID) + enter
			+ std::to_string(focusedCamID) + enter
			+ WriteReusableIDs(reuseableID)
			+ WriteAddressor(keysToAccessComponent);

		for (int i = 0; i < gameObjects.size(); i++)
		{
			scene += WriteGameObject(gameObjects[i]->GetID(), gameObjects[i]->GetName(), gameObjects[i]->transform.position, gameObjects[i]->transform.rotation, gameObjects[i]->transform.scale);
		}

		return scene;
	}

	std::string FileIO::WriteScene(std::string name)
	{
		std::string scene;
		char enter = '\n', space = ' ';
		scene =
			name + enter
			+ std::to_string(2) + enter	// next ID
			+ std::to_string(0) + enter // focused cam ID
			+ "[ ]\n"					// reusable queue
			+ "[ ]\n"					// addressor					
			+ WriteGameObject(0, "Main Camera", glm::vec3(8.0f), glm::vec3(-30.0f, 45.0f, 0.0f))
			+ WriteGameObject(1, "Light");

		return scene;
	}

	std::string FileIO::WriteAddressor(std::vector <std::vector<Accessor>>& keysToAccessComponent)
	{
		std::string addressor = "[ ";
		int size = keysToAccessComponent.size();
		for (int i = 0; i < size; i++)
		{
			int acc_siz = keysToAccessComponent[i].size();
			addressor += "[ ";
			for (int j = 0; j < acc_siz; j++)
			{
				std::string type = keysToAccessComponent[i][j].componentType;
				type += " ";

				std::string index = std::to_string(keysToAccessComponent[i][j].dataIndex);
				index += " ";

				addressor += type + index;
			}
			
			if (i < size - 1)
				addressor += " ]\n";
			else
				addressor += " ]";
		}


		addressor += " ]\n";

		return addressor;
	}

	std::string FileIO::WriteReusableIDs(std::queue<int> reuseableID)
	{
		char space = ' ';
		std::string reusable = "[ ";
		while (!reuseableID.empty())
		{
			reusable += std::to_string(reuseableID.front()) + " ";
		}
		reusable += "]\n";

		return reusable;
	}

	std::string FileIO::WriteGameObject(int id, std::string name, glm::vec3 position, glm::vec3 rotation, glm::vec3 scale)
	{
		std::string gameObject;
		char enter = '\n', space = ' ';
		gameObject =
			std::to_string(id) + enter
			+ name + enter
			+ std::to_string(position.x) + space + std::to_string(position.y) + space + std::to_string(position.y) + enter
			+ std::to_string(rotation.x) + space + std::to_string(rotation.y) + space + std::to_string(rotation.y) + enter
			+ std::to_string(scale.x) + space + std::to_string(scale.y) + space + std::to_string(scale.y) + enter;

		return gameObject;
	}



	std::string FileIO::WriteComponents()
	{

		std::pair<const char*, std::vector<Camera>> cameras = ComponentManager::GetInstance()->GetAllDataOfComponent<Camera>();
		std::pair<const char*, std::vector<MeshRenderer>> meshrenderers = ComponentManager::GetInstance()->GetAllDataOfComponent<MeshRenderer>();
		std::pair<const char*, std::vector<Light>> lights = ComponentManager::GetInstance()->GetAllDataOfComponent<Light>();

		std::string cam = cameras.first;
		cam += " [ " + WriteCameraComponent(cameras.second) + " ]\n";

		std::string mesh = meshrenderers.first;
		mesh += " [ " + WriteMeshRendererComponent(meshrenderers.second) + " ]\n";

		std::string light = lights.first;
		light += " [ " + WriteLightComponent(lights.second) + " ]";

		std::string components = "[ ";
		components += cam + mesh + light + " ]\n";

		return components;
	}

	std::string FileIO::WriteCameraComponent(std::vector<Camera>& cameras) 
	{
		std::string camera;
		char enter = '\n', space = ' ';

		int size = cameras.size();
		for (int i = 0; i < size; i++)
		{
			if (cameras[i].mod == Projection::PERSPECTIVE)
				camera += std::to_string(0) + space;
			else
				camera += std::to_string(1) + space;
			camera += 
				std::to_string(cameras[i].fov) + space
				+ std::to_string(cameras[i].offsetX) + space
				+ std::to_string(cameras[i].offsetY) + space
				+ std::to_string(cameras[i].nearView) + space
				+ std::to_string(cameras[i].farView) + space
				+ std::to_string(cameras[i].width) + space
				+ std::to_string(cameras[i].height) + space;
		}

		return camera;
	}
	
	std::string FileIO::WriteLightComponent(std::vector<Light>& lights)
	{
		std::string light;
		char enter = '\n', space = ' ';

		int size = lights.size();
		for (int i = 0; i < size; i++)
		{
			if (i == 0)
			{
				light += std::to_string(lights[i].pointLightCounter) + space
					+ std::to_string(lights[i].directionalLightCounter) + space
					+ std::to_string(lights[i].spotLightCounter) + space;
			}

			light +=
				std::to_string(lights[i].color.x) + space + std::to_string(lights[i].color.y) + space + std::to_string(lights[i].color.z) + space
				+ std::to_string(lights[i].ambient) + space
				+ std::to_string(lights[i].diffuse) + space
				+ std::to_string(lights[i].specular) + space
				+ std::to_string(lights[i].constant) + space
				+ std::to_string(lights[i].linear) + space
				+ std::to_string(lights[i].quadratic) + space
				+ std::to_string(lights[i].cutoff) + space
				+ std::to_string(lights[i].outercutoff) + space;
		}

		return light;
	}
	
	std::string FileIO::WriteMeshRendererComponent(std::vector<MeshRenderer>& meshRenderers)
	{
		std::string meshrenderer;
		char enter = '\n', space = ' ';

		int size = meshRenderers.size();
		for (int i = 0; i < size; i++)
		{
			meshrenderer +=
				std::to_string(meshRenderers[i].meshID) + space
				+ std::to_string(meshRenderers[i].materialID) + space
				+ std::to_string(meshRenderers[i].isEmpty) + space;
		}

		return meshrenderer;
	}

}
