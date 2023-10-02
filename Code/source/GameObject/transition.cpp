#include <SimpleMath.h>
#include "System/main.h"
#include "System/renderer.h"
#include "GameObject/transition.h"
#include "Component/shader.h"
#include "Component/sprite.h"

using namespace DirectX::SimpleMath;

void Transition::Init()
{
	AddComponent<Shader>()->Load("shader\\unlitTextureVS.cso", "shader\\unlitTexturePS.cso");

	m_Sprite = AddComponent<Sprite>();
	m_Sprite->Init(0.0f, 0.0f, SCREEN_WIDTH, SCREEN_HEIGHT, "asset\\texture\\title.jpg");

	// マテリアルの設定
	MATERIAL material{};
	material.Diffuse = Color(0.0f, 0.0f, 0.0f, 1.0f);
	material.TextureEnable = false;
	m_Sprite->SetMaterial(material);
}

void Transition::Update()
{
	switch (m_State)
	{

		case State::Stop:							// フェードイン終了を表す
			break;

		case State::In:								// フェードイン中（透明度を下げていく）
		{
			m_Alpha -= 2.0f / 60.0f;					// １秒間に２ずつ透明

			if (m_Alpha < 0.0f)							// 完全に透明になったらSTOP状態へ
				m_State = State::Stop;

			break;
		}

		case State::Out:							// フェードアウト中
		{
			m_Alpha += 2.0f / 60.0f;					// １秒間に２ずつ不透明

			if (m_Alpha > 1.0f)							// 完全に不透明になったらFINISH状態へ
				m_State = State::Finish;

			break;
		}

		case State::Finish:							// フェードアウト終了を表す
			break;

		default:
			break;
	}


	// マテリアルの設定
	MATERIAL material{};
	material.Diffuse = Color(0.0f, 0.0f, 0.0f, m_Alpha);
	material.TextureEnable = false;
	m_Sprite->SetMaterial(material);

}

