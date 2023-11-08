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
	std::array<std::list<GameObject*>, 4>	m_GameObject;	//STL�̃��X�g�\��

public:
	Scene() {}
	virtual ~Scene() {}

	virtual void Init() {}
	virtual void Uninit() {}
	virtual void Update() {}
	virtual void Draw() {}

	virtual void HitCheck(){}	// �����蔻����s�p���z�֐�

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
			objectList.clear();		//���X�g�̃N���A
		}

		Uninit();					// �V�[���̏I������

		ModelRenderer::UnloadAll();		// ���f����S�ăA�����[�h
	}

	void UpdateBase(uint64_t _deltaTime)
	{
		for (auto& objectList : m_GameObject)
		{
			for (GameObject* object : objectList)//�͈�for���[�v
			{
				object->UpdateBase(_deltaTime);			// �I�u�W�F�N�g�̍X�V����
			}

			objectList.remove_if([](GameObject* object) {return object->Destroy(); });
			//�����_��
		}

		Update();		// �V�[���̍X�V����
	}

	void DrawBase()
	{
		DirectX::SimpleMath::Matrix matrix;
		matrix = DirectX::SimpleMath::Matrix::Identity;

		for (auto& objectList : m_GameObject)
		{
			for (GameObject* object : objectList)
			{
				object->DrawBase(matrix);				// �I�u�W�F�N�g�̕`�揈��
			}
		}

		Draw();											// �V�[���̕`�揈��
	}

	template <typename T> //�e���v���[�g�֐�
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
				if (typeid(*object) == typeid(T))//�^�𒲂ׂ�(RTTI���I�^���)
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
		std::vector<T*> objects;//STL�̔z��

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