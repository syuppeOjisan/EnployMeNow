#pragma once

#include "../../System/renderer.h"

#include <list>
#include <SimpleMath.h>

#include "../../Component/Base/component.h"

// 度からラジアンへの変換
#define DEGREE_TO_RADIAN(deg) ((deg) * M_PI / 180.0)

class GameObject
{
public:
	GameObject() {}//コンストラクタ
	virtual ~GameObject() {}//デストラクタ（仮想関数）


	DirectX::SimpleMath::Vector3 GetPosition() { return m_Position; }
	void SetPosition(DirectX::SimpleMath::Vector3 Position) { m_Position = Position; }
	DirectX::SimpleMath::Vector3* GetPositionAdress() { return &m_Position; }

	DirectX::SimpleMath::Vector3 GetRotation() { return m_Rotation; }
	void SetRotation(DirectX::SimpleMath::Vector3 Rotation) { m_Rotation = Rotation; }

	DirectX::SimpleMath::Vector3 GetScale() { return m_Scale; }
	void SetScale(DirectX::SimpleMath::Vector3 Scale) { m_Scale = Scale; }


	DirectX::SimpleMath::Vector3 GetForward()//前方向ベクトル取得
	{
		DirectX::SimpleMath::Matrix rot;
		rot = DirectX::SimpleMath::Matrix::CreateFromYawPitchRoll(m_Rotation.y, m_Rotation.x, m_Rotation.z);

		DirectX::SimpleMath::Vector3 forward;
		forward.x = rot._31;
		forward.y = rot._32;
		forward.z = rot._33;

		return forward;
	}

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

	template <typename T>
	T* AddComponent()
	{
		T* component = new T(this);
		m_Component.push_back(component);
		((Component*)component)->Init();

		return component;
	}

	template <typename T>
	T* GetComponent()
	{
		for (Component* component : m_Component)
		{
			if (typeid(*component) == typeid(T))
			{
				return (T*)component;
			}
		}
		return nullptr;
	}

	template <typename T>
	T* AddChild()
	{
		T* child = new T();
		m_ChildGameObject.push_back(child);
		child->InitBase();

		return child;
	}

	void InitBase()
	{
		Init();
	}

	void UninitBase()
	{
		Uninit();
		// ts 20230630（子供ゲームオブジェクトの終了処理忘れを追加）
		for (auto* childgo : m_ChildGameObject)
		{
			childgo->Uninit();

			for (Component* component : childgo->m_Component)
			{
				component->Uninit();
				delete component;
			}
			childgo->m_Component.clear();//リストのクリア

			delete childgo;
		}
		m_ChildGameObject.clear();//リストのクリア

		for (Component* component : m_Component)
		{
			component->Uninit();
			delete component;
		}
		m_Component.clear();//リストのクリア
	}

	void UpdateBase(uint64_t _deltaTime)
	{
		// デルタタイムを更新
		this->m_DeltaTime = _deltaTime;
		
		// コンポーネントを更新
		for (Component* component : m_Component)
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

		for (GameObject* child : m_ChildGameObject)
		{
			child->DrawBase(world);
		}


		Renderer::SetWorldMatrix(&world);

		for (Component* component : m_Component)
		{
			component->Draw();
		}

		Draw();
	}

protected:
	bool		m_Destroy = false;
	uint64_t	m_DeltaTime;

	DirectX::SimpleMath::Vector3	m_Position = DirectX::SimpleMath::Vector3(0.0f, 0.0f, 0.0f);
	DirectX::SimpleMath::Vector3	m_Rotation = DirectX::SimpleMath::Vector3(0.0f, 0.0f, 0.0f); // 回転　ラジアン
	DirectX::SimpleMath::Vector3	m_Scale = DirectX::SimpleMath::Vector3(1.0f, 1.0f, 1.0f);

	std::list<Component*> m_Component;

	std::list<GameObject*> m_ChildGameObject;
};