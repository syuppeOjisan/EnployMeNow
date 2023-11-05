#pragma once
#include "Base/gameObject.h"

#include "Component/animationModel.h"

class grandpa : public GameObject
{
public:
	void Init() override;
	void Update() override;
	void PreDraw() override;

private:
	class	AnimationModel* m_Model;	// モデルデータ

	float	m_BlendRate;	// モーションブレンドレート
	int		m_Frame;		// フレーム
};

