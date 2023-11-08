#include "System/renderer.h"
#include "Component/shader.h"

void Shader::Load(const char* VertexShader, const char* PixelShader)
{

	Renderer::CreateVertexShader(&m_VertexShader, &m_VertexLayout, VertexShader);

	Renderer::CreatePixelShader(&m_PixelShader, PixelShader);
}

void Shader::Uninit()
{
	if (m_VertexLayout){ m_VertexLayout->Release(); }
	if (m_VertexShader){ m_VertexShader->Release(); }
	if (m_PixelShader) { m_PixelShader->Release(); }
}


void Shader::Draw()
{
	// ���̓��C�A�E�g�ݒ�
	Renderer::GetDeviceContext()->IASetInputLayout(m_VertexLayout);

	// �V�F�[�_�ݒ�
	Renderer::GetDeviceContext()->VSSetShader(m_VertexShader, nullptr, 0);
	Renderer::GetDeviceContext()->PSSetShader(m_PixelShader, nullptr, 0);
}


