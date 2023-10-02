#pragma once
#include <SimpleMath.h>
using namespace DirectX::SimpleMath;

#include "Base/component.h"
#include "../GameObject/player.h"

class Object_SizeChange :
    public Component
{
private:
    bool            sizeChange;		// �T�C�Y�ύX�����ǂ���
    BULLET_MODE     bulletMode; 	// �������Ă����e�̃��[�h
    Vector3         changeSpeed;    // �T�C�Y���ς��X�s�[�h


    // �T�C�Y�ύX
    void SizeIncrease();	// �T�C�Y����
    void SizeDecrease();	// �T�C�Y�ቺ

public:
    using Component::Component;

    void Init() override;
    void Update() override;
};

