#pragma once

#include "Base/gameObject.h"

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

class Player : public GameObject
{
	// TODO:SEを連想配列で管理したい
private:
	DirectX::SimpleMath::Vector3				m_Velocity{};
	class	AnimationModel*						m_Model;
	class	PlayerCamera*						m_pCamera;
	BULLET_MODE									m_BulletMode;	// 現在装備中の弾のモード
	BULLET_KIND									m_BulletEquip;	// 装備中の弾
	std::unordered_map<std::string, Audio*>		m_SEList;		// SEのリスト
	float	m_BlendRate;
	int		m_Frame;
	bool	m_isLanding;	// 一度しかジャンプできないようにする
	bool	m_isSEplay_X;
	bool	m_isSEplay_Y;
	bool	m_isSEplay_Z;


public:
	void Init() override;
	void Update() override;
	void PreDraw() override;

	void SetCamera(PlayerCamera* _camera);

	BULLET_MODE GetBulletMode();	// 今の弾のモードを取得
	BULLET_KIND GetBulletKind() { return m_BulletEquip; }
};