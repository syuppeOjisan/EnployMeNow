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
	/// <param name="_x">スティックX軸</param>
	/// <param name="_y">スティックY軸</param>
	void GetPadStick(DirectX::SimpleMath::Vector2& _left, DirectX::SimpleMath::Vector2& _right) override;

	// 以下２つのメソッドはこのクラスでは使用しない
	void GetMousePosition(int& _x, int& _y) override {};
	void GetMouseMove(int& _x, int& _y) override {};

private:
	XINPUT_GAMEPAD m_NowPadState;	// コントローラー入力状態 - 最新
	XINPUT_GAMEPAD m_OldPadState;	// コントローラー入力状態 - １フレーム前

	// SHORT型の最大値
	const float SHORT_RANGE = 32768.0f;
};

