#pragma once

#include "Base/CharacterBase.h"

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
	void Update() override;
	void PreDraw() override;

	void SetCamera(PlayerCamera* _camera);	// プレイヤーにカメラ情報を登録

private:
	class	PlayerCamera* m_pCamera;


	BULLET_MODE									m_BulletMode;	// 現在装備中の弾のモード
	BULLET_KIND									m_BulletEquip;	// 装備中の弾
	float	m_BlendRate;
	int		m_Frame;
	char	m_NextAnim;		// 次に遷移するアニメーション
	bool	m_isLanding;	// 一度しかジャンプできないようにする
};