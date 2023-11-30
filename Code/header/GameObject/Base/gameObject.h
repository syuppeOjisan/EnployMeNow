#pragma once

#include "../../System/renderer.h"

#include <list>
#include <SimpleMath.h>
#include <corecrt_math_defines.h>
#include "System/main.h"
#include "System/InputIntarface.h"

#include "../../Component/Base/component.h"

using namespace DirectX::SimpleMath;

class GameObject
{
public:
	GameObject() {}//コンストラクタ
	virtual ~GameObject() {}//デストラクタ（仮想関数）

	/// <summary>
	/// 座標を取得
	/// </summary>
	/// <returns>座標</returns>
	DirectX::SimpleMath::Vector3 GetPosition() { return m_Position; }

	/// <summary>
	/// 座標をセット
	/// </summary>
	/// <param name="Position">セットしたい座標</param>
	void SetPosition(DirectX::SimpleMath::Vector3 Position) { m_Position = Position; }

	/// <summary>
	/// 位置変数のアドレスを取得
	/// </summary>
	/// <returns>座標変数のアドレス</returns>
	DirectX::SimpleMath::Vector3* GetPositionAdress() { return &m_Position; }

	/// <summary>
	/// 現在の回転角を取得(ラジアン)
	/// </summary>
	/// <returns>現在の回転角</returns>
	DirectX::SimpleMath::Vector3 GetRotation() { return m_Rotation; }

	/// <summary>
	/// 回転角をセット(ラジアン)
	/// </summary>
	/// <param name="Rotation">セットしたい回転角</param>
	void SetRotation(DirectX::SimpleMath::Vector3 Rotation) 
	{ 
		// ラジアンを範囲内に収めるために補正
		Rotation.x = FixRadian(Rotation.x);
		Rotation.y = FixRadian(Rotation.y);
		Rotation.z = FixRadian(Rotation.z);

		// 回転角として反映
		m_Rotation = Rotation;
	}

	/// <summary>
	/// スケールを取得
	/// </summary>
	/// <returns>現在のスケール</returns>
	DirectX::SimpleMath::Vector3 GetScale() { return m_Scale; }

	/// <summary>
	/// スケールをセット
	/// </summary>
	/// <param name="Scale">セットしたいスケール</param>
	void SetScale(DirectX::SimpleMath::Vector3 Scale) { m_Scale = Scale; }



	/// <summary>
	/// 入力インターフェースをセット
	/// </summary>
	/// <param name="_input">入力インターフェース</param>
	void SetInput(InputIntarface* _input) { m_pInput = _input; m_pInput->Init(); }

	/// <summary>
	/// オブジェクトの向きをベクトルの方に向ける
	/// </summary>
	/// <param name="_vector">向いてほしい方向ベクトル</param>
	void SetRotateToVector(Vector3 _vector)
	{
		// ベクトルが向いている角度を求める
		float rotateRadianDestination = atan2f(_vector.x, _vector.z);
		rotateRadianDestination = FixRadian(rotateRadianDestination);	// ラジアン補正

		// 現在の回転角との角度差を求める
		float rotateRadian = rotateRadianDestination - GetRotation().y;
		rotateRadian = FixRadian(rotateRadian);	// ラジアン補正

		// 現在の回転角との差を求めて回転後の値を求める
		Vector3 pRotate = GetRotation();
		pRotate.y += rotateRadian;
		pRotate.y = FixRadian(pRotate.y);

		// 回転を適用
		SetRotation(Vector3(0, pRotate.y, 0));
	}

	/// <summary>
	/// オブジェクトの向きをベクトルの方向にゆっくり回転させる
	/// </summary>
	/// <param name="_vector">向いてほしい方向ベクトル</param>
	void SetRotateToVectorEaseIn(Vector3 _vector)
	{
		// ベクトルが向いている角度を求める
		float rotateRadian = atan2f(_vector.x, _vector.z);
		rotateRadian = FixRadian(rotateRadian);	// ラジアン補正

		// 現在の回転角との角度差を求める
		float rotateRadianDestination = rotateRadian - GetRotation().y;
		rotateRadianDestination = FixRadian(rotateRadianDestination);	// ラジアン補正

		// 回転後の値を求める ゆっくり回転
		float easeInFactor = 0.5f;// 回転のスピード
		Vector3 pRotate = GetRotation();
		pRotate.y += rotateRadianDestination * easeInFactor;
		pRotate.y = FixRadian(pRotate.y);

		// 回転を適用
		SetRotation(Vector3(0, pRotate.y, 0));
	}

	/// <summary>
	/// 前方ベクトルを取得
	/// </summary>
	/// <returns>前方ベクトル</returns>
	DirectX::SimpleMath::Vector3 GetForward()
	{
		// 回転行列を作成
		DirectX::SimpleMath::Matrix rot;
		rot = DirectX::SimpleMath::Matrix::CreateFromYawPitchRoll(m_Rotation.y, m_Rotation.x, m_Rotation.z);

		// 回転行列の3列目にある前方ベクトルを取得
		DirectX::SimpleMath::Vector3 forward;
		forward.x = rot._31;
		forward.y = rot._32;
		forward.z = rot._33;

		return forward;
	}

	/// <summary>
	/// ラジアンの値を補正する
	/// </summary>
	/// <param name="_radian">回転角度ラジアン</param>
	/// <returns>補正後の値</returns>
	float FixRadian(float _radian)
	{
		if (_radian > DirectX::XM_PI)
		{
			_radian -= DirectX::XM_PI * 2.0f;
		}
		if (_radian < -DirectX::XM_PI)
		{
			_radian += DirectX::XM_PI * 2.0f;
		}

		return _radian;
	}

	/// <summary>
	/// デストロイフラグをTrueにする
	/// </summary>
	void SetDestroy() { m_Destroy = true; }

	bool Destroy()
	{
		if (m_Destroy)
		{
			UninitBase();
			delete this;
			return true;
		}
		else
		{
			return false;
		}
	}



	virtual void Init() {}
	virtual void Uninit() {}
	virtual void Update() {}
	virtual void Draw() {}
	virtual void PreDraw() {}

	// コンポーネント追加
	template <typename T>
	T* AddComponent()
	{
		std::unique_ptr<T> component = std::make_unique<T>(this);	// ユニークポインタとしてコンポーネントを生成
		m_Component.push_back(std::move(component));				// 所有権をリストに移行して追加
		m_Component.back()->Init();									// リストから初期化を呼び出す

		return dynamic_cast<T*>(m_Component.back().get());			// リストからコンポーネントの生ポを取り出して返す、一応dynamic_cast
	}

	// コンポーネント取得
	template <typename T>
	T* GetComponent()
	{
		for (auto& component : m_Component)
		{
			T* castedComponent = dynamic_cast<T*>(component.get());	// リストからコンポーネントを取得してT型にキャスト
			if (castedComponent)									
			{
				return castedComponent;								// キャストが成功したらそのコンポーネントを返す
			}
		}
		return nullptr;												// キャストが成功しなかったらnullptrを返す
	}

	// 子オブジェクトを追加
	template <typename T>
	T* AddChild()
	{
		std::unique_ptr<T> childObj = std::make_unique<T>();		// ユニークポインタとして子オブジェクトを生成
		m_ChildGameObject.push_back(std::move(childObj));			// 所有権をリストに移行して追加
		m_ChildGameObject.back()->Init();							// リストから初期化を呼び出す

		return dynamic_cast<T*>(m_ChildGameObject.back().get());	// リストから子オブジェクトの生ポを取り出して返す
	}

	void InitBase()
	{
		Init();
	}

	void UninitBase()
	{
		Uninit();
		
		// 子オブジェクト関連の終了処理
		for (auto& childObj : m_ChildGameObject)
		{
			childObj->Uninit();				// 子オブジェクトの終了処理

			for (auto& component : childObj->m_Component)
			{
				component->Uninit();		// 子オブジェクトが持っているコンポーネントの終了処理
			}
			childObj->m_Component.clear();	// 子オブジェクトのコンポーネントリストをクリア

		}
		m_ChildGameObject.clear();			//子オブジェクトのリストをクリア

		// このオブジェクトの終了処理
		for (auto& component : m_Component)
		{
			component->Uninit();			// コンポーネントの終了処理s
		}
		m_Component.clear();				// コンポーネントリストのクリア
	}

	void UpdateBase(uint64_t _deltaTime)
	{
		// デルタタイムを更新
		this->m_DeltaTime = _deltaTime;
		
		// コンポーネントを更新
		for (auto& component : m_Component)
		{
			component->Update();
		}

		// 更新処理
		Update();
	}

	void DrawBase(DirectX::SimpleMath::Matrix ParentMatrix)
	{
		// マトリクス設定
		DirectX::SimpleMath::Matrix world, scale, rot, trans;
		scale = DirectX::SimpleMath::Matrix::CreateScale(m_Scale.x, m_Scale.y, m_Scale.z);
		rot = DirectX::SimpleMath::Matrix::CreateFromYawPitchRoll(m_Rotation.y, m_Rotation.x, m_Rotation.z);
		trans = DirectX::SimpleMath::Matrix::CreateTranslation(m_Position.x, m_Position.y, m_Position.z);
		world = scale * rot * trans * ParentMatrix;

		PreDraw();

		for (auto& child : m_ChildGameObject)
		{
			child->DrawBase(world);
		}


		Renderer::SetWorldMatrix(&world);

		for (auto& component : m_Component)
		{
			component->Draw();
		}

		Draw();
	}

protected:
	bool		m_Destroy = false;
	uint64_t	m_DeltaTime;

	DirectX::SimpleMath::Vector3	m_Position = DirectX::SimpleMath::Vector3(0.0f, 0.0f, 0.0f);
	DirectX::SimpleMath::Vector3	m_Rotation = DirectX::SimpleMath::Vector3(0.0f, 0.0f, 0.0f); // 回転
	DirectX::SimpleMath::Vector3	m_Scale = DirectX::SimpleMath::Vector3(1.0f, 1.0f, 1.0f);

	std::list<std::unique_ptr<Component>> m_Component;

	std::list<std::unique_ptr<GameObject>> m_ChildGameObject;

	InputIntarface* m_pInput;	// 入力インターフェース
};