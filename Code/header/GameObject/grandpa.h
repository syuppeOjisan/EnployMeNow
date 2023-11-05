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
	class	AnimationModel* m_Model;	// ���f���f�[�^

	float	m_BlendRate;	// ���[�V�����u�����h���[�g
	int		m_Frame;		// �t���[��
};

