#include "System/main.h"
#include "System/manager.h"
#include "System/renderer.h"
#include "System/input.h"
#include "System/manager.h"
#include "System/utility.h"
#include "imgui_impl_dx11.h"

#include "GameObject/camera.h"
#include "GameObject/PlayerCamera.h"
#include "GameObject/field.h"
#include "GameObject/player.h"
#include "GameObject/box.h"
#include "GameObject/cylinder.h"
#include "GameObject/bullet.h"
#include "GameObject/enemy.h"
#include "GameObject/battery.h"
#include "GameObject/score.h"
#include "GameObject/explosion.h"
#include "GameObject/transition.h"
#include "GameObject/goal.h"
#include "GameObject/sky.h"
#include "GameObject/Polygon2D.h"
#include "GameObject/Item_X.h"
#include "GameObject/Item_Y.h"
#include "GameObject/Item_Z.h"

#include "Scene/Stage01.h"
#include "Scene/Stage02.h"

#include "Component/Object_SizeChange.h"
#include "Component/Object_ScaleX.h"
#include "Component/Object_ScaleY.h"
#include "Component/Object_ScaleZ.h"
#include "Component/sprite.h"
#include "Component/shader.h"
#include "Component/audio.h"

using namespace DirectX::SimpleMath;

// ゲーム初期処理
void Stage01::Init()
{
	Bullet::Load();
	Explosion::Load();

	AddGameObject<Sky>(1);
	AddGameObject<Field>(1);
	Player* player = AddGameObject<Player>(1);
	player->SetPosition(Vector3(0, 0, -20));

	// チェック完了
	{
		// カメラを生成しゲームシーンに追加、プレイヤーをターゲットに指定
		PlayerCamera* pCamera = AddGameObject<PlayerCamera>(0);
		pCamera->SetTarget(GetGameObject<Player>());
		// プレイヤーにもカメラの情報を渡す
		GetGameObject<Player>()->SetCamera(pCamera);
	}

	// ゴールの周り
	{
		// ゴール土台
		{
			Box* box = AddGameObject<Box>(1);
			box->SetPosition(Vector3(0.0f, 0.0f, 20.0f));
			box->SetScale(Vector3(2.0f, 3.0f, 2.0f));
		}

		// ゴール
		{
			Goal* goal = AddGameObject<Goal>(1);
			goal->SetPosition(Vector3(0.0f, 7.0f, 20.0f));
			goal->SetScale(Vector3(0.5f, 0.5f, 0.5f));
		}

		// ゴール前
		{
			Box* box = AddGameObject<Box>(1);
			box->SetPosition(Vector3(0.0f, 0.0f, 15.0f));
			box->SetScale(Vector3(1.0f, 1.0f, 1.0f));
			box->AddComponent<Object_ScaleY>();
		}

		// アイテム説明用ブロックたち
		{
			// アイテムX
			{
				ItemBase* itemX = AddGameObject<Item_X>(1);
				itemX->SetPosition(Vector3(-5.0f, 0.8f, -15.0f));
				itemX->SetScale(Vector3(3.0f, 3.0f, 3.0f));
			}

			{
				Box* box = AddGameObject<Box>(1);
				box->SetPosition(Vector3(-15.0f, 0.8f, -5.0f));
				box->SetScale(Vector3(1.0f, 1.0f, 1.0f));
				box->AddComponent<Object_ScaleX>();
				
			}

			// アイテムY
			{
				ItemBase* itemX = AddGameObject<Item_Y>(1);
				itemX->SetPosition(Vector3(0.0f, 0.8f, -15.0f));
				itemX->SetScale(Vector3(3.0f, 3.0f, 3.0f));
			}

			{
				Box* box = AddGameObject<Box>(1);
				box->SetPosition(Vector3(0.0f, 0.8f, -5.0f));
				box->SetScale(Vector3(1.0f, 1.0f, 1.0f));
				box->AddComponent<Object_ScaleY>();
			}

			// アイテムZ
			{
				ItemBase* itemX = AddGameObject<Item_Z>(1);
				itemX->SetPosition(Vector3(5.0f, 0.8f, -15.0f));
				itemX->SetScale(Vector3(3.0f, 3.0f, 3.0f));
			}

			{
				Box* box = AddGameObject<Box>(1);
				box->SetPosition(Vector3(15.0f, 0.8f, -5.0f));
				box->SetScale(Vector3(1.0f, 1.0f, 1.0f));
				box->AddComponent<Object_ScaleZ>();
			}
		}

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

		GameObject* itemX = AddGameObject<GameObject>(3);
		itemX->AddComponent<Shader>()->Load("shader\\unlitTextureVS.cso", "shader\\unlitTexturePS.cso");
		itemX->AddComponent<Sprite>()->Init(750.0f, 500.0f, 500, 250, "asset\\texture\\Item_X.png");

		GameObject* itemY = AddGameObject<GameObject>(3);
		itemY->AddComponent<Shader>()->Load("shader\\unlitTextureVS.cso", "shader\\unlitTexturePS.cso");
		itemY->AddComponent<Sprite>()->Init(750.0f, 500.0f, 500, 250, "asset\\texture\\Item_Y.png");

		GameObject* itemZ = AddGameObject<GameObject>(3);
		itemZ->AddComponent<Shader>()->Load("shader\\unlitTextureVS.cso", "shader\\unlitTexturePS.cso");
		itemZ->AddComponent<Sprite>()->Init(750.0f, 500.0f, 500, 250, "asset\\texture\\Item_Z.png");

		GameObject* itemNone = AddGameObject<GameObject>(3);
		itemNone->AddComponent<Shader>()->Load("shader\\unlitTextureVS.cso", "shader\\unlitTexturePS.cso");
		itemNone->AddComponent<Sprite>()->Init(750.0f, 500.0f, 500, 250, "asset\\texture\\Item_None.png");
	}

	// 画面遷移オブジェクトを登録
	m_Transition = AddGameObject<Transition>(3);
	m_Transition->FadeIn();

	// BGM読み込み
	m_BGMPLayer = AddGameObject<GameObject>(3);
	m_BGMPLayer->AddComponent<Audio>()->Load("asset\\audio\\bgm_stage01.wav");
	//m_BGMPLayer->GetComponent<Audio>()->Play(true);

}

void Stage01::Uninit()
{

}

// ゲーム更新処理
void Stage01::Update()
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

	switch (GetGameObject<Player>()->GetBulletKind())
	{
	case CHANGE_X_AXSIS:
		GetGameObjects<GameObject>()[4]->GetComponent<Sprite>()->SetActive(true);
		GetGameObjects<GameObject>()[5]->GetComponent<Sprite>()->SetActive(false);
		GetGameObjects<GameObject>()[6]->GetComponent<Sprite>()->SetActive(false);
		GetGameObjects<GameObject>()[7]->GetComponent<Sprite>()->SetActive(false);
		break;
	case CHANGE_Y_AXSIS:
		GetGameObjects<GameObject>()[4]->GetComponent<Sprite>()->SetActive(false);
		GetGameObjects<GameObject>()[5]->GetComponent<Sprite>()->SetActive(true);
		GetGameObjects<GameObject>()[6]->GetComponent<Sprite>()->SetActive(false);
		GetGameObjects<GameObject>()[7]->GetComponent<Sprite>()->SetActive(false);
		break;
	case CHANGE_Z_AXSIS:
		GetGameObjects<GameObject>()[4]->GetComponent<Sprite>()->SetActive(false);
		GetGameObjects<GameObject>()[5]->GetComponent<Sprite>()->SetActive(false);
		GetGameObjects<GameObject>()[6]->GetComponent<Sprite>()->SetActive(true);
		GetGameObjects<GameObject>()[7]->GetComponent<Sprite>()->SetActive(false);
		break;
	case NONE:
		GetGameObjects<GameObject>()[4]->GetComponent<Sprite>()->SetActive(false);
		GetGameObjects<GameObject>()[5]->GetComponent<Sprite>()->SetActive(false);
		GetGameObjects<GameObject>()[6]->GetComponent<Sprite>()->SetActive(false);
		GetGameObjects<GameObject>()[7]->GetComponent<Sprite>()->SetActive(true);
		break;
	}


	// ゴールしていないのであれば
	if (!m_Goal)
	{
		Goal* goal = GetGameObject<Goal>();

		// ゴールした際にゴールオブジェクトは削除される
		if (goal == nullptr)
		{
			m_BGMPLayer->GetComponent<Audio>()->Stop();
			m_Goal = true;

			// ２秒後にスレッドを生成してフェードアウト開始
			Invoke([=]() { m_Transition->FadeOut(); }, 2000);
		}
	}

	// フェードアウトが終了しているか？
	if (m_Transition->GetState() == Transition::State::Finish)
	{
		Manager::SetScene<Stage02>();
	}

	// GUI出します
	bool showGUI = true;
	ImGui::SetNextWindowSize(ImVec2(320, 100), ImGuiCond(ImGuiCond_Once));
	ImGui::Begin("PlayerInfo", &showGUI);
	ImGui::Text("YOOOOOOOO");
	ImGui::Text("IM_THE_TOP_G");
	ImGui::End();
}
