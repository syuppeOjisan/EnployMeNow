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

#include "imgui.h"

#include <iostream>

using namespace DirectX::SimpleMath;

void Player::Init()
{
	CharacterBase::Init();

	// シェーダーの読み込み
	AddComponent<Shader>()->Load("shader\\vertexLightingOneSkinVS.cso", "shader\\vertexLightingPS.cso");
	m_pModel = AddComponent<AnimationModel>();
	
	// モーション読み込み
	m_pModel->Load("asset\\model\\Player_Tpose.fbx");
	//m_pModel->LoadAnimation("asset\\model\\Player_Idle.fbx", "Idle");
	//m_pModel->LoadAnimation("asset\\model\\Player_Walk.fbx", "Walk");
	//m_pModel->LoadAnimation("asset\\model\\Player_Punching.fbx", "Punching");

	m_pModel->LoadAnimation("asset\\model\\Player_Idle.fbx", ANIMATION_ID_IDLE);
	m_pModel->LoadAnimation("asset\\model\\Player_Walk.fbx", ANIMATION_ID_WALK);
	m_pModel->LoadAnimation("asset\\model\\Player_WalkBack.fbx", ANIMATION_ID_WALKBACK);
	m_pModel->LoadAnimation("asset\\model\\Player_Run.fbx", ANIMATION_ID_RUN);
	m_pModel->LoadAnimation("asset\\model\\Player_Punching.fbx", ANIMATION_ID_PUNCHING);
	m_pModel->LoadAnimation("asset\\model\\Player_Jump.fbx", ANIMATION_ID_JUMP);

	AddComponent<Shadow>()->SetSize(1.5f);

	// サイズ調整
	m_Scale = Vector3(0.02f, 0.02f, 0.02f);

	// 当たり判定設定
	this->m_AABBCollision = SetAABB(this->m_Position, this->m_Scale.x, this->m_Scale.y, this->m_Scale.z);

	// モーション変数初期化
	m_NowAnimation = "Idle";
	m_PrevAnimation = "Idle";

	m_NowAnimationSpeed = 1;
	m_PrevAnimationSpeed = 1;

	m_NowAnimationID = ANIMATION_ID_IDLE;
	m_PrevAnimationID = ANIMATION_ID_IDLE;
}

void Player::Uninit()
{
	delete m_pInput;
}

void Player::Update()
{
	Scene* nowscene = Manager::GetScene();
	Camera* cameraobj = nowscene->GetGameObject<PlayerCamera>();

	// インターフェースを更新して使用
	if (m_pInput)
	{
		// スティックもしくはコントローラーでキャラクター移動
		
		// スティックの情報を取得 - コントローラーの場合
		// キーボード移動キーの取得 - キーボードの場合
		m_pInput->Update();
		Vector2 leftStick = {};
		Vector2 rightStick = {};
		m_pInput->GetDeviceMovement(leftStick, rightStick);

		// カメラの前向きベクトル
		Vector3 CamForward = m_pCamera->GetCameraFrontVec();

		// Z軸の移動成分を作成
		Vector3 forward{};
		forward = CamForward;
		forward.x *= leftStick.y;
		forward.z *= leftStick.y;

		// X軸の移動成分を作成
		Vector3 right{};
		right.z = -CamForward.x;
		right.x = CamForward.z;
		right.x *= leftStick.x;
		right.z *= leftStick.x;

		// 作った2つの軸の移動成分を合成
		forward += right;
		// Y成分を削除
		forward.y = 0;

		// 完成した移動成分を適応
		SetVelocity(forward);


		// 常に進んでいる方向を向く
		Vector3 forwardVecFromFunc = GetPosition() - GetOldPosition();	// 移動方向ベクトルを計算
		forwardVecFromFunc.Normalize();	// 正規化しておく

		// コントローラーの入力によって処理を変える

		// スティックの倒され度合いに応じてモーションを変更
		if (abs(forward.x) > 0.2f || abs(forward.z) > 0.2f)
		{
			SetNextAnimation(ANIMATION_ID_RUN);	// モーションを変更
			SetRotateToVectorEaseIn(forwardVecFromFunc); // 進んでいる方向を向くように変更
		}
		else if (abs(forward.x) < 0.2f && abs(forward.x) != 0.0f || abs(forward.z) < 0.2f && abs(forward.z) != 0.0f)
		{
			SetNextAnimation(ANIMATION_ID_WALK);
			SetRotateToVectorEaseIn(forwardVecFromFunc);
		}
		else if (m_pInput->GetTregger(XINPUT_GAMEPAD_A))
		{
			SetNextAnimation(ANIMATION_ID_PUNCHING);

			XINPUT_VIBRATION vib{};
			vib.wLeftMotorSpeed = 65535;
			vib.wRightMotorSpeed = 65535;
			XInputSetState(0, &vib);
		}
		else if(m_pInput->GetPressed(XINPUT_GAMEPAD_B))
		{
			SetNextAnimation(ANIMATION_ID_JUMP);

			XINPUT_VIBRATION vib{};
			vib.wLeftMotorSpeed = 0;
			vib.wRightMotorSpeed = 0;
			XInputSetState(0, &vib);
		}
		else
		{
			SetNextAnimation(ANIMATION_ID_IDLE);
		}			
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

	Vector3 oldPosition = GetOldPosition();

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

	CharacterBase::Update();
}


void Player::PreDraw()
{
	m_pModel->Update(m_PrevAnimationID, m_PrevAnimationFrame, m_NowAnimationID, m_NowAnimationFrame, m_BlendRate);
}

void Player::SetCamera(PlayerCamera* _camera)
{
	this->m_pCamera = _camera;
}

//void Player::SetRotateToVector(Vector3 _vector)
//{
//	// ベクトルが向いている角度を求める
//	float rotateRadianDestination = atan2f(_vector.x, _vector.z);
//	rotateRadianDestination = FixRadian(rotateRadianDestination);	// ラジアン補正
//
//	// 現在の回転角との角度差を求める
//	float rotateRadian = rotateRadianDestination - GetRotation().y;
//	rotateRadian = FixRadian(rotateRadian);	// ラジアン補正
//
//	// 角度差分回す
//	Vector3 pRotate = GetRotation();
//	pRotate.y += rotateRadian;
//	pRotate.y = FixRadian(pRotate.y);
//
//	// 回転角適用
//	SetRotation(Vector3(0, pRotate.y, 0));
//}
