#include "System/main.h"
#include "System/manager.h"
#include "System/input.h"

#include "Scene/title.h"
#include "Scene/Stage01.h"
#include "Scene/Stage02.h"

#include "Component/shader.h"
#include "Component/sprite.h"

#include "GameObject/transition.h"

#include "imgui.h"

void Title::Init()
{
	GameObject* titleLogo = AddGameObject<GameObject>(3);			// 3はレイヤ番号
	titleLogo->AddComponent<Shader>()->Load("shader\\unlitTextureVS.cso", "shader\\unlitTexturePS.cso");
	titleLogo->AddComponent<Sprite>()->Init(0.0f, 0.0f, SCREEN_WIDTH, SCREEN_HEIGHT, "asset\\texture\\Title.png");

	m_Transition = AddGameObject<Transition>(3);					// 3はレイヤ番号
	m_Transition->FadeIn();
}


void Title::Update()
{
	//  画面遷移
	if (m_Transition->GetState() == Transition::State::Stop) {
		if (Input::GetKeyTrigger(VK_RETURN))
		{
			m_Transition->FadeOut();
		}
	}

	// 画面遷移が終了してるか？
	if (m_Transition->GetState() == Transition::State::Finish)
	{
		Manager::SetScene<Stage01>();
	}
}

void Title::Draw()
{
	ImGui::ShowDemoWindow();

	ImGui::Begin("Hello, world!");                          // Create a window called "Hello, world!" and append into it.

	ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
	ImGui::End();
}
