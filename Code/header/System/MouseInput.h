#pragma once
#include <Windows.h>
#include "InputIntarface.h"

/// <summary>
/// マウス入力管理クラス
/// </summary>
class MouseInput : public InputIntarface
{
public:
	bool Init(void) override;
	void Update(void) override;

	/// <summary>
	/// 押下状態取得
	/// </summary>
	/// <param name="_key">キーコード</param>
	/// <returns> 押下状態</returns>
	bool GetPressed(int _key) override;

	/// <summary>
	/// 押下状態取得 - トリガー
	/// </summary>
	/// <param name="_key">キーコード</param>
	/// <returns>　押下状態</returns>
	bool GetTregger(int _key) override;

	/// <summary>
	/// マウス座標取得
	/// </summary>
	/// <param name="_x">マウスX座標</param>
	/// <param name="_y">マウスY座標</param>
	void GetMousePosition(int& _x, int& _y) override;

	/// <summary>
	/// マウスの移動距離を取得
	/// </summary>
	/// <param name="_x">マウスX移動距離</param>
	/// <param name="_y">マウスY移動距離</param>
	void GetMouseMove(int& _x, int& _y) override;

	// このメソッドはこのクラスでは使用しない
	void GetPadStick(DirectX::SimpleMath::Vector2& _left, DirectX::SimpleMath::Vector2& _right) override {};

private:
	POINT m_nowMousePosition;	// マウス位置 - 最新
	POINT m_oldMousePosition;	// マウス位置 - １フレーム前
};