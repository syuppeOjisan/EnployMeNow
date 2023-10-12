#pragma once

#include "Base/gameObject.h"
#include "Component/animationModel.h"

// 弾のモード
enum BULLET_MODE
{
	SIZE_UP,
	SIZE_DOWN,
	NONE,
};

class Player : public GameObject
{
	// TODO:SEを配列で連想配列で管理したい
private:
	DirectX::SimpleMath::Vector3		m_Velocity{};
	class	Audio*						m_SE{};
	class	AnimationModel*				m_Model;
	class	PlayerCamera*				m_pCamera;
	BULLET_MODE							m_bulletMode;	// 現在装備中の弾のモード
	float	m_BlendRate;
	int		m_Frame;
	bool	m_isLanding;	// 一度しかジャンプできないようにする


public:
	void Init() override;
	void Update() override;
	void PreDraw() override;

	void SetCamera(PlayerCamera* _camera);

	BULLET_MODE GetBulletMode();	// 今の弾のモードを取得
};