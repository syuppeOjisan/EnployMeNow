#pragma once

#include "Base/gameObject.h"

enum BULLET_MODE;
enum BULLET_KIND;

class Bullet : public GameObject
{

private:

	DirectX::SimpleMath::Vector3		m_Velocity{};
	BULLET_MODE bulletMode;
	BULLET_KIND bulletEquip;

public:
	static void Load();
	static void Unload();

	void Init() override;
	void Update() override;

	void SetVelocity(DirectX::SimpleMath::Vector3 Velocity) { m_Velocity = Velocity; }

	BULLET_MODE GetBulletMode();
	void SetBulletMode(BULLET_MODE mode);

	BULLET_KIND GetBulletKind() { return bulletEquip; }
	void SetBulletKind(BULLET_KIND _bulletEquip) { bulletEquip = _bulletEquip; }

};