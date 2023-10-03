#include "System/main.h"
#include "System/manager.h"
#include "System/renderer.h"
#include "System/input.h"
#include "System/manager.h"
#include "System/utility.h"

#include "GameObject/camera.h"
#include "GameObject/PlayerCamera.h"
#include "GameObject/field.h"
#include "GameObject/player.h"
#include "GameObject/box.h"
#include "GameObject/cylinder.h"
#include "GameObject/bullet.h"
#include "GameObject/enemy.h"
#include "GameObject/battery.h"
#include "GameObject/score.h"
#include "GameObject/explosion.h"
#include "GameObject/transition.h"
#include "GameObject/goal.h"
#include "GameObject/sky.h"
#include "GameObject/Polygon2D.h"

#include "Scene/result.h"
#include "Scene/game.h"

#include "Component/Object_SizeChange.h"
#include "Component/sprite.h"
#include "Component/shader.h"
#include "Component/audio.h"

using namespace DirectX::SimpleMath;

// �Q�[����������
void Game::Init()
{
	Bullet::Load();
	Explosion::Load();

	AddGameObject<Sky>(1);
	AddGameObject<Field>(1);
	AddGameObject<Player>(1);
	AddGameObject<Score>(3);

	// �G�ǉ�
	AddGameObject<Enemy>(1)->SetPosition(Vector3(0.0f, 0.0f, 5.0f));
	AddGameObject<Enemy>(1)->SetPosition(Vector3(5.0f, 0.0f, 5.0f));
	AddGameObject<Enemy>(1)->SetPosition(Vector3(10.0f, 0.0f, 5.0f));
	AddGameObject<Enemy>(1)->SetPosition(Vector3(15.0f, 0.0f, 5.0f));

	// �`�F�b�N����
	{
		PlayerCamera* pcamera = AddGameObject<PlayerCamera>(0);
		pcamera->SetTarget(GetGameObject<Player>());
	}

	// �`�F�b�N����
	{
		Box* box = AddGameObject<Box>(1);
		box->SetPosition(Vector3(-5.0f, 0.0f, 5.0f));
		box->SetScale(Vector3(1.0f, 1.0f, 1.0f));
		box->AddComponent<Object_SizeChange>();
	}

	// �`�F�b�N����
	{
		Box* box = AddGameObject<Box>(1);
		box->SetPosition(Vector3(-11.0f, 0.0f, 5.0f));
		box->SetScale(Vector3(1.0f, 1.0f, 1.0f));
		box->AddComponent<Object_SizeChange>();
	}

	// �`�F�b�N����
	{
		Box* box = AddGameObject<Box>(1);
		box->SetPosition(Vector3(-11.0f, 0.0f, 11.0f));
		box->SetScale(Vector3(1.0f, 1.0f, 1.0f));
		box->AddComponent<Object_SizeChange>();
	}

	// �`�F�b�N����
	{
		Cylinder* cylinder = AddGameObject<Cylinder>(1);
		cylinder->SetPosition(Vector3(-11.0f, 0.0f, 20.0f));
		cylinder->SetScale(Vector3(3.0f, 6.0f, 3.0f));
	}

	// �`�F�b�N����
	{
		Box* box = AddGameObject<Box>(1);
		box->SetPosition(Vector3(0.0f, 0.0f, 20.0f));
		box->SetScale(Vector3(1.0f, 1.0f, 1.0f));
		box->AddComponent<Object_SizeChange>();
	}

	// �`�F�b�N����
	{
		Cylinder* cylinder = AddGameObject<Cylinder>(1);
		cylinder->SetPosition(Vector3(11.0f, 0.0f, 20.0f));
		cylinder->SetScale(Vector3(3.0f, 6.0f, 3.0f));
	}

	// �`�F�b�N����
	{
		Goal* goal = AddGameObject<Goal>(1);
		goal->SetPosition(Vector3(11.0f, 9.0f, 20.0f));
		goal->SetScale(Vector3(0.5f, 0.5f, 0.5f));
	}

	// UI�p�摜�ݒ�
	{
		GameObject* sizeUp = AddGameObject<GameObject>(3);
		sizeUp->AddComponent<Shader>()->Load("shader\\unlitTextureVS.cso", "shader\\unlitTexturePS.cso");
		sizeUp->AddComponent<Sprite>()->Init(25.0f, 50.0f, 300, 100, "asset\\texture\\sizeUp.png");

		GameObject* sizeDown = AddGameObject<GameObject>(3);
		sizeDown->AddComponent<Shader>()->Load("shader\\unlitTextureVS.cso", "shader\\unlitTexturePS.cso");
		sizeDown->AddComponent<Sprite>()->Init(25.0f, 50.0f, 300, 100, "asset\\texture\\sizeDown.png");

		GameObject* switchAmmo = AddGameObject<GameObject>(3);
		switchAmmo->AddComponent<Shader>()->Load("shader\\unlitTextureVS.cso", "shader\\unlitTexturePS.cso");
		switchAmmo->AddComponent<Sprite>()->Init(25.0f, 400.0f, 200, 100, "asset\\texture\\switchAmmo.png");

		GameObject* fireAmmo = AddGameObject<GameObject>(3);
		fireAmmo->AddComponent<Shader>()->Load("shader\\unlitTextureVS.cso", "shader\\unlitTexturePS.cso");
		fireAmmo->AddComponent<Sprite>()->Init(50.0f, 500.0f, 150, 195, "asset\\texture\\fireAmmo.png");
	}

	AddGameObject<Battery>(1)->SetPosition(Vector3(20.0f, 0.0f, 5.0f));

	// ��ʑJ�ڃI�u�W�F�N�g��o�^
	m_Transition = AddGameObject<Transition>(3);
	m_Transition->FadeIn();
}

// �Q�[���I������
void Game::Uninit()
{
}

// �Q�[���X�V����
void Game::Update()
{
	// �t�F�[�h�C�����I�����Ă��邩�H	
//	if (m_Transition->GetState() == Transition::State::Stop) {
//		if (Input::GetKeyTrigger(VK_RETURN))
//		{
//			m_Transition->FadeOut();
//		}
//	}


	// �v���C���[�����e�̃��[�h�ύX
	// UI��ύX
	if (GetGameObject<Player>()->GetBulletMode() == SIZE_UP)
	{
		GetGameObjects<GameObject>()[0]->GetComponent<Sprite>()->SetActive(true);
		GetGameObjects<GameObject>()[1]->GetComponent<Sprite>()->SetActive(false);
	}
	else if (GetGameObject<Player>()->GetBulletMode() == SIZE_DOWN)
	{
		GetGameObjects<GameObject>()[0]->GetComponent<Sprite>()->SetActive(false);
		GetGameObjects<GameObject>()[1]->GetComponent<Sprite>()->SetActive(true);
	}

	// �S�[�����Ă��Ȃ��̂ł����
	if (!m_Goal)
	{
		Goal* goal = GetGameObject<Goal>();

		// �S�[�������ۂɃS�[���I�u�W�F�N�g�͍폜�����
		if (goal == nullptr)
		{
			m_Goal = true;

			// �Q�b��ɃX���b�h�𐶐����ăt�F�[�h�A�E�g�J�n
			Invoke([=]() { m_Transition->FadeOut(); }, 2000);
		}
	}

	// �t�F�[�h�A�E�g���I�����Ă��邩�H
	if (m_Transition->GetState() == Transition::State::Finish)
	{
		Manager::SetScene<Result>();
	}
}
