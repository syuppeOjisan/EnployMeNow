#include "Gameobject/grandpa.h"

#include "Component/Shader.h"
#include "Scene/Base/scene.h"
#include "GameObject/player.h"
#include "System/manager.h"
#include "System/collision.h"
#include "Component/shadow.h"

using namespace DirectX::SimpleMath;

void grandpa::Init()
{
	CharacterBase::Init();
	AddComponent<Shader>()->Load("shader\\vertexLightingOneSkinVS.cso", "shader\\vertexLightingPS.cso");
	m_pModel = AddComponent<AnimationModel>();

	m_pModel->Load("asset\\model\\Grandpa_Tpose.fbx");							// animation ok
	m_pModel->LoadAnimation("asset\\model\\Grandpa_Idel.fbx", "Idle");
	m_pModel->LoadAnimation("asset\\model\\Grandpa_HIt.fbx", "Hit");
	m_pModel->LoadAnimation("asset\\model\\Grandpa_Die.fbx", "Die");

	AddComponent<Shadow>()->SetSize(1.5f);

	// サイズ調整
	m_Scale = Vector3(0.02f, 0.02f, 0.02f);

	// 当たり判定設定
	this->m_AABBCollision = MakeAABB(this->m_Position, this->m_Scale.x, this->m_Scale.y, this->m_Scale.z);
	this->m_OBBCollision = MakeOBB(this->m_pModel->GetVertexData());

	// モーション変数初期化
	m_NowAnimation = "Idle";
	m_PrevAnimation = "Idle";
}

void grandpa::Update()
{
	// コリジョン情報更新
	this->m_AABBCollision = MakeAABB(this->m_Position, 1, 1, 1);
	this->m_OBBCollision = MakeOBB(this->m_pModel->GetVertexData());

	CharacterBase::Update();
}

void grandpa::PreDraw()
{
	m_pModel->Update(m_PrevAnimation, m_PrevAnimationFrame, m_NowAnimation, m_NowAnimationFrame, m_BlendRate);
}
