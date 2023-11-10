#pragma once

#include "gameObject.h"
#include "System/collision.h"

#include "Component/animationModel.h"

using namespace DirectX::SimpleMath;

// キャラクター（敵味方関係なし）の基底クラスです
class CharacterBase : public GameObject
{
public:
	virtual void Init() override { m_pModel = nullptr; m_Time = 1.0f; }
	virtual void Uninit() override {}
	virtual void Update() override;
	virtual void Draw() override {}
	virtual void PreDraw() override {}

	// ベロシティをセット
	void SetVelocity(Vector3 _velocity) { m_Velocity = _velocity; }
	void SetVelocity(float _x, float _y, float _z) { m_Velocity.x = _x; m_Velocity.y = _y; m_Velocity.z = _z;}

	Vector3 GetVelocity(void) { return m_Velocity; }

	// 当たり判定情報を取得(AABB)
	AABB GetAABBCollision() { return m_AABBCollision; }
	// 当たり判定情報を取得(球)
	BoundingSphere GetSphereCollision() { return m_SphereCollision; }

	// 当たり状態をセット
	void SetIsHit(bool _isHit) { m_IsHit = _isHit; }
	// 当たり判定状態をゲット
	bool* GetIsHit(void) { return &m_IsHit; }

	// モデル情報をゲット
	AnimationModel* GetModel(void) { return m_pModel; }

	// 時間管理変数をセット
	void SetTime(float _time) { m_Time = _time; }
	// 時間管理変数をゲット
	float GetTime(void) { return m_Time; }

	// モーション変更 - モデルデータ指定なし
	bool SetNextAnimation(const char* _nextAnimation);
	// モーション変更　- モデルデータ指定あり
	bool SetNextAnimation(AnimationModel* _pModel, const char* _nextAnimation);

	/// <summary>
	/// ヒットストップの計算を行います
	/// </summary>
	/// <param name="_startFrame"> ヒットストップを開始するフレーム</param>
	/// <param name="_endFrame"> ヒットストップを終了するフレーム</param>
	/// <param name="difference"> ヒットストップ中の速度差</param>
	void CalcHitStop(float _startFrame, float _endFrame, float difference);

	// 現在のモーション取得
	const char* GetNowAnimName(void) { return m_NowAnimation; }
	// 前回のモーションを取得
	const char* GetPrevAnimName(void) { return m_PrevAnimation; }

	// 現在のモーションフレームを取得
	int GetNowAnimationFrame(void) { return m_NowAnimationFrame; }
	// 前回のモーションフレームを取得
	int GetPrevAnimationFrame(void) { return m_PrevAnimationFrame; }

	// アニメーションブレンドが終わったかどうかを取得
	bool* GetIsAnimBlendOver(void) { return &m_IsAnimBlendOver; }

protected:
	Vector3				m_Velocity{};				// ベロシティ

	AnimationModel*		m_pModel;					// モデル情報
	float				m_Time;						// 時間管理変数
	float				m_BlendRate;				// モーションブレンドレート
	float				m_NowAnimationFrame = 0;	// 再生中のアニメーションフレーム
	float				m_PrevAnimationFrame = 0;	// 前再生していたアニメーションフレーム
	const char*			m_NowAnimation;				// 今のモーション名
	const char*			m_PrevAnimation;			// 前回のモーション名

	bool				m_IsHit;					// 当たり状態
	bool				m_IsAnimBlendOver;		// アニメーションブレンドが終わったかどうか


	// 当たり判定情報
	AABB			m_AABBCollision{};		// AABBコリジョン情報
	BoundingSphere	m_SphereCollision{};	// 丸コリジョン情報
};