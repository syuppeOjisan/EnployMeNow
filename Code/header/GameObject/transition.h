#pragma once


#include "Base/gameObject.h"

class Transition : public GameObject
{
public:
	enum class State
	{
		Stop,			// 停止
		In,				// フェードイン
		Out,			// フェードアウト
		Finish			// 画面遷移終了
	};


private:

	float m_Alpha = 0.0f;
	State m_State = State::Stop;

	class Sprite* m_Sprite{};

public:


	void Init() override;
	void Update() override;

	State GetState() { return m_State; }


	void FadeIn()
	{
		m_Alpha = 1.0f;
		m_State = State::In;
	}

	void FadeOut() {
		m_Alpha = 0.0f;
		m_State = State::Out;
	}

};