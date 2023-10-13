#include "System/main.h"
#include "System/manager.h"
#include "System/renderer.h"
#include "System/input.h"
#include "System/manager.h"
#include "System/utility.h"

#include "GameObject/camera.h"
#include "GameObject/PlayerCamera.h"
#include "GameObject/field.h"
#include "GameObject/player.h"
#include "GameObject/box.h"
#include "GameObject/cylinder.h"
#include "GameObject/bullet.h"
#include "GameObject/score.h"
#include "GameObject/transition.h"
#include "GameObject/goal.h"
#include "GameObject/sky.h"
#include "GameObject/Polygon2D.h"

#include "Scene/result.h"
#include "Scene/Stage02.h"

#include "Component/Object_SizeChange.h"
#include "Component/sprite.h"
#include "Component/shader.h"
#include "Component/audio.h"

using namespace DirectX::SimpleMath;

// ゲーム初期処理
void Stage02::Init()
{
	Bullet::Load();

	AddGameObject<Sky>(1);
	AddGameObject<Field>(1);
	AddGameObject<Player>(1);
	AddGameObject<Score>(3);

	// チェック完了
	{
		// カメラを生成しゲームシーンに追加、プレイヤーをターゲットに指定
		PlayerCamera* pCamera = AddGameObject<PlayerCamera>(0);
		pCamera->SetTarget(GetGameObject<Player>());
		// プレイヤーにもカメラの情報を渡す
		GetGameObject<Player>()->SetCamera(pCamera);
	}

	// TODO:いい感じのステージを作る
	// チェック完了
	{
		Box* box = AddGameObject<Box>(1);
		box->SetPosition(Vector3(-5.0f, 0.0f, 5.0f));
		box->SetScale(Vector3(1.0f, 1.0f, 1.0f));
		box->AddComponent<Object_SizeChange>();
	}

	// チェック完了
	{
		Box* box = AddGameObject<Box>(1);
		box->SetPosition(Vector3(-11.0f, 0.0f, 5.0f));
		box->SetScale(Vector3(1.0f, 1.0f, 1.0f));
		box->AddComponent<Object_SizeChange>();	}

	// チェック完了
	{
		Box* box = AddGameObject<Box>(1);
		box->SetPosition(Vector3(11.0f, 0.0f, 10.0f));
		box->SetScale(Vector3(1.0f, 1.0f, 1.0f));
		box->AddComponent<Object_SizeChange>();
	}

	// チェック完了
	{
		Box* box = AddGameObject<Box>(1);
		box->SetPosition(Vector3(11.0f, 0.0f, 20.0f));
		box->SetScale(Vector3(2.0f, 3.0f, 2.0f));
	}

	// チェック完了
	{
		Goal* goal = AddGameObject<Goal>(1);
		goal->SetPosition(Vector3(11.0f, 9.0f, 20.0f));
		goal->SetScale(Vector3(0.5f, 0.5f, 0.5f));
	}

	// UI用画像設定
	{
		GameObject* sizeUp = AddGameObject<GameObject>(3);
		sizeUp->AddComponent<Shader>()->Load("shader\\unlitTextureVS.cso", "shader\\unlitTexturePS.cso");
		sizeUp->AddComponent<Sprite>()->Init(25.0f, 50.0f, 300, 100, "asset\\texture\\sizeUp.png");

		GameObject* sizeDown = AddGameObject<GameObject>(3);
		sizeDown->AddComponent<Shader>()->Load("shader\\unlitTextureVS.cso", "shader\\unlitTexturePS.cso");
		sizeDown->AddComponent<Sprite>()->Init(25.0f, 50.0f, 300, 100, "asset\\texture\\sizeDown.png");

		GameObject* switchAmmo = AddGameObject<GameObject>(3);
		switchAmmo->AddComponent<Shader>()->Load("shader\\unlitTextureVS.cso", "shader\\unlitTexturePS.cso");
		switchAmmo->AddComponent<Sprite>()->Init(25.0f, 400.0f, 200, 100, "asset\\texture\\switchAmmo.png");

		GameObject* fireAmmo = AddGameObject<GameObject>(3);
		fireAmmo->AddComponent<Shader>()->Load("shader\\unlitTextureVS.cso", "shader\\unlitTexturePS.cso");
		fireAmmo->AddComponent<Sprite>()->Init(50.0f, 500.0f, 150, 195, "asset\\texture\\fireAmmo.png");
	}

	// 画面遷移オブジェクトを登録
	m_Transition = AddGameObject<Transition>(3);
	m_Transition->FadeIn();
}

void Stage02::Uninit()
{

}

// ゲーム更新処理
void Stage02::Update()
{
	// プレイヤーが撃つ弾のモード変更
	// UIを変更
	if (GetGameObject<Player>()->GetBulletMode() == SIZE_UP)
	{
		GetGameObjects<GameObject>()[0]->GetComponent<Sprite>()->SetActive(true);
		GetGameObjects<GameObject>()[1]->GetComponent<Sprite>()->SetActive(false);
	}
	else if (GetGameObject<Player>()->GetBulletMode() == SIZE_DOWN)
	{
		GetGameObjects<GameObject>()[0]->GetComponent<Sprite>()->SetActive(false);
		GetGameObjects<GameObject>()[1]->GetComponent<Sprite>()->SetActive(true);
	}

	// ゴールしていないのであれば
	if (!m_Goal)
	{
		Goal* goal = GetGameObject<Goal>();

		// ゴールした際にゴールオブジェクトは削除される
		if (goal == nullptr)
		{
			m_Goal = true;

			// ２秒後にスレッドを生成してフェードアウト開始
			Invoke([=]() { m_Transition->FadeOut(); }, 2000);
		}
	}

	// フェードアウトが終了しているか？
	if (m_Transition->GetState() == Transition::State::Finish)
	{
		Manager::SetScene<Result>();
	}
}
