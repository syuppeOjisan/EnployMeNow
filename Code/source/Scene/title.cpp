#include "System/main.h"
#include "System/manager.h"
#include "System/input.h"
#include "System/KeyboardInput.h"
#include "System/PadInput.h"

#include "Scene/title.h"
#include "Scene/game.h"
#include "Scene/Stage01.h"
#include "Scene/Stage02.h"

#include "Component/shader.h"
#include "Component/sprite.h"

#include "GameObject/transition.h"

#include <memory>
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
	std::unique_ptr<InputIntarface> pInput = std::make_unique<PadInput>();

	if (pInput)
	{
		pInput->Update();

		//  画面遷移
		if (m_Transition->GetState() == Transition::State::Stop) {
			if (pInput->GetPressed(VK_PAD_A))
			{
				m_Transition->FadeOut();
			}
		}

		// 画面遷移が終了してるか？
		if (m_Transition->GetState() == Transition::State::Finish)
		{
			Manager::SetScene<Game>();
		}
	}
}

void Title::Draw()
{
	// GUI描画
	{
		// メニューバーをいい感じに表示
		ImGui::BeginMainMenuBar();
		ImGui::Text("%.1f |", ImGui::GetIO().Framerate);
		if (ImGui::BeginMenu("Menu"))
		{
			if (ImGui::BeginMenu("ChangeScene"))
			{
				if (ImGui::MenuItem("game"))
				{
					Manager::SetScene<Game>();
				}
				else if (ImGui::MenuItem("Stage01"))
				{
					Manager::SetScene<Stage01>();
				}
				else if (ImGui::MenuItem("Stage02"))
				{
					Manager::SetScene<Stage02>();
				}

				ImGui::EndMenu();
			}

			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu("Quit"))
		{
			Manager::EndApp();

			ImGui::EndMenu();
		}


		ImGui::EndMainMenuBar();
	}

	ImGui::ShowDemoWindow();
}
