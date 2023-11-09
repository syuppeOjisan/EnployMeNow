#include "Gameobject/grandpa.h"

#include "Component/Shader.h"
#include "Scene/Base/scene.h"
#include "GameObject/player.h"
#include "System/manager.h"
#include "System/collision.h"
using namespace DirectX::SimpleMath;

void grandpa::Init()
{
	//AddComponent<Shader>()->Load("shader\\vertexLightingOneSkinVS.cso", "shader\\vertexLightingPS.cso");
	AddComponent<Shader>()->Load("shader\\vertexLightingVS.cso", "shader\\vertexLightingPS.cso");
	m_Model = AddComponent<AnimationModel>();

	m_Model->Load("asset\\model\\Grandpa_Tpose.fbx");							// animation ok
	m_Model->LoadAnimation("asset\\model\\Grandpa_Idel.fbx", "Idle");
	m_Model->LoadAnimation("asset\\model\\Grandpa_Die.fbx", "Die");
	m_Model->SetFirstAnimation("Idle");

	// サイズ調整
	m_Scale = Vector3(0.02f, 0.02f, 0.02f);

	// 当たり判定設定
	this->m_AABBCollision = SetAABB(this->m_Position, this->m_Scale.x, this->m_Scale.y, this->m_Scale.z);
}

void grandpa::Update()
{
	// コリジョン情報更新
	this->m_AABBCollision = SetAABB(this->m_Position, 1, 1, 1);


	//// 現在シーンを取得
	//Scene* scene = Manager::GetScene();
	//// プレイヤーとの当たり判定
	//{
	//	Player* pPlayer = scene->GetGameObject<Player>();

	//	if (pPlayer)
	//	{
	//		Vector3 position = pPlayer->GetPosition();
	//		Vector3 scale = pPlayer->GetScale();

	//		AABB aabbBox;
	//		AABB aabbPlayer;

	//		aabbBox = SetAABB(position, fabs(scale.x), fabs(scale.y), fabs(scale.z));
	//		aabbPlayer = SetAABB(m_Position, fabs(m_Scale.x), fabs(m_Scale.y), fabs(m_Scale.z));

	//		bool sts = CollisionAABB(aabbBox, aabbPlayer);

	//		if (sts)
	//		{
	//			m_Model->SetNextAnimation("Die");
	//		}
	//	}
	//}

	m_Frame++;
}

void grandpa::PreDraw()
{
	m_Model->Update("Idle", m_Frame, "Run", m_Frame, m_BlendRate);
}
