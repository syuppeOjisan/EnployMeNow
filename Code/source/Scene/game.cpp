#include "System/main.h"
#include "System/manager.h"
#include "System/renderer.h"
#include "System/input.h"
#include "System/KeyboardInput.h"
#include "System/PadInput.h"
#include "System/MouseInput.h"
#include "System/manager.h"
#include "System/utility.h"

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
#include "GameObject/grandpa.h"

#include "Scene/result.h"
#include "Scene/game.h"
#include "Scene/Stage01.h"
#include "Scene/Stage02.h"


#include "Component/Object_SizeChange.h"
#include "Component/sprite.h"
#include "Component/shader.h"
#include "Component/audio.h"

#include "imgui.h"

using namespace DirectX::SimpleMath;

// ゲーム初期処理
void Game::Init()
{
	Bullet::Load();
	Explosion::Load();

	AddGameObject<Sky>(1);
	AddGameObject<Field>(1);
	AddGameObject<Score>(3);
	AddGameObject<grandpa>(1);

	//// 敵追加
	//AddGameObject<Enemy>(1)->SetPosition(Vector3(0.0f, 0.0f, 5.0f));
	//AddGameObject<Enemy>(1)->SetPosition(Vector3(5.0f, 0.0f, 5.0f));
	//AddGameObject<Enemy>(1)->SetPosition(Vector3(10.0f, 0.0f, 5.0f));
	//AddGameObject<Enemy>(1)->SetPosition(Vector3(15.0f, 0.0f, 5.0f));

	//// 味方追加
	//AddGameObject<Battery>(1)->SetPosition(Vector3(20.0f, 0.0f, 5.0f));

	{
		AddGameObject<Player>(1);
		Player* pPlayer = GetGameObject<Player>();
		InputIntarface* pInput = new PadInput();	// キーボード入力デバイスを渡す
		pPlayer->SetInput(pInput);
	}


	// チェック完了
	{
		// カメラを生成しゲームシーンに追加、プレイヤーをターゲットに指定
		PlayerCamera* pCamera = AddGameObject<PlayerCamera>(0);
		pCamera->SetTarget(GetGameObject<Player>());
		// 入力インターフェースを渡す
		InputIntarface* pInput = new PadInput();
		pCamera->SetInput(pInput);
		// プレイヤーにもカメラの情報を渡す
		GetGameObject<Player>()->SetCamera(pCamera);
	}


	// チェック完了
	{
		grandpa* pGrandpa = GetGameObject<grandpa>();
		pGrandpa->SetPosition(Vector3(0, 0, 0));
	}

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
		box->AddComponent<Object_SizeChange>();
	}

	// チェック完了
	{
		Box* box = AddGameObject<Box>(1);
		box->SetPosition(Vector3(11.0f, 0.0f, 10.0f));
		box->SetScale(Vector3(1.0f, 1.0f, 1.0f));
		box->AddComponent<Object_SizeChange>();
	}

	//// チェック完了
	//{
	//	Cylinder* cylinder = AddGameObject<Cylinder>(1);
	//	cylinder->SetPosition(Vector3(-11.0f, 0.0f, 20.0f));
	//	cylinder->SetScale(Vector3(3.0f, 6.0f, 3.0f));
	//}

	//// チェック完了
	//{
	//	Cylinder* cylinder = AddGameObject<Cylinder>(1);
	//	cylinder->SetPosition(Vector3(11.0f, 0.0f, 20.0f));
	//	cylinder->SetScale(Vector3(3.0f, 6.0f, 3.0f));
	//}

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

// ゲーム終了処理
void Game::Uninit()
{
}

// ゲーム更新処理
void Game::Update()
{
	// フェードインが終了しているか？	
//	if (m_Transition->GetState() == Transition::State::Stop) {
//		if (Input::GetKeyTrigger(VK_RETURN))
//		{
//			m_Transition->FadeOut();
//		}
//	}

	HitCheck();

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

void Game::Draw()
{
	// GUI描画
	{
		// メニューバーをいい感じに表示
		ImGui::BeginMainMenuBar();
		ImGui::Text("%.1f |", ImGui::GetIO().Framerate);
		if (ImGui::BeginMenu("Scene"))
		{
			if (ImGui::MenuItem("game"))
			{
				Manager::SetScene<Game>();
			}
			else if (ImGui::MenuItem("Stage01"))
			{
				Manager::SetScene<Stage01>();
			}
			else if (ImGui::MenuItem("Stage02"))
			{
				Manager::SetScene<Stage02>();
			}


			ImGui::EndMenu();
		}

		if (ImGui::BeginMenu("Quit"))
		{
			Manager::EndApp();

			ImGui::EndMenu();
		}
		ImGui::EndMainMenuBar();


		ImGui::Begin("CharactorInfos");

		if (GetGameObject<Player>())
		{
			Player* pPlayer = GetGameObject<Player>();
			PlayerCamera* pCamera = GetGameObject<PlayerCamera>();
			InputIntarface* pPad = new PadInput();
			pPad->Update();
			Vector2 StickInfo{};
			Vector2 tmp{};
			pPad->GetDeviceMovement(StickInfo, tmp);


			float test = 3.0f;
			if (ImGui::TreeNode("PlayerInfo"))
			{
				if (ImGui::TreeNode("Position"))
				{
					Vector3 playerPos = pPlayer->GetPosition();
					ImGui::InputFloat("X:%f", &playerPos.x);
					ImGui::InputFloat("Y:%f", &playerPos.y);
					ImGui::InputFloat("Z:%f", &playerPos.z);
					pPlayer->SetPosition(playerPos);

					ImGui::TreePop();
				}
				if (ImGui::TreeNode("Rotate"))
				{
					Vector3 rotate = pPlayer->GetRotation();
					rotate.x = RADIAN_TO_DEGREE(rotate.x);
					rotate.y = RADIAN_TO_DEGREE(rotate.y);
					rotate.z = RADIAN_TO_DEGREE(rotate.z);
					ImGui::DragFloat("XAxsis", &rotate.x, 1.0f);
					ImGui::DragFloat("YAxsis", &rotate.y, 1.0f);
					ImGui::DragFloat("ZAxsis", &rotate.z, 1.0f);
					rotate.x = DEGREE_TO_RADIAN(rotate.x);
					rotate.y = DEGREE_TO_RADIAN(rotate.y);
					rotate.z = DEGREE_TO_RADIAN(rotate.z);
					pPlayer->SetRotation(rotate);

					ImGui::Text("FrontVector");
					ImGui::Text("X:%f", pPlayer->GetForward().x);
					ImGui::Text("Y:%f", pPlayer->GetForward().y);
					ImGui::Text("Z:%f", pPlayer->GetForward().z);

					ImGui::TreePop();
				}
				if (ImGui::TreeNode("Scale"))
				{
					ImGui::Text("X:%f", pPlayer->GetScale().x);
					ImGui::Text("Y:%f", pPlayer->GetScale().y);
					ImGui::Text("Z:%f", pPlayer->GetScale().z);

					ImGui::TreePop();
				}
				if (ImGui::TreeNode("Velocity"))
				{
					ImGui::Text("X:%f", pPlayer->GetVelocity().x);
					ImGui::Text("Y:%f", pPlayer->GetVelocity().y);
					ImGui::Text("Z:%f", pPlayer->GetVelocity().z);

					ImGui::TreePop();
				}
				if (ImGui::TreeNode("Collision"))
				{
					ImGui::Text("MAX - X:%f", pPlayer->GetAABBCollision().max.x);
					ImGui::Text("MAX - Y:%f", pPlayer->GetAABBCollision().max.y);
					ImGui::Text("MAX - Z:%f", pPlayer->GetAABBCollision().max.z);
					ImGui::Text("---------------------------------------------");
					ImGui::Text("MIN - X:%f", pPlayer->GetAABBCollision().min.x);
					ImGui::Text("MIN - Y:%f", pPlayer->GetAABBCollision().min.y);
					ImGui::Text("MIN - Z:%f", pPlayer->GetAABBCollision().min.z);
					ImGui::Checkbox("IsHit", pPlayer->GetIsHit());
					ImGui::TreePop();
				}
				if (ImGui::TreeNode("Animation"))
				{
					ImGui::Text("NowAnimationName  :%s", pPlayer->GetNowAnimName(), pPlayer->GetNowAnimName() + 1);
					ImGui::Text("NowAnimationFrame :%d", pPlayer->GetNowAnimationFrame());
					ImGui::Text("---------------------------------------------");
					ImGui::Text("PrevAnimationName :%s", pPlayer->GetPrevAnimName(), pPlayer->GetPrevAnimName() + 1);
					ImGui::Text("PrevAnimationFrame :%d", pPlayer->GetPrevAnimationFrame());
					ImGui::Text("---------------------------------------------");
					bool isEnd = pPlayer->GetIsAnimBlendOver();
					ImGui::Checkbox("isAnimBlendOver", &isEnd);
					ImGui::Text("BlendRate:%f", pPlayer->GetBlendRate());

					ImGui::TreePop();
				}
				if (ImGui::TreeNode("MOVEING"))
				{
					ImGui::Text("CamFrontVec");
					ImGui::Text("X:%f", pCamera->GetCameraFrontVec().x);
					ImGui::Text("Y:%f", pCamera->GetCameraFrontVec().y);
					ImGui::Text("Z:%f", pCamera->GetCameraFrontVec().z);
					ImGui::Text("---------------------------------------------");
					ImGui::Text("StickVec");
					ImGui::Text("X:%f", StickInfo.x);
					ImGui::Text("Y:%f", StickInfo.y);
					ImGui::Text("---------------------------------------------");
					Vector3 forward = pCamera->GetCameraFrontVec();
					forward.x *= StickInfo.x;
					forward.z *= StickInfo.y;
					forward.Normalize();
					forward.x *= 0.1;
					forward.y = 0;
					forward.z *= 0.1;
					ImGui::Text("Forward");
					ImGui::Text("X:%f", forward.x);
					ImGui::Text("Y:%f", forward.y);
					ImGui::Text("Z:%f", forward.z);

					delete pPad;

					ImGui::TreePop();
				}

				if (ImGui::Button("Slomo"))
				{
					pPlayer->SetTime(0.5f);
				}
				if (ImGui::Button("Normal"))
				{
					pPlayer->SetTime(1.0f);
				}

				ImGui::Text("Time:%f", pPlayer->GetTime());

				ImGui::TreePop();
			}
		}

		// grandpa GUI
		{
			if (GetGameObject<grandpa>())
			{
				grandpa* pPlayer = GetGameObject<grandpa>();
				float test = 3.0f;

				if (ImGui::TreeNode("grandpaInfo"))
				{
					if (ImGui::TreeNode("Position"))
					{
						Vector3 playerPos = pPlayer->GetPosition();
						ImGui::InputFloat("X:%f", &playerPos.x);
						ImGui::InputFloat("Y:%f", &playerPos.y);
						ImGui::InputFloat("Z:%f", &playerPos.z);
						pPlayer->SetPosition(playerPos);

						ImGui::TreePop();
					}
					if (ImGui::TreeNode("Scale"))
					{
						ImGui::Text("X:%f", pPlayer->GetScale().x);
						ImGui::Text("Y:%f", pPlayer->GetScale().y);
						ImGui::Text("Z:%f", pPlayer->GetScale().z);

						ImGui::TreePop();
					}
					if (ImGui::TreeNode("Velocity"))
					{
						ImGui::Text("X:%f", pPlayer->GetVelocity().x);
						ImGui::Text("Y:%f", pPlayer->GetVelocity().y);
						ImGui::Text("Z:%f", pPlayer->GetVelocity().z);

						ImGui::TreePop();
					}
					if (ImGui::TreeNode("Collision"))
					{
						ImGui::Text("MAX - X:%f", pPlayer->GetAABBCollision().max.x);
						ImGui::Text("MAX - Y:%f", pPlayer->GetAABBCollision().max.y);
						ImGui::Text("MAX - Z:%f", pPlayer->GetAABBCollision().max.z);
						ImGui::Text("---------------------------------------------");
						ImGui::Text("MIN - X:%f", pPlayer->GetAABBCollision().min.x);
						ImGui::Text("MIN - Y:%f", pPlayer->GetAABBCollision().min.y);
						ImGui::Text("MIN - Z:%f", pPlayer->GetAABBCollision().min.z);
						ImGui::Checkbox("IsHit", pPlayer->GetIsHit());
						ImGui::TreePop();
					}
					if (ImGui::TreeNode("Animation"))
					{
						ImGui::Text("NowAnimationName  :%s", pPlayer->GetNowAnimName(), pPlayer->GetNowAnimName() + 1);
						ImGui::Text("NowAnimationFrame :%d", pPlayer->GetNowAnimationFrame());
						ImGui::Text("---------------------------------------------");
						ImGui::Text("PrevAnimationName :%s", pPlayer->GetPrevAnimName(), pPlayer->GetPrevAnimName() + 1);
						ImGui::Text("PrevAnimationFrame :%d", pPlayer->GetPrevAnimationFrame());
						ImGui::Text("---------------------------------------------");

						ImGui::TreePop();
					}

					if (ImGui::Button("Slomo"))
					{
						pPlayer->SetTime(0.5f);
					}
					if (ImGui::Button("Normal"))
					{
						pPlayer->SetTime(1.0f);
					}
					
					ImGui::Text("Time:%f", pPlayer->GetTime());

					ImGui::TreePop();
				}
			}
		}

		// Camera GUI
		{
			if (GetGameObject<PlayerCamera>())
			{

				if (ImGui::TreeNode("Camera"))
				{
					PlayerCamera* pCamera = GetGameObject<PlayerCamera>();
					if (ImGui::TreeNode("Shake"))
					{
						ImGui::DragFloat("shake", pCamera->GetShake(), 0.01f);
						ImGui::DragFloat("shakeWidth", pCamera->GetShakeWidth(), 0.01f);
						ImGui::DragFloat("shakeSpeed", pCamera->GetShakeSpeed(), 0.01f);
						ImGui::Checkbox("shakeDo", pCamera->GetShakeDO());

						ImGui::TreePop();
					}

					if (ImGui::TreeNode("CameraFrontVec"))
					{
						ImGui::Text("X:%f", pCamera->GetCameraFrontVec().x);
						ImGui::Text("Y:%f", pCamera->GetCameraFrontVec().y);
						ImGui::Text("Z:%f", pCamera->GetCameraFrontVec().z);

						ImGui::TreePop();
					}

					ImGui::Checkbox("MouseMove", pCamera->GetCamMove());

					//内積から角度を求める
					Vector3 playerFront = GetGameObject<Player>()->GetForward();
					Vector3 cameraFront = GetGameObject<PlayerCamera>()->GetCameraFrontVec();

					float sensi = pCamera->getNowSensi();
					ImGui::DragFloat("CameraSensi", &sensi, 0.01f);
					pCamera->SetSensitivity(sensi);

					ImGui::TreePop();
				}
			}
		}


		ImGui::End();
	}

}

void Game::HitCheck()
{
	Player* pPlayer = GetGameObject<Player>();
	grandpa* pGrandpa = GetGameObject<grandpa>();

	Matrix playerWorldMatrix = pPlayer->GetWorldMatrix();				// プレイヤーのワールド行列

	AnimationModel* pPlayerModel = pPlayer->GetAnimationModel();	// プレイヤーのモデル
	BONE pPlayerRightHandBoneInfo = pPlayerModel->GetBoneByIndex("mixamorig:RightHand");	// プレイヤーのモデルのボーン情報 右手の甲
	BONE pPlayerLeftHandBoneInfo = pPlayerModel->GetBoneByIndex("mixamorig:LeftHand");		// プレイヤーのモデルのボーン情報 左手の甲 インデックスあってるかわからん
	

	// プレイヤーとグランパの当たり判定
	if (CollisionAABB(pPlayer->GetAABBCollision(), pGrandpa->GetAABBCollision()))
	{
		pPlayer->SetIsHit(true);

		pGrandpa->SetIsHit(true);
		pGrandpa->SetNextAnimation("Die");
		pGrandpa->CalcHitStop(26,71,0.5f);
	}
	else
	{
		pPlayer->SetIsHit(false);
		pGrandpa->SetIsHit(false);
		pGrandpa->SetNextAnimation("Idle");
	}
}