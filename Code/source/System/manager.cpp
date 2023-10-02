#include	"System/main.h"
#include	"System/renderer.h"
#include	"System/manager.h"
#include	"System/input.h"
#include	"Scene/Base/scene.h"
#include	"Scene/game.h"
#include	"Scene/title.h"
#include	"Component/audio.h"

// �V�[���}�l�[�W���������o�ϐ��̎���
Scene* Manager::m_Scene{};					// ���݃V�[��

void Manager::Init(Application* ap, HWND hWnd)
{
	// �^�C�}�𑜓x���P�~���b�ɂ���
	timeBeginPeriod(1);

	// �����_���[��������
	Renderer::Init(ap);

	// ��������
	Audio::InitMaster();

	// ���͂�������
	Input::Init(hWnd);

	// Title�������V�[���ɓo�^
	SetScene<Title>();
}

void Manager::Uninit()
{
	m_Scene->UninitBase();
	delete m_Scene;

	// �R�c���f����S�ĉ��
	ModelRenderer::UnloadAll();

	// �C���v�b�g�I��
	Input::Uninit();

	// �I�[�f�B�I�I��
	Audio::UninitMaster();

	// �^�C�}�𑜓x�����ɖ߂�
	timeEndPeriod(1);
}

void Manager::Update(uint64_t d)
{
	Input::Update();

	m_Scene->UpdateBase();
}

void Manager::Draw(uint64_t d)
{
	Renderer::Begin();

	m_Scene->DrawBase();

	Renderer::End();
}
