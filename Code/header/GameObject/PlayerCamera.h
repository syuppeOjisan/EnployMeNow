#pragma once

#include "camera.h"
#include <SimpleMath.h>
using namespace DirectX::SimpleMath;

class PlayerCamera : public Camera
{
public:
	//-----------------------
	// 初期化
	//-----------------------
	// コンストラクタ
	PlayerCamera(GameObject *_pTarget);	
	PlayerCamera() {};

	// デストラクタ
	~PlayerCamera(void) {m_Position = Vector3(0.0f, 10.0f, -50.0f); m_Target = Vector3(0.0f, 0.0f, 0.0f);};
	
	void Init()override;	// 初期化処理
	void Update()override;	// 更新処理

	//-----------------------
	// アクセサ
	//-----------------------

	// ターゲット設定
	void SetTarget(GameObject* _pTarget);

	// カメラの前向き単位ベクトルを取得
	Vector3 GetCameraFrontVec();

	//-----------------------
	// メンバ変数
	//-----------------------
private:
	// 追従するゲームオブジェクト
	GameObject* m_pTargetObject;

	// プレイヤーの後ろ向きベクトル
	Vector3 m_BackVec;
	// 前回フレームでのカメラ位置
	Vector3 m_LastCamPos;
	// マウス感度
	float m_MouseSensitivity;
};