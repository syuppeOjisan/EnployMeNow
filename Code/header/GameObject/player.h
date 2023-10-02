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

private:

	DirectX::SimpleMath::Vector3		m_Velocity{};
	class	Audio*						m_SE{};
	class	AnimationModel*				m_Model;
	BULLET_MODE							m_bulletMode;	// 現在装備中の弾のモード
	float	m_BlendRate;
	int		m_Frame;
	bool	m_isLanding;	// 一度しかジャンプできないようにする


public:
	void Init() override;
	void Update() override;
	void PreDraw() override;

	BULLET_MODE GetBulletMode();	// 今の弾のモードを取得
};