#pragma once
#include <SimpleMath.h>
using namespace DirectX::SimpleMath;

#include "Base/component.h"
#include "../GameObject/player.h"

class Object_ScaleX : public Component
{
private:
    bool            m_SizeChange;		// サイズ変更中かどうか
    BULLET_MODE     m_BulletMode; 	    // 当たってきた弾のモード
    float           m_ChangeSpeed;      // サイズが変わるスピード


    // サイズ変更
    void SizeIncrease();	// サイズ増加
    void SizeDecrease();	// サイズ低下

public:
    using Component::Component;

    void Init() override;
    void Update() override;
};

