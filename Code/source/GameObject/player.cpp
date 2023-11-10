#include "System/main.h"
#include "System/manager.h"
#include "System/renderer.h"
#include "System/collision.h"
#include "System/input.h"
#include "Component/modelRenderer.h"
#include "Component/animationModel.h"
#include "Component/shader.h"
#include "Component/audio.h"
#include "Component/shadow.h"
#include "GameObject/player.h"
#include "GameObject/cylinder.h"
#include "GameObject/box.h"
#include "GameObject/bullet.h"
#include "GameObject/goal.h"
#include "GameObject/PlayerCamera.h"
#include "GameObject/Item_X.h"
#include "GameObject/Item_Y.h"
#include "GameObject/Item_Z.h"

#include <iostream>

using namespace DirectX::SimpleMath;

void Player::Init()
{
	CharacterBase::Init();

	AddComponent<Shader>()->Load("shader\\vertexLightingOneSkinVS.cso", "shader\\vertexLightingPS.cso");
	m_pModel = AddComponent<AnimationModel>();
	
	m_pModel->Load("asset\\model\\Player_Tpose.fbx");							// animation ok
	m_pModel->LoadAnimation("asset\\model\\Player_Idle.fbx", "Idle");
	m_pModel->LoadAnimation("asset\\model\\Player_Walk.fbx", "Run");
	m_pModel->LoadAnimation("asset\\model\\Player_Punching.fbx", "Punching");

	AddComponent<Shadow>()->SetSize(1.5f);

	// サイズ調整
	m_Scale = Vector3(0.02f, 0.02f, 0.02f);

	// 当たり判定設定
	this->m_AABBCollision = SetAABB(this->m_Position, this->m_Scale.x, this->m_Scale.y, this->m_Scale.z);

	// モーション変数初期化
	m_NowAnimation = "Idle";
	m_PrevAnimation = "Idle";
}

void Player::Update()
{
	Vector3 oldPosition = m_Position;

	Scene* nowscene = Manager::GetScene();
	Camera* cameraobj = nowscene->GetGameObject<PlayerCamera>();

	Matrix viewmtx = cameraobj->GetViewMatrix();
	Vector3 ZAxis = Vector3(viewmtx._13, 0.0f, viewmtx._33);
	Vector3 XAxis = Vector3(viewmtx._11, 0.0f, viewmtx._31);

	// 左右に回転
	if (Input::GetKeyPress('A'))
	{
		m_Position -= XAxis * 0.4f;
	}
	if (Input::GetKeyPress('D'))
	{
		m_Position += XAxis * 0.3f;
	}

	// 前方ベクトルを取得
	Vector3 forward = m_pCamera->GetCameraFrontVec();
	forward.Normalize();

	forward.x *= 0.3;
	forward.y = 0;
	forward.z *= 0.3;

	// 前後移動
	if (Input::GetKeyPress('W'))
	{
		SetVelocity(forward);
		SetNextAnimation("Run");
	}
	else if (Input::GetKeyPress('S'))
	{
		SetVelocity(-forward);
		SetNextAnimation("Run");
	}
	else if (Input::GetKeyPress('P'))
	{
		SetNextAnimation("Punching");
	}
	else
	{
		SetNextAnimation("Idle");
	}

	//ジャンプ
	if (Input::GetKeyTrigger(VK_SPACE))
	{
		m_Velocity.y = 0.2f;
	}


	// TODO:弾を見ている方向に撃つ
	// 一応できた
	//弾発射
	if (Input::GetKeyTrigger('K'))
	{
		forward.y += 0.2f;
		forward.Normalize();
		Scene* scene = Manager::GetScene();
		Bullet* bullet = scene->AddGameObject<Bullet>(2);
		bullet->SetPosition(m_Position + Vector3(0.0f,2.0f,0.0f));
		bullet->SetVelocity(forward);
	}


	//重力
	m_Velocity.y -= 0.015f;

	//抵抗
	m_Velocity.y -= m_Velocity.y * 0.01f;


	//接地
	float groundHeight = 0.0f;
	if (GetPosition().y <= groundHeight)
	{
		m_isLanding = true;
	}
	else
	{
		m_isLanding = false;
	}


	// 当たり判定更新
	this->m_AABBCollision = SetAABB(this->m_Position, 1, 1, 1);

	// 現在シーンを取得
	Scene* scene = Manager::GetScene();

	// TODO:当たり判定をいい感じに変える
	// せめてゲームシーン内で判定を行いたい

	// cylinderとの当たり判定
	{
		std::vector<Cylinder*> cylinderlist = scene->GetGameObjects<Cylinder>();

		for (const auto& cylobj : cylinderlist) {
			Vector3 intersectiopoint{};
			// プレイヤのBS作成
			BoundingSphere playerbs{};

			playerbs.center = m_Position;
			playerbs.radius = fabs(m_Scale.x / 2.0f);

			//　円柱の当たり判定データ作成 
			Vector3 position = cylobj->GetPosition();
			Vector3 scale = cylobj->GetScale();

			BoundingCylinder boundingcylinder{};
			boundingcylinder.bottom = position;
			boundingcylinder.bottom.y = 0.0f;

			boundingcylinder.top = position;
			boundingcylinder.top.y = position.y + scale.y - 0.5f;

			boundingcylinder.radius = scale.x;
			bool sts = CollisionSphereCylinder(playerbs, boundingcylinder);
			if (sts) {
				if (m_Position.y < position.y + scale.y - 0.5f)
				{
					m_Position.x = oldPosition.x;
					m_Position.z = oldPosition.z;
				}
				else
				{
					groundHeight = position.y + scale.y;
				}
				break;
			}
		}
	}

	// Boxとの当たり判定
	{
		std::vector<Box*> boxlist = scene->GetGameObjects<Box>();

		for (const auto& boxobj : boxlist) {
			// BOXの当たり判定データ作成 
			Vector3 position = boxobj->GetPosition();
			Vector3 scale = boxobj->GetScale();

			if (position.x - scale.x - 0.5f < m_Position.x && m_Position.x < position.x + scale.x + 0.5f &&
				position.z - scale.z - 0.5f < m_Position.z && m_Position.z < position.z + scale.z + 0.5f)
			{
				if (m_Position.y < position.y + scale.y * 2.0f - 0.5f)
				{
					m_isLanding = true;
					m_Position.x = oldPosition.x;
					m_Position.z = oldPosition.z;
				}
				else
				{
					groundHeight = position.y + scale.y * 2.0f;
				}
			}
			else
			{
				m_isLanding = false;
			}
		}
	}


	// 位置が０以下なら地面位置にセットする
	if (m_Position.y < groundHeight && m_Velocity.y < 0.0f)
	{
		m_Position.y = groundHeight;
		m_Velocity.y = 0.0f;
	}

	////TODO:ブレンドレートが初期化できてないからタイミングを見つける
	////ブレンドレート自体をモデルの方に持たせてもいいかも
	//if (Input::GetKeyPress('W'))
	//{
	//	m_Model->SetNextAnimation("Run");
	//	m_BlendRate += 0.1f;

	//}
	//else if (Input::GetKeyPress('S'))
	//{
	//	m_Model->SetNextAnimation("Run");
	//	m_BlendRate += 0.1f;
	//}
	//else if (Input::GetKeyPress('P'))
	//{
	//	//TODO: パンチ中は手の先に当たり判定を付けて殴れるようにする
	//	m_Model->SetNextAnimation("Punching");
	//	m_BlendRate += 0.1f;
	//}
	//else if(m_Model->GetIsAnimBlend())
	//{
	//	m_Model->SetNextAnimation("Idle");
	//	m_Model->SetBlendSpeed(0.1f);
	//	m_BlendRate += 0.1f;
	//}
	//m_Frame++;

	CharacterBase::Update();
}


void Player::PreDraw()
{
	m_pModel->Update(m_PrevAnimation, m_PrevAnimationFrame, m_NowAnimation, m_NowAnimationFrame, m_BlendRate);
}

void Player::SetCamera(PlayerCamera* _camera)
{
	this->m_pCamera = _camera;
}