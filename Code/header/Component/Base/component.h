#pragma once

class Component
{
protected:

	class  GameObject* m_GameObject = nullptr;	// �I�[�i�[

public:
	Component() = delete;
	Component(GameObject* Object) { m_GameObject = Object; }
	virtual ~Component() {}		// ���z�f�X�g���N�^

	virtual void Init() {};
	virtual void Uninit() {};
	virtual void Update() {};
	virtual void Draw() {};

};