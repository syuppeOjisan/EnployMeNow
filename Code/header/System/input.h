#pragma once
#include <SimpleMath.h>

#pragma comment (lib, "xinput.lib")
#include <Xinput.h>

// 入力状態用の構造体
struct UNIVERSAL_INPUT_STATE
{
	bool Forward;
	bool BackWord;
	bool Right;
	bool Left;

	bool Jump;
	bool Punching;
};

// 入力状態を取得するときのID
enum UNIVERSAL_INPUT_ID
{
	FORWARD,
	BACKWORD,
	RIGHT,
	LEFT,

	JUMP,
	PUNCHING,
};

class Input
{
private:
	static const int KEYCOUNT = 256;
	static BYTE m_OldKeyState[KEYCOUNT];
	static BYTE m_KeyState[KEYCOUNT];


	static UNIVERSAL_INPUT_STATE m_OldInputState;	// １フレーム前の入力状態
	static UNIVERSAL_INPUT_STATE m_NowInputState;	// 現在の入力状態


	static POINT	m_WindowCenter;
	static POINT	m_NowMousePos;			// マウスポインタの位置 - 今
	static POINT	m_OldMousePos;			// マウスポインタの位置	- １フレーム前
	static bool		m_isMouseMove;			// マウスが移動しているかどうか

	static bool m_CursorTrap;				// マウスカーソルをウィンドウ内に閉じ込めるかどうか
	
	static HWND m_WindowInfo;				// ウィンドウ情報


public:
	static void Init(HWND hWnd);
	static void Uninit();
	static void Update();

	static void UpdateUnivInput(void);
	
	// キーの押下状態を返す
	static bool GetKeyPress( BYTE KeyCode );	
	// キーの押下状態（トリガー）を返す
	static bool GetKeyTrigger( BYTE KeyCode );
	// キーが離されたかどうかを返す
	static bool GetKeyReleased( BYTE KeyCode );

	// ユニバーサル入力状態を取得 - 押下
	static bool GetUniversulPress(UNIVERSAL_INPUT_ID _inputID);
	// ユニバーサル入力状態を取得 - トリガー
	static bool GetUniversulTrigger(UNIVERSAL_INPUT_ID _inputID);

	static POINT GetNowMoucePos();						// マウスの現在位置を返す - スクリーン座標
	static POINT GetNowMoucePosClient();				// マウスの現在位置を返す - クライアント座標
	static DirectX::SimpleMath::Vector2 GetMouseMove();	// マウスの移動度を返す
	static void SetCursorTrap(bool trap);
	static void SetisMouceMove(bool _isMove) { m_isMouseMove = _isMove; } //　マウスが移動しているかどうかを格納
	static bool GetisMouceMove(void) { return m_isMouseMove; }


};
