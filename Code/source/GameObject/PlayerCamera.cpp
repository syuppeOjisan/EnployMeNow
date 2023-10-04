#include <iostream>
#include "GameObject/PlayerCamera.h"
#include "../../header/System/input.h"

PlayerCamera::PlayerCamera(GameObject *_pTarget)
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
}

void PlayerCamera::Update()
{
	if (m_pTargetObject)
	{
		// 操作キャラの前向きベクトルを取得
		Vector3 forwordVec = m_pTargetObject->GetForward();
		// 前向きベクトルを反転させ、後ろ向きベクトルにする
		Vector3 backVec;
		backVec.x = -forwordVec.x;
		backVec.y = -forwordVec.y;
		backVec.z = -forwordVec.z;

		// カメラ位置を真後ろから20度回転させる
		Matrix mxRotOffset = Matrix::CreateRotationY(DirectX::XMConvertToRadians(20.0f));
		backVec = Vector3::Transform(backVec, mxRotOffset);

		// TODO:なんかいい感じにカメラが動かないから治す
		// カメラを移動したあとに勝手に初期位置に戻る
		// カメラが回転する中心が固定されているからおかしい（プレイヤーの少し前を注視点にせず、プレイヤーそのものを注視点にすれば解決するけどどうにかしたい）
		// 
		// 上で作ったオフセット付きの後ろ向きベクトルにマウスの移動を加える
		// カメラをマウスで移動させる
		Matrix mxMouseRotationY = Matrix::CreateRotationY(DirectX::XMConvertToRadians(Input::GetMouseMove().x));
		Matrix mxMouseRotationZ = Matrix::CreateRotationY(DirectX::XMConvertToRadians(Input::GetMouseMove().y));
		backVec = Vector3::Transform(backVec, mxMouseRotationY);
		backVec = Vector3::Transform(backVec, mxMouseRotationZ);

		// 作成した後ろ向きベクトル上にカメラを設置
		Vector3 CamPos;
		Vector3 TargetPos = m_pTargetObject->GetPosition();
		float CamDistance = 10.0f; // キャラとカメラの距離
		CamPos.x = TargetPos.x + backVec.x * CamDistance;
		CamPos.y = TargetPos.y + backVec.y * CamDistance + 5.0f;
		CamPos.z = TargetPos.z + backVec.z * CamDistance;

		// 緩やかカメラ処理
		//float blendFactor = 0.97f; // 平均化の重み
		//CamPos.x = m_LastCamPos.x * blendFactor + CamPos.x * (1.0f - blendFactor);
		//CamPos.y = m_LastCamPos.y * blendFactor + CamPos.y * (1.0f - blendFactor);
		//CamPos.z = m_LastCamPos.z * blendFactor + CamPos.z * (1.0f - blendFactor);

		// カメラ位置を設定
		this->SetPosition(CamPos);
		m_LastCamPos = CamPos;	// 位置を保存
		
		// カメラの注視点を設定
		Vector3 CamFocus;
		CamFocus.x = TargetPos.x + forwordVec.x;
		CamFocus.y = TargetPos.y + forwordVec.y;
		CamFocus.z = TargetPos.z + forwordVec.z;
		this->m_Target = CamFocus;
	}
	else
	{
		// もしターゲットがnullならログに出す
		std::cout << "PlayerCamera.cpp - Update()" << std::endl;
		std::cout << "m_pTargetがnullです" << std::endl;
	}
}

void PlayerCamera::SetTarget(GameObject* _pTarget)
{
	// ターゲット指定
	m_pTargetObject = _pTarget;
}
