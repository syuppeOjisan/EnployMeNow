#include "System/main.h"
#include "System/manager.h"
#include "System/input.h"

#include "Scene/title.h"
#include "Scene/game.h"
#include "Scene/result.h"

#include "Component/shader.h"
#include "Component/sprite.h"

#include "GameObject/transition.h"


void Result::Init()
{
	GameObject* resultLogo = AddGameObject<GameObject>(3);			// 3�̓��C���ԍ�
	resultLogo->AddComponent<Shader>()->Load("shader\\unlitTextureVS.cso", "shader\\unlitTexturePS.cso");
	resultLogo->AddComponent<Sprite>()->Init(0.0f, 0.0f, SCREEN_WIDTH, SCREEN_HEIGHT, "asset\\texture\\result.jpg");

	m_Transition = AddGameObject<Transition>(3);					// 3�̓��C���ԍ�
	m_Transition->FadeIn();
}


void Result::Update()
{
	//  ��ʑJ�ځi�t�F�[�h�C���j���I�����Ă���Ȃ�
	if (m_Transition->GetState() == Transition::State::Stop) {
		if (Input::GetKeyTrigger(VK_RETURN))
		{
			m_Transition->FadeOut();
		}
	}

	// ��ʑJ��(�t�F�[�h�A�E�g)���I�����Ă邩�H
	if (m_Transition->GetState() == Transition::State::Finish)
	{
		Manager::SetScene<Title>();
	}
}
