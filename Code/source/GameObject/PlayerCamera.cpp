#include <iostream>
#include "GameObject/PlayerCamera.h"
#include "../../header/System/input.h"

PlayerCamera::PlayerCamera(CharacterBase*_pTarget)
{
	// ターゲットを指定
	m_pTargetObject = _pTarget;

	// 注視点とカメラ位置を指定（この２つが同じ値だとバグる）
	m_Position = Vector3(0.0f, 10.0f, -50.0f);
	m_Target = Vector3(0.0f, 0.0f, 0.0f);

	Init();
}

void PlayerCamera::Init()
{
	// 前回フレームでのカメラ位置初期化
	m_LastCamPos = Vector3(0, 0, 0);

	// 注視点とカメラ位置を指定（この２つが同じ値だとバグる）
	m_Position = Vector3(0.0f, 10.0f, -50.0f);
	m_Target = Vector3(0.0f, 0.0f, 0.0f);

	// 視点移動マウス感度調整
	m_MouseSensitivity = 0.3f;
}

void PlayerCamera::Update()
{
	if (m_pTargetObject)
	{
		// カメラ位置を真後ろから20度回転させる
		//Matrix mxRotOffset = Matrix::CreateRotationY(DirectX::XMConvertToRadians(20.0f));
		//m_BackVec = Vector3::Transform(m_BackVec, mxRotOffset);

		// 上で作ったオフセット付きの後ろ向きベクトルにマウスの移動を加える
		// カメラをマウスで移動させる
		if (m_pInput)
		{
			int mouseX, mouseY;
			m_pInput->GetMouseMove(mouseX, mouseY);
			Matrix mxMouseRotationY = Matrix::CreateRotationY(DirectX::XMConvertToRadians(mouseX * m_MouseSensitivity));
			Matrix mxMouseRotationX = Matrix::CreateRotationX(DirectX::XMConvertToRadians(mouseY * m_MouseSensitivity));
			m_BackVec = Vector3::Transform(m_BackVec, mxMouseRotationY);
			m_BackVec = Vector3::Transform(m_BackVec, mxMouseRotationX);
			
		}


		// TODO:カメラにも当たり判定を付けたい
		// TODO:カメラから見た正面をプレイヤーの正面とする
		// プレイヤーを移動キー押したらカメラが向いている方向に回転させたい

		// 作成した後ろ向きベクトル上にカメラを設置
		Vector3 CamPos;
		Vector3 TargetPos = m_pTargetObject->GetPosition();
		float CamDistance = 13.0f; // キャラとカメラの距離
		CamPos.x = TargetPos.x + m_BackVec.x * CamDistance;
		CamPos.y = TargetPos.y + m_BackVec.y * CamDistance + 5.0f; // 少し上から見る
		CamPos.z = TargetPos.z + m_BackVec.z * CamDistance;

		// カメラ位置を設定
		this->SetPosition(CamPos);
		m_LastCamPos = CamPos;	// 位置を保存
		
		// カメラの注視点を設定
		Vector3 CamFocus;
		static bool shakePuls = true;
		CamFocus.x = TargetPos.x;
		CamFocus.y = TargetPos.y + shake;
		CamFocus.z = TargetPos.z;
		this->m_Target = CamFocus;

		// 歩きモーションのとき（いまはプレイヤーに依存したコードなので後で直す）
		if (shakeDO && m_pTargetObject->GetNextAnimationID() == 1)
		{
			if (shakePuls)
			{
				shake += shakeSpeed;
				if (shake > shakeWidth)
				{
					shakePuls = false;
				}
			}
			else if (!shakePuls)
			{
				shake -= shakeSpeed;
				if (shake < -shakeWidth)
				{
					shakePuls = true;
				}
			}
		}
		else
		{
			shake = 0.0f;
		}

	}
	else
	{
		// もしターゲットがnullならログに出す
		std::cout << "PlayerCamera.cpp - Update()" << std::endl;
		std::cout << "m_pTargetがnullです" << std::endl;
	}
}

void PlayerCamera::SetTarget(CharacterBase* _pTarget)
{
	// ターゲット指定
	m_pTargetObject = _pTarget;
	m_BackVec = -m_pTargetObject->GetForward();
}

Vector3 PlayerCamera::GetCameraFrontVec()
{
	Vector3 camPos = this->GetPosition();
	Vector3 playerPos = m_pTargetObject->GetPosition();

	Vector3 camFrontVec = playerPos - camPos;
	camFrontVec.Normalize();

	return camFrontVec;
}
