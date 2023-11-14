#pragma once

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
	/// マウス座標取得
	/// </summary>
	/// <param name="_x">マウスX座標</param>
	/// <param name="_y">マウスY座標</param>
	virtual void GetMousePosition(int& _x, int& _y) = 0;

	/// <summary>
	/// マウスの移動距離を取得
	/// </summary>
	/// <param name="_x">マウスX移動距離</param>
	/// <param name="_y">マウスY移動距離</param>
	virtual void GetMouseMove(int& _x, int& _y) = 0;

	/// <summary>
	/// コントローラースティック傾き取得
	/// </summary>
	/// <param name="_x">傾きX軸</param>
	/// <param name="_y">傾きY軸</param>
	virtual void GetPadStick(float& _x, float& _y) = 0;

};

// これらのメソッドは下位クラスに依存しており、SOLID原則的に良くない。けど仕方ないかな？