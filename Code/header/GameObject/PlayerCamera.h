#pragma once

#include "camera.h"
#include "GameObject/Base/CharacterBase.h"
#include <SimpleMath.h>
using namespace DirectX::SimpleMath;

class PlayerCamera : public Camera
{
public:
	//-----------------------
	// 初期化
	//-----------------------
	// コンストラクタ
	PlayerCamera(CharacterBase*_pTarget);
	PlayerCamera() {};

	// デストラクタ
	~PlayerCamera(void) {m_Position = Vector3(0.0f, 10.0f, -50.0f); m_Target = Vector3(0.0f, 0.0f, 0.0f);};
	
	void Init()override;	// 初期化処理
	void Update()override;	// 更新処理

	//-----------------------
	// アクセサ
	//-----------------------

	// ターゲット設定
	void SetTarget(CharacterBase* _pTarget);

	// カメラの前向き単位ベクトルを取得
	Vector3 GetCameraFrontVec();

	float getNowSensi(void) { return m_ViewControlSensitivity; }
	void SetSensitivity(float _sensi) { m_ViewControlSensitivity = _sensi; }

	float* GetShake(void) { return &shake; }
	float* GetShakeWidth(void) { return &shakeWidth; }
	float* GetShakeSpeed(void) { return &shakeSpeed; }
	bool* GetShakeDO(void) { return &shakeDO; }

	bool* GetCamMove(void) { return &camMove; }

	//-----------------------
	// メンバ変数
	//-----------------------
private:
	// 追従するキャラクタ
	CharacterBase* m_pTargetObject;

	// プレイヤーの後ろ向きベクトル
	Vector3 m_BackVec;
	// 前回フレームでのカメラ位置
	Vector3 m_LastCamPos;
	// 視点操作感度
	float m_ViewControlSensitivity;

	float shake = 0.0f;
	float shakeWidth = 0.2f;
	float shakeSpeed = 0.1f;
	bool shakeDO = false;

	bool camMove = true;
};