#include "System/main.h"
#include "System/manager.h"
#include "System/renderer.h"
#include "Scene/Base/scene.h"
#include "Component/modelRenderer.h"
#include "Component/shadow.h"
#include "Component/shader.h"
#include "GameObject/explosion.h"
#include "GameObject/score.h"
#include "GameObject/player.h"
#include "GameObject/bullet.h"
#include "GameObject/enemy.h"


using namespace DirectX::SimpleMath;

void Bullet::Load()
{

	ModelRenderer::Preload("asset\\model\\bullet.obj");

}


void Bullet::Unload()
{

}



void Bullet::Init()
{
	AddComponent<Shader>()->Load("shader\\vertexLightingVS.cso", "shader\\vertexLightingPS.cso");
	AddComponent<ModelRenderer>()->Load("asset\\model\\bullet.obj");

	AddComponent<Shadow>()->SetSize(0.5f);

	bulletMode = BULLET_MODE::SIZE_UP;	// �����l�ł̓T�C�Y��傫�����郂�[�h
}


void Bullet::Update()
{
	m_Position += m_Velocity;


	if (m_Position.Length() > 100.0f)
		SetDestroy();


	// �V�[������G�̃��X�g���擾
	Scene* scene = Manager::GetScene();
	std::vector<Enemy*> enemyList = scene->GetGameObjects<Enemy>();

	// ��Ŏ擾�������X�g���g���Ă�
	// �G�ƒe�̓����蔻��
	for (Enemy* enemy : enemyList)
	{
		Vector3 enemyPosition = enemy->GetPosition();

		Vector3 direction = m_Position - enemyPosition;
		float length = direction.Length();

		// �������Ă���Ȃ�G�A�e���폜���G�t�F�N�g��\��
		if (length < 2.0f)
		{
			scene->AddGameObject<Explosion>(2)->SetPosition(enemyPosition + Vector3(0.0f, 1.0f, 0.0f));

			Score* score = scene->GetGameObject<Score>();
			score->AddCount(10);


			enemy->SetDestroy();
			SetDestroy();
			return;
		}
	}
}

BULLET_MODE Bullet::GetBulletMode()
{
	return bulletMode;
}

void Bullet::SetBulletMode(BULLET_MODE mode)
{
	this->bulletMode = mode;
}

