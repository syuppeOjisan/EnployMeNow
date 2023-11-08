#pragma once

#include <list>
#include <array>
#include <vector>
#include <typeinfo>
#include "../../GameObject/Base/gameObject.h"
#include "../../Component/modelRenderer.h"

class Scene
{
protected:
	std::array<std::list<GameObject*>, 4>	m_GameObject;	//STLのリスト構造

public:
	Scene() {}
	virtual ~Scene() {}

	virtual void Init() {}
	virtual void Uninit() {}
	virtual void Update() {}
	virtual void Draw() {}

	virtual void HitCheck(){}	// 当たり判定実行用仮想関数

	void InitBase()
	{
		Init();
	}

	void UninitBase()
	{

		for (auto& objectList : m_GameObject)
		{
			for (GameObject* object : objectList)
			{
				object->UninitBase();
				delete object;
			}
			objectList.clear();		//リストのクリア
		}

		Uninit();					// シーンの終了処理

		ModelRenderer::UnloadAll();		// モデルを全てアンロード
	}

	void UpdateBase(uint64_t _deltaTime)
	{
		for (auto& objectList : m_GameObject)
		{
			for (GameObject* object : objectList)//範囲forループ
			{
				object->UpdateBase(_deltaTime);			// オブジェクトの更新処理
			}

			objectList.remove_if([](GameObject* object) {return object->Destroy(); });
			//ラムダ式
		}

		Update();		// シーンの更新処理
	}

	void DrawBase()
	{
		DirectX::SimpleMath::Matrix matrix;
		matrix = DirectX::SimpleMath::Matrix::Identity;

		for (auto& objectList : m_GameObject)
		{
			for (GameObject* object : objectList)
			{
				object->DrawBase(matrix);				// オブジェクトの描画処理
			}
		}

		Draw();											// シーンの描画処理
	}

	template <typename T> //テンプレート関数
	T* AddGameObject(int Layer)
	{
		T* gameObject = new T();
		m_GameObject[Layer].push_back(gameObject);
		gameObject->Init();

		return gameObject;
	}

	template <typename T>
	T* GetGameObject()
	{
		for (auto& objectList : m_GameObject)
		{
			for (GameObject* object : objectList)
			{
				if (typeid(*object) == typeid(T))//型を調べる(RTTI動的型情報)
				{
					return (T*)object;
				}
			}
		}
		return nullptr;
	}

	template <typename T>
	std::vector<T*> GetGameObjects()
	{
		std::vector<T*> objects;//STLの配列

		for (auto& objectList : m_GameObject)
		{
			for (GameObject* object : objectList)
			{
				if (typeid(*object) == typeid(T))
				{
					objects.push_back((T*)object);
				}
			}
		}
		return objects;
	}
};