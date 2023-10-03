#pragma once

// TODO:プレイヤーカメラの位置と注視点がどちらも同じ値になるのでバグって実行できないから治す

#include "camera.h"
#include <SimpleMath.h>
using namespace DirectX::SimpleMath;

class PlayerCamera : public Camera
{
public:
	PlayerCamera(GameObject *_pTarget);
	PlayerCamera() {};
	~PlayerCamera(void) {};
	
	void Init()override;
	void Update()override;

	// ターゲット設定
	void SetTarget(GameObject* _pTarget);

private:
	// 追従するゲームオブジェクト
	GameObject* m_pTarget;

	// 前回フレームでのカメラ位置
	Vector3 m_LastCamPos;
};