#pragma once

#include "Base/CharacterBase.h"

#include "System/KeyboardInput.h"
#include "GameObject/PlayerCamera.h"
#include "Component/animationModel.h"
#include "Component/audio.h"

// 撃つ弾のモード
enum BULLET_MODE
{
	SIZE_UP,
	SIZE_DOWN,
};

// 撃つ弾の種類
enum BULLET_KIND
{
	CHANGE_X_AXSIS,
	CHANGE_Y_AXSIS,
	CHANGE_Z_AXSIS,
	NONE,
};

class Player : public CharacterBase
{
public:
	void Init() override;
	void Uninit() override;
	void Update() override;
	void PreDraw() override;

	/// <summary>
	/// プレイヤーに入力インターフェースをセット
	/// </summary>
	/// <param name="_input">入力インターフェース</param>
	void SetInput(InputIntarface* _input) { m_pInput = _input; m_pInput->Init(); }

	void SetCamera(PlayerCamera* _camera);	// プレイヤーにカメラ情報を登録

	///// <summary>
	///// プレイヤーの向きをベクトルの方に向ける
	///// </summary>
	///// <param name="_vector">向いてほしい方向ベクトル</param>
	//void SetRotateToVector(Vector3 _vector);

private:
	class	PlayerCamera* m_pCamera;	// プレイヤー用カメラ
	bool	m_isLanding;				// 一度しかジャンプできないようにする

	// モーションID
	enum
	{
		ANIMATION_ID_IDLE = 0,
		ANIMATION_ID_WALK,
		ANIMATION_ID_WALKBACK,
		ANIMATION_ID_RUN,
		ANIMATION_ID_PUNCHING,
		ANIMATION_ID_PUNCHI_CHARGE,
		ANIMATION_ID_JUMP,
	};
};