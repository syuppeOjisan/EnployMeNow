#pragma once
#include <SimpleMath.h>
using namespace DirectX::SimpleMath;

#include "Base/component.h"
#include "../GameObject/player.h"

class Object_SizeChange :
    public Component
{
private:
    bool            sizeChange;		// サイズ変更中かどうか
    BULLET_MODE     bulletMode; 	// 当たってきた弾のモード
    Vector3         changeSpeed;    // サイズが変わるスピード


    // サイズ変更
    void SizeIncrease();	// サイズ増加
    void SizeDecrease();	// サイズ低下

public:
    using Component::Component;

    void Init() override;
    void Update() override;
};

