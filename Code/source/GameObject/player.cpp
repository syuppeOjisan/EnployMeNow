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
	//AddComponent<Shader>()->Load("shader\\vertexLightingOneSkinVS.cso", "shader\\vertexLightingPS.cso");
	AddComponent<Shader>()->Load("shader\\vertexLightingVS.cso", "shader\\vertexLightingPS.cso");
	m_Model = AddComponent<AnimationModel>();
	
	m_Model->Load("asset\\model\\Player_Tpose.fbx");							// animation ok
	m_Model->LoadAnimation("asset\\model\\Player_Idle.fbx", "Idle");
	m_Model->LoadAnimation("asset\\model\\Player_Walk.fbx", "Run");
	m_Model->LoadAnimation("asset\\model\\Player_Punching.fbx", "Punching");
	m_Model->SetFirstAnimation("Idle");

	{
		//m_Model->Load("asset\\model\\Player_Tpose.fbx");						// animation ok
		//m_Model->LoadAnimation("asset\\model\\Player_Tpose.fbx", "Idle");
		//m_Model->LoadAnimation("asset\\model\\Player_Tpose.fbx", "Run");
		
		//m_Model->Load("asset\\model\\Akai2.fbx");								// animation ok
		//m_Model->LoadAnimation("asset\\model\\Akai_Walk.fbx", "Idle");
		//m_Model->LoadAnimation("asset\\model\\Akai_Walk.fbx", "Run");
		
		//m_Model->Load("data\\model\\Walking\\Walking2.fbx");					// animation ok
		//m_Model->LoadAnimation("data\\model\\Walking\\Walking2.fbx", "Idle");
		//m_Model->LoadAnimation("data\\model\\Walking\\Walking2.fbx", "Run");
		
		//m_Model->Load("data\\model\\Walking\\Walking.fbx");					// animation ok
		//m_Model->LoadAnimation("data\\model\\Walking\\Walking.fbx", "Idle");
		//m_Model->LoadAnimation("data\\model\\Walking\\Walking.fbx", "Run");
		
		//m_Model->Load("data\\model\\Walking\\Walking.dae");					// animation ok
		//m_Model->LoadAnimation("data\\model\\Walking\\Walking.dae", "Idle");	// animation ok
		//m_Model->LoadAnimation("data\\model\\Walking\\Walking.dae", "Run");	// animation ok
	}


	AddComponent<Shadow>()->SetSize(1.5f);

	m_BulletEquip = NONE;

	// SE読み込み
	m_SEList["Fire"] = AddComponent<Audio>();
	m_SEList["Fire"]->Load("asset\\audio\\SE\\bulletFire.wav");

	m_SEList["Goal"] = AddComponent<Audio>();
	m_SEList["Goal"]->Load("asset\\audio\\SE\\goal.wav");

	m_SEList["Get"] = AddComponent<Audio>();
	m_SEList["Get"]->Load("asset\\audio\\SE\\itemGet.wav");

	// サイズ調整
	m_Scale = Vector3(0.02f, 0.02f, 0.02f);
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

	if (Input::GetKeyPress('W'))
	{
		forward.y = 0;
		m_Position += forward * 0.3f;
	}
	if (Input::GetKeyPress('S'))
	{
		forward.y = 0;
		m_Position -= forward * 0.3f;
	}

	//ジャンプ
	if (Input::GetKeyTrigger(VK_SPACE))
	{
		m_Velocity.y = 0.2f;
	}

	// 弾のモード変更
	if (Input::GetKeyTrigger('1'))
	{
		m_BulletMode = SIZE_UP;
	}

	if (Input::GetKeyTrigger('2'))
	{
		m_BulletMode = SIZE_DOWN;
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
		bullet->SetBulletMode(m_BulletMode);
		bullet->SetBulletKind(m_BulletEquip);

		m_SEList["Fire"]->Play();
	}


	//重力
	m_Velocity.y -= 0.015f;

	//抵抗
	m_Velocity.y -= m_Velocity.y * 0.01f;

	//移動
	m_Position += m_Velocity;

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

	// アイテムとの当たり判定
	{
		// アイテムXのとの当たり判定
		{
			std::vector<Item_X*> itemXList = scene->GetGameObjects<Item_X>();
			for (const auto& itemObj : itemXList)
			{
				Vector3 position = itemObj->GetPosition();
				Vector3 scale = itemObj->GetScale();

				AABB aabbBox;
				AABB aabbPlayer;

				aabbBox = SetAABB(position, fabs(2.0f * scale.x), fabs(2.0f * scale.y), fabs(2.0f * scale.z));
				aabbPlayer = SetAABB(m_Position, fabs(1.0f * m_Scale.x), fabs(1.0f * m_Scale.y), fabs(1.0f * m_Scale.z));

				bool sts = CollisionAABB(aabbBox, aabbPlayer);

				if (sts)
				{
					m_BulletEquip = BULLET_KIND::CHANGE_X_AXSIS;
					if (m_isSEplay_Z)
					{
						m_SEList["Get"]->Play();
						m_isSEplay_Z = true;
					}
				}
				else
				{
					m_isSEplay_Z = false;
				}
			}
		}

		// アイテムYのとの当たり判定
		{
			std::vector<Item_Y*> itemYList = scene->GetGameObjects<Item_Y>();
			for (const auto& itemObj : itemYList)
			{
				Vector3 position = itemObj->GetPosition();
				Vector3 scale = itemObj->GetScale();

				AABB aabbBox;
				AABB aabbPlayer;

				aabbBox = SetAABB(position, fabs(2.0f * scale.x), fabs(2.0f * scale.y), fabs(2.0f * scale.z));
				aabbPlayer = SetAABB(m_Position, fabs(1.0f * m_Scale.x), fabs(1.0f * m_Scale.y), fabs(1.0f * m_Scale.z));

				bool sts = CollisionAABB(aabbBox, aabbPlayer);

				if (sts)
				{
					m_BulletEquip = BULLET_KIND::CHANGE_Y_AXSIS;
					if (m_isSEplay_Z)
					{
						m_SEList["Get"]->Play();
						m_isSEplay_Y = true;
					}
				}
				else
				{
					m_isSEplay_Y = false;
				}
			}
		}

		// アイテムZのとの当たり判定
		{
			std::vector<Item_Z*> itemZList = scene->GetGameObjects<Item_Z>();
			for (const auto& itemObj : itemZList)
			{
				Vector3 position = itemObj->GetPosition();
				Vector3 scale = itemObj->GetScale();

				AABB aabbBox;
				AABB aabbPlayer;

				aabbBox = SetAABB(position, fabs(2.0f * scale.x), fabs(2.0f * scale.y), fabs(2.0f * scale.z));
				aabbPlayer = SetAABB(m_Position, fabs(1.0f * m_Scale.x), fabs(1.0f * m_Scale.y), fabs(1.0f * m_Scale.z));

				bool sts = CollisionAABB(aabbBox, aabbPlayer);

				if (sts)
				{
					m_BulletEquip = BULLET_KIND::CHANGE_Z_AXSIS;
					if (m_isSEplay_Z)
					{
						m_SEList["Get"]->Play();
						m_isSEplay_Z = true;
					}
				}
				else
				{
					m_isSEplay_Z = false;
				}
			}
		}
	}

	// 位置が０以下なら地面位置にセットする
	if (m_Position.y < groundHeight && m_Velocity.y < 0.0f)
	{
		m_Position.y = groundHeight;
		m_Velocity.y = 0.0f;
	}

	// ゴールとの当たり判定
	{
		Goal* goal = scene->GetGameObject<Goal>();

		if (goal)
		{
			Vector3 position = goal->GetPosition();
			Vector3 scale = goal->GetScale();

			AABB aabbBox;
			AABB aabbPlayer;

			aabbBox = SetAABB(position, fabs(2.0f * scale.x), fabs(2.0f * scale.y), fabs(2.0f * scale.z));
			aabbPlayer = SetAABB(m_Position, fabs(1.0f * m_Scale.x), fabs(1.0f * m_Scale.y), fabs(1.0f * m_Scale.z));

			bool sts = CollisionAABB(aabbBox, aabbPlayer);

			if (sts)
			{
				m_SEList["Goal"]->Play();
				goal->SetDestroy();
			}
		}
	}


	if (Input::GetKeyPress('W'))
	{
		m_Model->SetNextAnimation("Run");
		m_BlendRate += 0.1f;
		m_Frame++;

	}
	else if (Input::GetKeyPress('S'))
	{
		m_Model->SetNextAnimation("Run");
		m_BlendRate += 0.1f;
		m_Frame++;
	}
	else if (Input::GetKeyPress('P'))
	{
		//TODO: パンチ中は手の先に当たり判定を付けて殴れるようにする
		m_Model->SetNextAnimation("Punching");
		m_BlendRate += 0.1f;
		m_Frame++;
	}
	else /*if(モーションブレンドが終わったら)*/
	{
		m_Model->SetNextAnimation("Idle");
		m_BlendRate += 0.1f;
		m_Frame++;
	}

	if (m_BlendRate > 1.0f)
		m_BlendRate = 1.0f;
	if (m_BlendRate < 0.0f)
		m_BlendRate = 0.0f;


	//std::cout << "PayerPos X[" << GetPosition().x << "]Y[" << GetPosition().y << "]Z[" << GetPosition().z << "]" << std::endl;

	//if (true)
	//{
	//	std::cout << "なんかのキーが離されました" << std::endl;
	//}
}


void Player::PreDraw()
{
	m_Model->Update(m_Frame, m_Frame, m_BlendRate);
}

void Player::SetCamera(PlayerCamera* _camera)
{
	this->m_pCamera = _camera;
}

BULLET_MODE Player::GetBulletMode()
{
	return m_BulletMode;
}

