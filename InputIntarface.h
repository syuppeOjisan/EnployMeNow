#pragma once


/// <summary>
/// 入力情報管理用インターフェース
/// </summary>
class InputIntarface
{
public:
	virtual bool Init(void) = 0;
	virtual void Update(void) = 0;
	
	virtual bool GetKeyPressed(int _key) = 0;
	virtual bool GetKeyTregger(int _key) = 0;
};