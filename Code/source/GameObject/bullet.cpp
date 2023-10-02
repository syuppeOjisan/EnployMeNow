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

	bulletMode = BULLET_MODE::SIZE_UP;	// 初期値ではサイズを大きくするモード
}


void Bullet::Update()
{
	m_Position += m_Velocity;


	if (m_Position.Length() > 100.0f)
		SetDestroy();


	// シーンから敵のリストを取得
	Scene* scene = Manager::GetScene();
	std::vector<Enemy*> enemyList = scene->GetGameObjects<Enemy>();

	// 上で取得したリストを使ってる
	// 敵と弾の当たり判定
	for (Enemy* enemy : enemyList)
	{
		Vector3 enemyPosition = enemy->GetPosition();

		Vector3 direction = m_Position - enemyPosition;
		float length = direction.Length();

		// 当たっているなら敵、弾を削除しエフェクトを表示
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

