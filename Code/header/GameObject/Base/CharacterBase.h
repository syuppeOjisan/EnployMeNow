#pragma once

#include "gameObject.h"
#include "System/collision.h"

#include "Component/animationModel.h"

using namespace DirectX::SimpleMath;

// キャラクター（敵味方関係なし）の基底クラスです
class CharacterBase : public GameObject
{
public:
	virtual void Init() { m_Model = nullptr; }
	virtual void Uninit() {}
	virtual void Update() {}
	virtual void Draw() {}
	virtual void PreDraw() {}

	// 当たり判定情報を取得(AABB)
	AABB GetAABBCollision() { return m_AABBCollision; }
	// 当たり判定情報を取得(球)
	BoundingSphere GetSphereCollision() { return m_SphereCollision; }

	// 当たり状態をセット
	void SetIsHit(bool _isHit) { m_IsHit = _isHit; }
	// 当たり判定状態をゲット
	bool* GetIsHit(void) { return &m_IsHit; }
	// モデル情報をゲット
	AnimationModel* GetModel(void) { return m_Model; }

protected:
	Vector3				m_Velocity{};	// ベロシティ
	AnimationModel*		m_Model;		// モデル情報
	float				m_BlendRate;	// モーションブレンドレート
	int					m_Frame;		// フレーム
	bool				m_IsHit;		// 当たり状態

	// 当たり判定情報
	AABB			m_AABBCollision{};		// AABBコリジョン情報
	BoundingSphere	m_SphereCollision{};	// 丸コリジョン情報
};