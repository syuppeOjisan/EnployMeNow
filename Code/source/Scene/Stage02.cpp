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
#include "GameObject/score.h"
#include "GameObject/transition.h"
#include "GameObject/goal.h"
#include "GameObject/sky.h"
#include "GameObject/Polygon2D.h"
#include "GameObject/Item_X.h"
#include "GameObject/Item_Y.h"
#include "GameObject/Item_Z.h"

#include "Scene/result.h"
#include "Scene/Stage02.h"

#include "Component/Object_SizeChange.h"
#include "Component/Object_ScaleX.h"
#include "Component/Object_ScaleY.h"
#include "Component/Object_ScaleZ.h"
#include "Component/sprite.h"
#include "Component/shader.h"
#include "Component/audio.h"

using namespace DirectX::SimpleMath;

// �Q�[����������
void Stage02::Init()
{
	Bullet::Load();

	AddGameObject<Sky>(1);
	AddGameObject<Box>(1)->SetScale(Vector3(10.0f,0.2f,10.0f));
	AddGameObject<Player>(1)->SetPosition(Vector3(0.0f,5.0f,0.0f));

	// �`�F�b�N����
	{
		// �J�����𐶐����Q�[���V�[���ɒǉ��A�v���C���[���^�[�Q�b�g�Ɏw��
		PlayerCamera* pCamera = AddGameObject<PlayerCamera>(0);
		pCamera->SetTarget(GetGameObject<Player>());
		// �v���C���[�ɂ��J�����̏���n��
		GetGameObject<Player>()->SetCamera(pCamera);
	}

	// TODO:���������̃X�e�[�W�����

	// �A�C�e��X
	{
		ItemBase* itemX = AddGameObject<Item_X>(1);
		itemX->SetPosition(Vector3(0.0f,0.8f,0.0f));
		itemX->SetScale(Vector3(3.0f, 3.0f, 3.0f));
	}
	
	// �A�C�e��Y
	{
		ItemBase* itemY = AddGameObject<Item_Y>(1);
		itemY->SetPosition(Vector3(-23.0f, 2.8f, 0.0f));
		itemY->SetScale(Vector3(3.0f, 3.0f, 3.0f));
	}

	// �A�C�e��Z
	{
		ItemBase* itemZ = AddGameObject<Item_Z>(1);
		itemZ->SetPosition(Vector3(-25.0f, 8.0f, 0.0f));
		itemZ->SetScale(Vector3(3.0f, 3.0f, 3.0f));
	}


	{
		Box* box = AddGameObject<Box>(1);
		box->SetPosition(Vector3(-19.0f, -0.5f, 0.0f));
		box->SetScale(Vector3(1.0f, 1.0f, 1.0f));
		box->AddComponent<Object_ScaleX>();
	}

	{
		Box* box = AddGameObject<Box>(1);
		box->SetPosition(Vector3(-25.0f, -0.5f, 0.0f));
		box->SetScale(Vector3(1.0f, 1.0f, 1.0f));
		box->AddComponent<Object_ScaleY>();
	}

	{
		Box* box = AddGameObject<Box>(1);
		box->SetPosition(Vector3(0.0f, -0.5f, 17.0f));
		box->SetScale(Vector3(1.0f, 1.0f, 1.0f));
		box->AddComponent<Object_ScaleZ>();
	}

	// �`�F�b�N����
	{
		Goal* goal = AddGameObject<Goal>(1);
		goal->SetPosition(Vector3(0.0f, 2.0f, 20.0f));
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

		GameObject* itemX = AddGameObject<GameObject>(3);
		itemX->AddComponent<Shader>()->Load("shader\\unlitTextureVS.cso", "shader\\unlitTexturePS.cso");
		itemX->AddComponent<Sprite>()->Init(750.0f, 500.0f, 500, 250, "asset\\texture\\Item_X.png");

		GameObject* itemY = AddGameObject<GameObject>(3);
		itemY->AddComponent<Shader>()->Load("shader\\unlitTextureVS.cso", "shader\\unlitTexturePS.cso");
		itemY->AddComponent<Sprite>()->Init(750.0f, 500.0f, 500, 250, "asset\\texture\\Item_Y.png");

		GameObject* itemZ = AddGameObject<GameObject>(3);
		itemZ->AddComponent<Shader>()->Load("shader\\unlitTextureVS.cso", "shader\\unlitTexturePS.cso");
		itemZ->AddComponent<Sprite>()->Init(750.0f, 500.0f, 500, 250, "asset\\texture\\Item_Z.png");

		GameObject* itemNone = AddGameObject<GameObject>(3);
		itemNone->AddComponent<Shader>()->Load("shader\\unlitTextureVS.cso", "shader\\unlitTexturePS.cso");
		itemNone->AddComponent<Sprite>()->Init(750.0f, 500.0f, 500, 250, "asset\\texture\\Item_None.png");
	}

	// ��ʑJ�ڃI�u�W�F�N�g��o�^
	m_Transition = AddGameObject<Transition>(3);
	m_Transition->FadeIn();
}

void Stage02::Uninit()
{

}

// �Q�[���X�V����
void Stage02::Update()
{
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
	
	// UI�؂�ւ�
	switch (GetGameObject<Player>()->GetBulletKind())
	{
	case CHANGE_X_AXSIS:
		GetGameObjects<GameObject>()[4]->GetComponent<Sprite>()->SetActive(true);
		GetGameObjects<GameObject>()[5]->GetComponent<Sprite>()->SetActive(false);
		GetGameObjects<GameObject>()[6]->GetComponent<Sprite>()->SetActive(false);
		GetGameObjects<GameObject>()[7]->GetComponent<Sprite>()->SetActive(false);
		break;
	case CHANGE_Y_AXSIS:
		GetGameObjects<GameObject>()[4]->GetComponent<Sprite>()->SetActive(false);
		GetGameObjects<GameObject>()[5]->GetComponent<Sprite>()->SetActive(true);
		GetGameObjects<GameObject>()[6]->GetComponent<Sprite>()->SetActive(false);
		GetGameObjects<GameObject>()[7]->GetComponent<Sprite>()->SetActive(false);
		break;
	case CHANGE_Z_AXSIS:
		GetGameObjects<GameObject>()[4]->GetComponent<Sprite>()->SetActive(false);
		GetGameObjects<GameObject>()[5]->GetComponent<Sprite>()->SetActive(false);
		GetGameObjects<GameObject>()[6]->GetComponent<Sprite>()->SetActive(true);
		GetGameObjects<GameObject>()[7]->GetComponent<Sprite>()->SetActive(false);
		break;
	case NONE:
		GetGameObjects<GameObject>()[4]->GetComponent<Sprite>()->SetActive(false);
		GetGameObjects<GameObject>()[5]->GetComponent<Sprite>()->SetActive(false);
		GetGameObjects<GameObject>()[6]->GetComponent<Sprite>()->SetActive(false);
		GetGameObjects<GameObject>()[7]->GetComponent<Sprite>()->SetActive(true);
		break;
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
