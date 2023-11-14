#pragma once

class Component
{
protected:

	class  GameObject* m_GameObject = nullptr;	// オーナー

public:
	Component() = delete;
	Component(GameObject* Object) { m_GameObject = Object; }
	virtual ~Component() {}		// 仮想デストラクタ

	virtual void Init() {};
	virtual void Uninit() {};
	virtual void Update() {};
	virtual void Draw() {};

};