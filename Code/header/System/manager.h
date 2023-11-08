#pragma once
#include <cinttypes>
#include <Windows.h>
#include "../Scene/Base/scene.h"

// ëOï˚éQè∆
class Application;

class Manager
{
private:
	static class Scene* m_Scene;
	static bool m_IsLoop;

public:
	static void Init(Application* ap, HWND hWnd);
	static void Uninit();
	static void Update(uint64_t d);
	static void Draw(uint64_t d);

	//static void SetLoopFlag(bool _flg) { m_IsLoop = _flg; }
	static bool GetLoopFlag(void) { return m_IsLoop; }

	static void EndApp() { m_IsLoop = false; };

	static class Scene* GetScene()
	{
		return m_Scene;
	}

	template <typename T>
	static void SetScene()
	{
		if (m_Scene)
		{
			m_Scene->UninitBase();
			delete m_Scene;
		}

		m_Scene = new T();
		m_Scene->InitBase();
	}
};