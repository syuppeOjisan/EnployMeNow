#pragma once
#include <SimpleMath.h>
using namespace DirectX::SimpleMath;

#include "Base/component.h"
#include "../GameObject/player.h"

class Object_ScaleX : public Component
{
private:
    bool            m_SizeChange;		// �T�C�Y�ύX�����ǂ���
    BULLET_MODE     m_BulletMode; 	    // �������Ă����e�̃��[�h
    float           m_ChangeSpeed;      // �T�C�Y���ς��X�s�[�h


    // �T�C�Y�ύX
    void SizeIncrease();	// �T�C�Y����
    void SizeDecrease();	// �T�C�Y�ቺ

public:
    using Component::Component;

    void Init() override;
    void Update() override;
};

