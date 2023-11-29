#pragma once
#include <Windows.h>
#include <Xinput.h>
#pragma comment (lib, "xinput.lib")

#include "InputIntarface.h"

/// <summary>
/// ゲームパッド入力管理クラス
/// </summary>
class PadInput : public InputIntarface
{
public:
	bool Init(void) override;
	void Update(void) override;

	/// <summary>
	/// 押下状態取得
	/// </summary>
	/// <param name="_key">キーコード</param>
	/// <returns>押下状態</returns>
	bool GetPressed(int _key) override;

	/// <summary>
	/// 押下状態取得 - トリガー
	/// </summary>
	/// <param name="_key">キーコード</param>
	/// <returns>押下状態</returns>
	bool GetTregger(int _key) override;

	/// <summary>
	/// スティックの倒され度
	/// </summary>
	/// <param name="_leftStick">スティックX軸</param>
	/// <param name="_rightStick">スティックY軸</param>
	void GetDeviceMovement(DirectX::SimpleMath::Vector2& _leftStick, DirectX::SimpleMath::Vector2& _rightStick) override;

private:
	XINPUT_GAMEPAD	m_NowPadState;		// コントローラー入力状態 - 最新
	XINPUT_GAMEPAD	m_OldPadState;		// コントローラー入力状態 - １フレーム前

	bool			m_isVib;			// バイブレーション中かどうか
	int				m_VibCount;			// バイブレーションの経過時間
	int				m_VibCountLimit;	// バイブレーション時間

	// SHORT型の最大値
	const float SHORT_RANGE = 32768.0f;
};

