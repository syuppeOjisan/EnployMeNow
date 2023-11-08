#include	"System/main.h"
#include	"System/renderer.h"
#include	"System/manager.h"
#include	"System/input.h"
#include	"Scene/Base/scene.h"
#include	"Scene/game.h"
#include	"Scene/title.h"
#include	"Component/audio.h"

#include "imgui_impl_dx11.h"
#include "imgui_impl_win32.h"

// �V�[���}�l�[�W���������o�ϐ��̎���
Scene* Manager::m_Scene{};		// ���݃V�[��
bool Manager::m_IsLoop;			// ���[�v�t���O

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

	// ���[�v�t���O������
	m_IsLoop = true;
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

void Manager::Update(uint64_t _deltaTime)
{
	Input::Update();

	m_Scene->UpdateBase(_deltaTime);
}

void Manager::Draw(uint64_t d)
{
	Renderer::Begin();

	ImGui_ImplDX11_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();

	m_Scene->DrawBase();

	ImGui::Render();
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

	Renderer::End();
}
