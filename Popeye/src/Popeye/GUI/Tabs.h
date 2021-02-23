#pragma once
#include "../Events/EventManager.h"

namespace Popeye{
	struct MeshRenderer;
	struct Camera;

	struct Tab
	{
		const char* name;
		EventMod eventmod;
		void SetTab(const char*, EventMod = EventMod::ONGUI);
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
		void ShowCamera(Camera&, int id);
		void ShowMeshRenderer(MeshRenderer&);
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
	};
}

