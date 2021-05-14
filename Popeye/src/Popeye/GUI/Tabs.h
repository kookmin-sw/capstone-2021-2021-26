#pragma once

namespace Popeye{
	namespace fs = std::filesystem;

	struct MeshRenderer;
	struct Camera;
	struct Light;

	struct Tab
	{
		const char* name;
		int eventmod;
		void SetTab(const char*, int = 1);
		void ShowTab();
		void CheckHover();
		virtual void ShowContents();
	};

	struct Hierarchy : public Tab
	{
		virtual void ShowContents();
	};

	struct Inspector : public Tab
	{
		virtual void ShowContents();
		
		void ShowComponent(Camera&);
		void ShowComponent(MeshRenderer&);
		void ShowComponent(Light&);
	};

	struct SceneView : public Tab
	{
		virtual void ShowContents();
	};

	struct GameView : public Tab
	{
		virtual void ShowContents();
	};

	struct Debug : public Tab
	{
		virtual void ShowContents();
	};

	struct Project : public Tab
	{
		virtual void ShowContents();
		void ShowDirectories(int id, std::string directory);
	};

	struct Resource : public Tab
	{
		virtual void ShowContents();
	};
}

