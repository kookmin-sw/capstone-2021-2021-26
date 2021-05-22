#include "FileIO.h"

#include "./Scene/Scene.h"
#include "./Scene/GameObject.h"

#include "./Manager/ResourceManager.h"
#include "./Manager/SceneManager.h"
#include "./Manager/ComponentManager.h"
#include "./Component/RenderingComponents.h"
#include "./Component/PhysicsComponents.h"
#include "./Component/UIComponents.h"

namespace Popeye
{
	extern ResourceManager* g_ResourceManager;
	extern const char* fragmentShader;
	extern const char* vertexShader;
	extern const char* gizmovertShder;
	extern const char* gizmofragShder;
	extern const char* screenvertShder;
	extern const char* screenfragShder;

	FileData::FileData() {}
	FileData::FileData(FileType _type, fs::path _path) : type(_type), path(_path) {}

	DirectoryData::DirectoryData(fs::path _path, bool _hasSubDir) : path(_path), hasSubDir(_hasSubDir) {}

	FileIO::FileIO() {}
	FileIO::~FileIO() {}

	void FileIO::Init()
	{
		//set path
		root = fs::current_path() / "Root"; resource = fs::current_path() / "Resource"; curr_focused_path = root;

		fs::path shaderPath = fs::current_path() / "shader";
		if (!fs::exists(shaderPath)) {
			fs::create_directories(shaderPath);  InitShader(shaderPath);
		}

		if (!fs::exists(resource)) { fs::create_directories(resource); }
		if (!fs::exists(resource / "Resource.dat"))			{ std::ofstream out(resource / "Resource.dat");			out.close();}
		if (!fs::exists(resource / "ResourceTable.dat"))	{ std::ofstream out(resource / "ResourceTable.dat");	out.close();}

		if (!fs::exists(resource / "Material.dat")) { std::ofstream out(resource / "Material.dat");	out.close(); }

		if (!fs::exists(fs::current_path() / "popeye.info")) { 
			DefaultSetting(); 
		}
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
				else if (!extension.compare(".dat"))
				{
					type = FileType::RESRC;
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
		out << "Camera 1 0 45.000000 800.000000 600.000000 0.100000 100.000000 5.000000 5.000000 \n0\n"
			<< "MeshRenderer 0  \n0\n"
			<< "Light 1 0 1 0 1 1.000000 1.000000 1.000000 0.800000 0.700000 0.100000 1.000000 0.090000 0.032000 12.500000 17.500000 \n0\n"
			<< "BoxCollider 0 \n0\n"
			<< "Rigidbody 0 \n0\n"
			<< "UIFrame 0 \n0\n"
			<< "Text 0 \n0\n"
			<< '\n';
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
		int intReader;
		std::string strReader;
		glm::vec3 vecReader;
		if (writedata.is_open())
		{
			writedata >> strReader; // current focused Scene
			LoadScene(strReader);


			writedata >> vecReader.x >> vecReader.y >> vecReader.z; // editor cam pos

			writedata >> vecReader.x >> vecReader.y >> vecReader.z; // editor cam rot

			writedata >> intReader;									// editor cam mod
			
			writedata.close();
		}

		LoadMaterial();

	}

	void FileIO::SaveMaterial()
	{
		std::ofstream out;
		Material material;
		int size = g_ResourceManager->materials.size();
		out.open(resource / "Material.dat");
		out << size - 1 << ' ';
		for (int i = 1; i < size; i++)
		{
			material = g_ResourceManager->materials[i];
			out << material.id << ' '
				<< material.textureID << ' '
				<< material.shininess << ' '
				<< material.color.x << ' ' << material.color.y << ' ' << material.color.z << ' '
				<< material.amb_diff_spec.x << ' ' << material.amb_diff_spec.y << ' ' << material.amb_diff_spec.z << ' ';
		}


		out.close();
	}

	void  FileIO::LoadMaterial()
	{
		std::ifstream writedata;
		writedata.open(resource / "Material.dat", std::ios::in | std::ios::binary);
		int size;
		std::vector<Material> materials;
		materials.push_back(g_ResourceManager->materials[0]);
		if (writedata.is_open())
		{
			writedata >> size;
			for (int i = 0; i < size; i++)
			{
				Material material;

				writedata >> material.id 
					>> material.textureID
					>> material.shininess 
					>> material.color.x >> material.color.y  >> material.color.z 
					>> material.amb_diff_spec.x >> material.amb_diff_spec.y  >> material.amb_diff_spec.z;


				materials.push_back(material);
			}

			g_ResourceManager->materials.swap(materials);

			writedata.close();
		}

	}

	void FileIO::SaveScene()
	{
		std::ofstream out;
		Scene* scene = SceneManager::GetInstance()->currentScene;
		std::string scenedata = WriteScene(scene->GetName(), scene->GetNextID(), scene->focusedCamID, scene->GetRecycleQueue(), scene->GetAccessors(), scene->gameObjects);
		
		//out.open(root / "scene" / "test.pop");
		out.open(SceneManager::GetInstance()->currentScenePath);
		out << scenedata;
		out.close();

		SaveMaterial();
	}

	std::string FileIO::WriteScene(std::string name, int nextID, int focusedCamID, std::queue<int>& reuseableID, std::vector <std::vector<Accessor>>& keysToAccessComponent, std::vector <GameObject*>& gameObjects)
	{
		std::string scene;
		char enter = '\n', space = ' ';
		int gameObject_size = gameObjects.size();
		scene =
			WriteComponents()
			+ name + enter
			+ std::to_string(nextID) + enter
			+ std::to_string(focusedCamID) + enter
			+ WriteRecycleQ(reuseableID)
			+ WriteAddressor(keysToAccessComponent)
			+ std::to_string(gameObject_size) + enter;

		for (int i = 0; i < gameObject_size; i++)
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
			+ std::to_string(0) + enter					
			+ std::to_string(2) + enter
			+ "1 Camera 0 " + enter
			+ "1 Light 0 " + enter
			+ std::to_string(2) + enter
			+ WriteGameObject(0, "Main_Camera", glm::vec3(8.0f), glm::vec3(-30.0f, 45.0f, 0.0f))
			+ WriteGameObject(1, "Light");

		return scene;
	}

	std::string FileIO::WriteAddressor(std::vector <std::vector<Accessor>>& keysToAccessComponent)
	{
		int size = keysToAccessComponent.size();
		std::string addressor = std::to_string(size) + "\n";
		for (int i = 0; i < size; i++)
		{
			int acc_siz = keysToAccessComponent[i].size();
			addressor += std::to_string(acc_siz) + " ";
			for (int j = 0; j < acc_siz; j++)
			{
				if(keysToAccessComponent[i][j].dataIndex != -1)
				{
					std::string type = keysToAccessComponent[i][j].componentType;
					type += " ";

					std::string index = std::to_string(keysToAccessComponent[i][j].dataIndex);
					index += " ";

					addressor += type + index;
				}
			}
			
			if (i < size - 1)
				addressor += "\n";
			else
				addressor += " ";
		}


		addressor += " \n";

		return addressor;
	}

	std::string FileIO::WriteRecycleQ(std::queue<int> reuseableID)
	{
		std::string reusable = std::to_string(reuseableID.size()) + " ";
		while (!reuseableID.empty())
		{
			reusable += std::to_string(reuseableID.front()) + " ";
			reuseableID.pop();
		}
		reusable += "\n";

		return reusable;
	}

	std::string FileIO::WriteGameObject(int id, std::string name, glm::vec3 position, glm::vec3 rotation, glm::vec3 scale)
	{
		std::string gameObject;
		char enter = '\n', space = ' ';
		gameObject =
			std::to_string(id) + enter
			+ name + enter
			+ std::to_string(position.x) + space + std::to_string(position.y) + space + std::to_string(position.z) + enter
			+ std::to_string(rotation.x) + space + std::to_string(rotation.y) + space + std::to_string(rotation.z) + enter
			+ std::to_string(scale.x)	 + space + std::to_string(scale.y)	  + space + std::to_string(scale.z)	   + enter;

		return gameObject;
	}

	std::string FileIO::WriteComponents()
	{
		std::pair<std::vector<Camera>, std::queue<int>> cameras = ComponentManager::GetInstance()->GetAllDataOfComponent<Camera>();
		std::pair<std::vector<MeshRenderer>, std::queue<int>> meshrenderers = ComponentManager::GetInstance()->GetAllDataOfComponent<MeshRenderer>();
		std::pair<std::vector<Light>, std::queue<int>> lights = ComponentManager::GetInstance()->GetAllDataOfComponent<Light>();
		std::pair<std::vector<BoxCollider>, std::queue<int>> boxcolliders = ComponentManager::GetInstance()->GetAllDataOfComponent<BoxCollider>();
		std::pair<std::vector<Rigidbody>, std::queue<int>> rigidbodies = ComponentManager::GetInstance()->GetAllDataOfComponent<Rigidbody>();
		std::pair<std::vector<UIFrame>, std::queue<int>> uiFrames = ComponentManager::GetInstance()->GetAllDataOfComponent<UIFrame>();
		std::pair<std::vector<Text>, std::queue<int>> texts = ComponentManager::GetInstance()->GetAllDataOfComponent<Text>();

		std::string cam = typeid(Camera).name() + 15;
		cam += WriteCameraComponent(cameras.first) + "\n";
		cam += WriteRecycleQ(cameras.second);

		std::string mesh = typeid(MeshRenderer).name() +15;
		mesh += WriteMeshRendererComponent(meshrenderers.first) + "\n";
		mesh += WriteRecycleQ(meshrenderers.second);

		std::string light = typeid(Light).name() + 15;
		light += WriteLightComponent(lights.first) + "\n";
		light +=  WriteRecycleQ(lights.second) ;

		std::string col = typeid(BoxCollider).name() + 15;
		col += WriteBoxColliderComponent(boxcolliders.first) + "\n";
		col += WriteRecycleQ(boxcolliders.second);
		
		std::string rig = typeid(Rigidbody).name() + 15;
		rig += WriteRigidbodyComponent(rigidbodies.first) + "\n";
		rig += WriteRecycleQ(rigidbodies.second);
		
		std::string uiframe = typeid(UIFrame).name() + 15;
		uiframe += WriteUIFrameComponent(uiFrames.first) + "\n";
		uiframe += WriteRecycleQ(uiFrames.second);

		std::string text = typeid(Text).name() + 15;
		text += WriteTextComponent(texts.first) + "\n";
		text += WriteRecycleQ(texts.second);

		std::string components = "\n";
		components += cam + mesh + light + col + rig + uiframe + text + " \n";

		return components;
	}

	std::string FileIO::WriteCameraComponent(std::vector<Camera>& cameras) 
	{
		int size = cameras.size();
		std::string camera = " " + std::to_string(size) + " ";
		char space = ' ';

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
		int size = lights.size();
		std::string light = " " + std::to_string(size) + " ";
		char space = ' ';
		
		int type = 0;
		for (int i = 0; i < size; i++)
		{
			if (i == 0)
			{
				light += std::to_string(lights[i].pointLightCounter) + space
					+ std::to_string(lights[i].directionalLightCounter) + space
					+ std::to_string(lights[i].spotLightCounter) + space;
			}

			switch (lights[i].ShowLightType())
			{
			case LightType::POINT:
				type = 0;
				break;
			case LightType::DIRECTION:
				type = 1;
				break;
			case LightType::SPOT:
				type = 2;
				break;
			}

			light +=
				std::to_string(type) + space
				+ std::to_string(lights[i].color.x) + space + std::to_string(lights[i].color.y) + space + std::to_string(lights[i].color.z) + space
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
		int size = meshRenderers.size();
		std::string meshrenderer = " " + std::to_string(size) + " ";
		char space = ' ';

		for (int i = 0; i < size; i++)
		{
			meshrenderer +=
				std::to_string(meshRenderers[i].meshID) + space
				+ std::to_string(meshRenderers[i].materialID) + space
				+ std::to_string(meshRenderers[i].isEmpty) + space;
		}

		return meshrenderer;
	}

	std::string	 FileIO::WriteBoxColliderComponent(std::vector<BoxCollider>& boxcols)
	{
		int size = boxcols.size();
		std::string boxcollider = " " + std::to_string(size) + " ";
		char space = ' ';

		for (int i = 0; i < size; i++)
		{
			boxcollider +=
				std::to_string(boxcols[i].width) + space
				+ std::to_string(boxcols[i].length) + space
				+ std::to_string(boxcols[i].height) + space;
		
		}

		return boxcollider;
	}

	std::string	 FileIO::WriteRigidbodyComponent(std::vector<Rigidbody>& rigbodies)
	{
		int size = rigbodies.size();
		std::string rigidbody = " " + std::to_string(size) + " ";
		char space = ' ';

		for (int i = 0; i < size; i++)
		{
			rigidbody +=
				std::to_string(rigbodies[i].gravity) + space
				+ std::to_string(rigbodies[i].weight) + space;
		}

		return rigidbody;
	}

	std::string	 FileIO::WriteUIFrameComponent(std::vector<UIFrame>& uiframes)
	{
		int size = uiframes.size();
		std::string uiframe = " " + std::to_string(size) + " ";
		char space = ' ';

		for (int i = 0; i < size; i++)
		{
			uiframe +=
				std::to_string(uiframes[i].textureID) + space
				+ std::to_string(uiframes[i].leftTop.x) + space + std::to_string(uiframes[i].leftTop.y)
				+ std::to_string(uiframes[i].rightBot.x) + space + std::to_string(uiframes[i].rightBot.y);

		}

		return uiframe;
	}

	std::string	 FileIO::WriteTextComponent(std::vector<Text>& texts)
	{
		int size = texts.size();
		std::string text = " " + std::to_string(size) + " ";
		char space = ' ';
		
		for (int i = 0; i < size; i++)
		{
			text +=
				texts[i].contents + space
				+ std::to_string(texts[i].leftTop.x) + space + std::to_string(texts[i].leftTop.y)
				+ std::to_string(texts[i].rightBot.x) + space + std::to_string(texts[i].rightBot.y);
		}

		return text;
	}

	
	
	void FileIO::LoadScene(fs::path path)
	{
		SceneManager::GetInstance()->currentScenePath = path.string();
		Scene* scene = SceneManager::GetInstance()->currentScene;
		// Readers for each datatype
		int				intReader;
		std::string		strReader;

		std::ifstream writedata;

		writedata.open(path, std::ios::in | std::ios::binary);
		if (writedata.is_open())
		{
			// Component data for the scene
			{
				{	// Component Camera
					std::vector<Camera>			camDatas = ReadCameraComponent(writedata);
					std::queue<int>				recycleQ = ReadRecycleQ(writedata);

					ComponentManager::GetInstance()->GetAllDataOfComponent<Camera>(camDatas, recycleQ);
				}

				{	// Component MeshRenderer
					std::vector<MeshRenderer>	mshDatas = ReadMeshRendererComponent(writedata);
					std::queue<int>				recycleQ = ReadRecycleQ(writedata);

					ComponentManager::GetInstance()->GetAllDataOfComponent<MeshRenderer>(mshDatas, recycleQ);
				}

				{	// Component Light
					std::vector<Light> lihtDatas = ReadLightComponent(writedata);
					std::queue<int> recycleQ = ReadRecycleQ(writedata);

					ComponentManager::GetInstance()->GetAllDataOfComponent<Light>(lihtDatas, recycleQ);
				}

				{	// Component Light
					std::vector<BoxCollider> lihtDatas = ReadBoxColliderComponent(writedata);
					std::queue<int> recycleQ = ReadRecycleQ(writedata);

					ComponentManager::GetInstance()->GetAllDataOfComponent<BoxCollider>(lihtDatas, recycleQ);
				}

				{	// Component Light
					std::vector<Rigidbody> lihtDatas = ReadRigidbodyComponent(writedata);
					std::queue<int> recycleQ = ReadRecycleQ(writedata);

					ComponentManager::GetInstance()->GetAllDataOfComponent<Rigidbody>(lihtDatas, recycleQ);
				}

				{	// Component Light
					std::vector<UIFrame> lihtDatas = ReadUIFrameComponent(writedata);
					std::queue<int> recycleQ = ReadRecycleQ(writedata);

					ComponentManager::GetInstance()->GetAllDataOfComponent<UIFrame>(lihtDatas, recycleQ);
				}

				{	// Component Light
					std::vector<Text> lihtDatas = ReadTextComponent(writedata);
					std::queue<int> recycleQ = ReadRecycleQ(writedata);

					ComponentManager::GetInstance()->GetAllDataOfComponent<Text>(lihtDatas, recycleQ);
				}
			}

			// Scene
			{
				writedata >> strReader;			// name
				scene->SetName(strReader);

				writedata >> intReader;			// next gameobject
				scene->SetNextID(intReader);

				writedata >> intReader;			// current camera id
				scene->focusedCamID = intReader;

				std::queue<int> recycleQ = ReadRecycleQ(writedata);
				scene->SetRecycleQueue(recycleQ);

				std::vector<std::vector<Accessor>> adressor = ReadAddressor(writedata);
				scene->SetAccessors(adressor);

				std::vector<GameObject> gmaeObjects = ReadGameObjects(writedata);
				scene->SetGameObjects(gmaeObjects);
			}

			writedata.close();
		}

	}

	std::queue<int>	FileIO::ReadRecycleQ(std::ifstream& writedata)
	{
		int intReader;
		std::queue<int> recycleQ;
		writedata >> intReader;

		int qsize = intReader;
		for (int i = 0; i < qsize; i++)
		{
			writedata >> intReader;
			recycleQ.push(intReader);
		}

		return recycleQ;
	}
	
	std::vector<GameObject>	FileIO::ReadGameObjects(std::ifstream& writedata)
	{
		std::vector<GameObject> gameobjects;
		int intReader;
		std::string strReader;
		Transform tReader;
		writedata >> intReader;
		int size = intReader;
		for (int i = 0; i < size; i++)
		{
			writedata >> intReader >> strReader
				>> tReader.position.x >> tReader.position.y >> tReader.position.z
				>> tReader.rotation.x >> tReader.rotation.y >> tReader.rotation.z
				>> tReader.scale.x >> tReader.scale.z >> tReader.scale.z;
			GameObject gameobject(intReader, strReader, tReader);
			gameobjects.push_back(gameobject);
		}

		return gameobjects;
	}
	
	std::vector<std::vector<Accessor>>	FileIO::ReadAddressor(std::ifstream& writedata)
	{
		int intReader;
		std::string strReader;
		std::vector<std::vector<Accessor>> keysToaccess;

		writedata >> intReader;
		int size = intReader;
		for (int i = 0; i < size; i++)
		{
			std::vector<Accessor> accessor_arr;
			
			writedata >> intReader;
			int ssize = intReader;
			for (int i = 0; i < ssize; i++)
			{
				Accessor accesor;
				writedata >> strReader >> intReader;

				std::string str = strReader;
				accesor.componentType = str.c_str();
				accesor.dataIndex = intReader;

				accessor_arr.push_back(accesor);
			}
			keysToaccess.push_back(accessor_arr);
		}

		return keysToaccess;
	}

	std::vector<Camera> FileIO::ReadCameraComponent(std::ifstream& writedata)
	{
		std::vector<Camera> camDatas;
		int intReader;
		std::string strReader;
		Camera cReader;

		writedata >> strReader >> intReader;
		int size = intReader;
		for (int i = 0; i < size; i++)
		{
			writedata >> intReader;
			if (intReader == 0)
				cReader.mod = Projection::PERSPECTIVE;
			else
				cReader.mod = Projection::ORTHOGRAPHIC;
			writedata >> cReader.fov
				>> cReader.offsetX >> cReader.offsetY
				>> cReader.nearView >> cReader.farView
				>> cReader.width >> cReader.height;

			camDatas.push_back(cReader);
		}
		
		return camDatas;
	}
	
	std::vector<Light> FileIO::ReadLightComponent(std::ifstream& writedata)
	{
		std::vector<Light> lihtDatas;
		int intReader;
		std::string strReader;
		Light lReader;

		writedata >> strReader >> intReader;
		int size = intReader;
		
		writedata >> intReader;
		Light::pointLightCounter = intReader;
		writedata >> intReader;
		Light::directionalLightCounter = intReader;
		writedata >> intReader;
		Light::spotLightCounter = intReader;
		
		for (int i = 0; i < size; i++)
		{

			writedata >> intReader;
			switch (intReader)
			{
			case 0:
				lReader.type = LightType::POINT;
				break;
			case 1:
				lReader.type = LightType::DIRECTION;
				break;
			case 2:
				lReader.type = LightType::SPOT;
				break;
			}

			writedata >> lReader.color.x >> lReader.color.y >> lReader.color.z
				>> lReader.ambient >> lReader.diffuse >> lReader.specular
				>> lReader.constant >> lReader.linear >> lReader.quadratic
				>> lReader.cutoff >> lReader.outercutoff;

			lihtDatas.push_back(lReader);
		}


		return lihtDatas;
	}
	
	std::vector<MeshRenderer> FileIO::ReadMeshRendererComponent(std::ifstream& writedata)
	{
		std::vector<MeshRenderer> mshDatas;
		int intReader;
		std::string strReader;
		MeshRenderer mReader;

		writedata >> strReader >> intReader;
		int size = intReader;
		for (int i = 0; i < size; i++)
		{
			writedata >> mReader.meshID >> mReader.materialID >> mReader.isEmpty;
			mshDatas.push_back(mReader);
		}

		return mshDatas;
	}

	std::vector<BoxCollider> FileIO::ReadBoxColliderComponent(std::ifstream& writedata)
	{
		std::vector<BoxCollider> boxcolls;
		int intReader;
		std::string strReader;
		BoxCollider coll;

		writedata >> strReader >> intReader;
		int size = intReader;
		for (int i = 0; i < size; i++)
		{
			writedata >> coll.width >> coll.length >> coll.height;

			boxcolls.push_back(coll);
		}
		return boxcolls;
	}
	
	std::vector<Rigidbody>	FileIO::ReadRigidbodyComponent(std::ifstream& writedata)
	{
		std::vector<Rigidbody> rigbodies;
		int intReader;
		std::string strReader;
		Rigidbody rigid;

		writedata >> strReader >> intReader;
		int size = intReader;
		for (int i = 0; i < size; i++)
		{
			writedata >> rigid.gravity >> rigid.weight;
			
			rigbodies.push_back(rigid);
		}
		return rigbodies;
	}

	std::vector<UIFrame> FileIO::ReadUIFrameComponent(std::ifstream& writedata)
	{
		std::vector<UIFrame> uiFrames;
		int intReader;
		std::string strReader;
		UIFrame frame;

		writedata >> strReader >> intReader;
		int size = intReader;
		for (int i = 0; i < size; i++)
		{
			writedata >> frame.textureID >>
				frame.leftTop.x >> frame.leftTop.y
				>> frame.rightBot.x >> frame.rightBot.y;

			uiFrames.push_back(frame);
		}
		return uiFrames;
	}
	
	std::vector<Text> FileIO::ReadTextComponent(std::ifstream& writedata)
	{
		std::vector<Text> texts;
		int intReader;
		std::string strReader;
		Text text;

		writedata >> strReader >> intReader;
		int size = intReader;
		for (int i = 0; i < size; i++)
		{
			writedata >> text.leftTop.x >> text.leftTop.y
				>> text.rightBot.x >> text.rightBot.y;

			texts.push_back(text);
		}

		return texts;
	}


	void FileIO::InitShader(fs::path shaderpath)
	{
		std::ofstream out;
		out.open(shaderpath / "vertexShader.GLSL");
		out << vertexShader;
		out.close();

		out.open(shaderpath / "fragmentShader.GLSL");
		out << fragmentShader;
		out.close();

		out.open(shaderpath / "gridfrag.GLSL");
		out << gizmofragShder;
		out.close();

		out.open(shaderpath / "gridvert.GLSL");
		out << gizmovertShder;
		out.close();

		out.open(shaderpath / "vertexShaderfb.GLSL");
		out << screenvertShder;
		out.close();

		out.open(shaderpath / "fragmentShaderfb.GLSL");
		out << screenfragShder;
		out.close();
		
	//out.close();
	}
}
