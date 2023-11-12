#pragma once
#include <SimpleMath.h>

#pragma comment (lib, "xinput.lib")
#include <Xinput.h>

// ���͏�ԗp�̍\����
struct UNIVERSAL_INPUT_STATE
{
	bool Forward;
	bool BackWord;
	bool Right;
	bool Left;

	bool Jump;
	bool Punching;
};

// ���͏�Ԃ��擾����Ƃ���ID
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


	static UNIVERSAL_INPUT_STATE m_OldInputState;	// �P�t���[���O�̓��͏��
	static UNIVERSAL_INPUT_STATE m_NowInputState;	// ���݂̓��͏��


	static POINT	m_WindowCenter;
	static POINT	m_NowMousePos;			// �}�E�X�|�C���^�̈ʒu - ��
	static POINT	m_OldMousePos;			// �}�E�X�|�C���^�̈ʒu	- �P�t���[���O
	static bool		m_isMouseMove;			// �}�E�X���ړ����Ă��邩�ǂ���

	static bool m_CursorTrap;				// �}�E�X�J�[�\�����E�B���h�E���ɕ����߂邩�ǂ���
	
	static HWND m_WindowInfo;				// �E�B���h�E���


public:
	static void Init(HWND hWnd);
	static void Uninit();
	static void Update();

	static void UpdateUnivInput(void);
	
	// �L�[�̉�����Ԃ�Ԃ�
	static bool GetKeyPress( BYTE KeyCode );	
	// �L�[�̉�����ԁi�g���K�[�j��Ԃ�
	static bool GetKeyTrigger( BYTE KeyCode );
	// �L�[�������ꂽ���ǂ�����Ԃ�
	static bool GetKeyReleased( BYTE KeyCode );

	// ���j�o�[�T�����͏�Ԃ��擾 - ����
	static bool GetUniversulPress(UNIVERSAL_INPUT_ID _inputID);
	// ���j�o�[�T�����͏�Ԃ��擾 - �g���K�[
	static bool GetUniversulTrigger(UNIVERSAL_INPUT_ID _inputID);

	static POINT GetNowMoucePos();						// �}�E�X�̌��݈ʒu��Ԃ� - �X�N���[�����W
	static POINT GetNowMoucePosClient();				// �}�E�X�̌��݈ʒu��Ԃ� - �N���C�A���g���W
	static DirectX::SimpleMath::Vector2 GetMouseMove();	// �}�E�X�̈ړ��x��Ԃ�
	static void SetCursorTrap(bool trap);
	static void SetisMouceMove(bool _isMove) { m_isMouseMove = _isMove; } //�@�}�E�X���ړ����Ă��邩�ǂ������i�[
	static bool GetisMouceMove(void) { return m_isMouseMove; }


};
