#pragma once

#include <SimpleMath.h>

/// <summary>
/// 入力情報管理用インターフェース
/// </summary>
class InputIntarface
{
public:
	virtual ~InputIntarface() {};	// 仮想デストラクタ

	virtual bool Init(void) = 0;
	virtual void Update(void) = 0;
	
	/// <summary>
	/// 押下状態取得
	/// </summary>
	/// <param name="_key">キーコード</param>
	/// <returns> 押下状態</returns>
	virtual bool GetPressed(int _key) = 0;

	/// <summary>
	/// 押下状態取得 - トリガー
	/// </summary>
	/// <param name="_key">キーコード</param>
	/// <returns>　押下状態</returns>
	virtual bool GetTregger(int _key) = 0;

	/// <summary>
	/// デバイスの移動度合いを取得
	/// </summary>
	/// <param name="_leftStickOrMousemove">左スティック傾き OR マウス移動距離</param>
	/// <param name="_rightStick">右スティック傾き</param>
	virtual void GetDeviceMovement(DirectX::SimpleMath::Vector2& _leftStickOrMousemove, DirectX::SimpleMath::Vector2& _rightStick) = 0;
};

// 下位クラスはこれらのメソッドすべてを使用するわけではないのでSOLID原則的に微妙