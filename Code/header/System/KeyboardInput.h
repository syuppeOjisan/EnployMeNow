#pragma once
#include <Windows.h>
#include "InputIntarface.h"

/// <summary>
/// キーボード入力管理クラス
/// </summary>
class KeyboardInput : public InputIntarface
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
	/// 移動キー入力値
	/// </summary>
	/// <param name="_leftStick">WASDキー入力値</param>
	/// <param name="_rightStick">矢印キー入力値</param>
	void GetDeviceMovement(DirectX::SimpleMath::Vector2& _wasdMove, DirectX::SimpleMath::Vector2& _arrowMove) override;

private:
	// キーの数を定義
	static const int KEY_COUNT = 256;

	bool m_nowKeyState[KEY_COUNT];	// 現在のキー入力状態
	bool m_oldKeyState[KEY_COUNT];	// １フレーム前の入力状態
};

