#pragma once
#include <SimpleMath.h>

#pragma comment (lib, "xinput.lib")
#include <Xinput.h>

// �R���g���[���[�ƃL�[�{�[�h�̓��͏�����ɂ܂Ƃ߂�����
enum UNIVERSUL_INPUT_CODE
{
	FORWARD,
	BACKWARD,
	RIGHT,
	LEFT,

	JUMP,
	BULLETMODE_01,
	BULLETMODE_02,
};

class Input
{
private:
	static const int KEYCOUNT = 256;
	static BYTE m_OldKeyState[KEYCOUNT];
	static BYTE m_KeyState[KEYCOUNT];

	static POINT	m_NowMousePos;			// �}�E�X�|�C���^�̈ʒu - ��
	static POINT	m_OldMousePos;			// �}�E�X�|�C���^�̈ʒu	- �P�t���[���O
	static bool		m_isMouseMove;			// �}�E�X���ړ����Ă��邩�ǂ���

	static bool m_CursorTrap;				// �}�E�X�J�[�\�����E�B���h�E���ɕ����߂邩�ǂ���
	
	static HWND m_WindowInfo;				// �E�B���h�E���


public:
	static void Init(HWND hWnd);
	static void Uninit();
	static void Update();
	
	// �L�[�̉�����Ԃ�Ԃ�
	static bool GetKeyPress( BYTE KeyCode );	
	// �L�[�̉�����ԁi�g���K�[�j��Ԃ�
	static bool GetKeyTrigger( BYTE KeyCode );
	// �L�[�������ꂽ���ǂ�����Ԃ�
	static bool GetKeyReleased( BYTE KeyCode );

	static POINT GetNowMoucePos();						// �}�E�X�̌��݈ʒu��Ԃ� - �X�N���[�����W
	static POINT GetNowMoucePosClient();				// �}�E�X�̌��݈ʒu��Ԃ� - �N���C�A���g���W
	static DirectX::SimpleMath::Vector2 GetMouseMove();	// �}�E�X�̈ړ��x��Ԃ�
	static void SetCursorTrap(bool trap);
	static void SetisMouceMove(bool _isMove) { m_isMouseMove = _isMove; } //�@�}�E�X���ړ����Ă��邩�ǂ������i�[
	static bool GetisMouceMove(void) { return m_isMouseMove; }


};
