#pragma once

namespace Popeye{
	namespace fs = std::filesystem;

	struct MeshRenderer;
	struct Camera;
	struct Light;

	struct BoxCollider;
	struct Rigidbody;

	struct UIFrame;
	struct Text;
	struct Button;

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
		
		void ShowComponent(Camera&, bool&);
		void ShowComponent(MeshRenderer&, bool&);
		void ShowComponent(Light&, bool&);
		void ShowComponent(BoxCollider&, bool&);
		void ShowComponent(Rigidbody&, bool&);
		void ShowComponent(UIFrame&, bool&);
		void ShowComponent(Text&, bool&);
		//void ShowComponent(Button&, bool&);
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
		unsigned int previewFBO, preview;
		virtual void ShowContents();
		void ShowPreview(int index);
	};
}

