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
	GameObject() {}//�R���X�g���N�^
	virtual ~GameObject() {}//�f�X�g���N�^�i���z�֐��j

	/// <summary>
	/// ���W���擾
	/// </summary>
	/// <returns>���W</returns>
	DirectX::SimpleMath::Vector3 GetPosition() { return m_Position; }

	/// <summary>
	/// ���W���Z�b�g
	/// </summary>
	/// <param name="Position">�Z�b�g���������W</param>
	void SetPosition(DirectX::SimpleMath::Vector3 Position) { m_Position = Position; }

	/// <summary>
	/// �ʒu�ϐ��̃A�h���X���擾
	/// </summary>
	/// <returns>���W�ϐ��̃A�h���X</returns>
	DirectX::SimpleMath::Vector3* GetPositionAdress() { return &m_Position; }

	/// <summary>
	/// ���݂̉�]�p���擾(���W�A��)
	/// </summary>
	/// <returns>���݂̉�]�p</returns>
	DirectX::SimpleMath::Vector3 GetRotation() { return m_Rotation; }

	/// <summary>
	/// ��]�p���Z�b�g(���W�A��)
	/// </summary>
	/// <param name="Rotation">�Z�b�g��������]�p</param>
	void SetRotation(DirectX::SimpleMath::Vector3 Rotation) 
	{ 
		// ���W�A����͈͓��Ɏ��߂邽�߂ɕ␳
		Rotation.x = FixRadian(Rotation.x);
		Rotation.y = FixRadian(Rotation.y);
		Rotation.z = FixRadian(Rotation.z);

		// ��]�p�Ƃ��Ĕ��f
		m_Rotation = Rotation;
	}

	/// <summary>
	/// �X�P�[�����擾
	/// </summary>
	/// <returns>���݂̃X�P�[��</returns>
	DirectX::SimpleMath::Vector3 GetScale() { return m_Scale; }

	/// <summary>
	/// �X�P�[�����Z�b�g
	/// </summary>
	/// <param name="Scale">�Z�b�g�������X�P�[��</param>
	void SetScale(DirectX::SimpleMath::Vector3 Scale) { m_Scale = Scale; }



	/// <summary>
	/// ���̓C���^�[�t�F�[�X���Z�b�g
	/// </summary>
	/// <param name="_input">���̓C���^�[�t�F�[�X</param>
	void SetInput(InputIntarface* _input) { m_pInput = _input; m_pInput->Init(); }

	/// <summary>
	/// �I�u�W�F�N�g�̌������x�N�g���̕��Ɍ�����
	/// </summary>
	/// <param name="_vector">�����Ăق��������x�N�g��</param>
	void SetRotateToVector(Vector3 _vector)
	{
		// �x�N�g���������Ă���p�x�����߂�
		float rotateRadianDestination = atan2f(_vector.x, _vector.z);
		rotateRadianDestination = FixRadian(rotateRadianDestination);	// ���W�A���␳

		// ���݂̉�]�p�Ƃ̊p�x�������߂�
		float rotateRadian = rotateRadianDestination - GetRotation().y;
		rotateRadian = FixRadian(rotateRadian);	// ���W�A���␳

		// ���݂̉�]�p�Ƃ̍������߂ĉ�]��̒l�����߂�
		Vector3 pRotate = GetRotation();
		pRotate.y += rotateRadian;
		pRotate.y = FixRadian(pRotate.y);

		// ��]��K�p
		SetRotation(Vector3(0, pRotate.y, 0));
	}

	/// <summary>
	/// �I�u�W�F�N�g�̌������x�N�g���̕����ɂ�������]������
	/// </summary>
	/// <param name="_vector">�����Ăق��������x�N�g��</param>
	void SetRotateToVectorEaseIn(Vector3 _vector)
	{
		// �x�N�g���������Ă���p�x�����߂�
		float rotateRadian = atan2f(_vector.x, _vector.z);
		rotateRadian = FixRadian(rotateRadian);	// ���W�A���␳

		// ���݂̉�]�p�Ƃ̊p�x�������߂�
		float rotateRadianDestination = rotateRadian - GetRotation().y;
		rotateRadianDestination = FixRadian(rotateRadianDestination);	// ���W�A���␳

		// ��]��̒l�����߂� ��������]
		float easeInFactor = 0.5f;// ��]�̃X�s�[�h
		Vector3 pRotate = GetRotation();
		pRotate.y += rotateRadianDestination * easeInFactor;
		pRotate.y = FixRadian(pRotate.y);

		// ��]��K�p
		SetRotation(Vector3(0, pRotate.y, 0));
	}

	/// <summary>
	/// �O���x�N�g�����擾
	/// </summary>
	/// <returns>�O���x�N�g��</returns>
	DirectX::SimpleMath::Vector3 GetForward()
	{
		// ��]�s����쐬
		DirectX::SimpleMath::Matrix rot;
		rot = DirectX::SimpleMath::Matrix::CreateFromYawPitchRoll(m_Rotation.y, m_Rotation.x, m_Rotation.z);

		// ��]�s���3��ڂɂ���O���x�N�g�����擾
		DirectX::SimpleMath::Vector3 forward;
		forward.x = rot._31;
		forward.y = rot._32;
		forward.z = rot._33;

		return forward;
	}

	/// <summary>
	/// ���W�A���̒l��␳����
	/// </summary>
	/// <param name="_radian">��]�p�x���W�A��</param>
	/// <returns>�␳��̒l</returns>
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
	/// �f�X�g���C�t���O��True�ɂ���
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

	// �R���|�[�l���g�ǉ�
	template <typename T>
	T* AddComponent()
	{
		std::unique_ptr<T> component = std::make_unique<T>(this);	// ���j�[�N�|�C���^�Ƃ��ăR���|�[�l���g�𐶐�
		m_Component.push_back(std::move(component));				// ���L�������X�g�Ɉڍs���Ēǉ�
		m_Component.back()->Init();									// ���X�g���珉�������Ăяo��

		return dynamic_cast<T*>(m_Component.back().get());			// ���X�g����R���|�[�l���g�̐��|�����o���ĕԂ��A�ꉞdynamic_cast
	}

	// �R���|�[�l���g�擾
	template <typename T>
	T* GetComponent()
	{
		for (auto& component : m_Component)
		{
			T* castedComponent = dynamic_cast<T*>(component.get());	// ���X�g����R���|�[�l���g���擾����T�^�ɃL���X�g
			if (castedComponent)									
			{
				return castedComponent;								// �L���X�g�����������炻�̃R���|�[�l���g��Ԃ�
			}
		}
		return nullptr;												// �L���X�g���������Ȃ�������nullptr��Ԃ�
	}

	// �q�I�u�W�F�N�g��ǉ�
	template <typename T>
	T* AddChild()
	{
		std::unique_ptr<T> childObj = std::make_unique<T>();		// ���j�[�N�|�C���^�Ƃ��Ďq�I�u�W�F�N�g�𐶐�
		m_ChildGameObject.push_back(std::move(childObj));			// ���L�������X�g�Ɉڍs���Ēǉ�
		m_ChildGameObject.back()->Init();							// ���X�g���珉�������Ăяo��

		return dynamic_cast<T*>(m_ChildGameObject.back().get());	// ���X�g����q�I�u�W�F�N�g�̐��|�����o���ĕԂ�
	}

	void InitBase()
	{
		Init();
	}

	void UninitBase()
	{
		Uninit();
		
		// �q�I�u�W�F�N�g�֘A�̏I������
		for (auto& childObj : m_ChildGameObject)
		{
			childObj->Uninit();				// �q�I�u�W�F�N�g�̏I������

			for (auto& component : childObj->m_Component)
			{
				component->Uninit();		// �q�I�u�W�F�N�g�������Ă���R���|�[�l���g�̏I������
			}
			childObj->m_Component.clear();	// �q�I�u�W�F�N�g�̃R���|�[�l���g���X�g���N���A

		}
		m_ChildGameObject.clear();			//�q�I�u�W�F�N�g�̃��X�g���N���A

		// ���̃I�u�W�F�N�g�̏I������
		for (auto& component : m_Component)
		{
			component->Uninit();			// �R���|�[�l���g�̏I������s
		}
		m_Component.clear();				// �R���|�[�l���g���X�g�̃N���A
	}

	void UpdateBase(uint64_t _deltaTime)
	{
		// �f���^�^�C�����X�V
		this->m_DeltaTime = _deltaTime;
		
		// �R���|�[�l���g���X�V
		for (auto& component : m_Component)
		{
			component->Update();
		}

		// �X�V����
		Update();
	}

	void DrawBase(DirectX::SimpleMath::Matrix ParentMatrix)
	{
		// �}�g���N�X�ݒ�
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
	DirectX::SimpleMath::Vector3	m_Rotation = DirectX::SimpleMath::Vector3(0.0f, 0.0f, 0.0f); // ��]
	DirectX::SimpleMath::Vector3	m_Scale = DirectX::SimpleMath::Vector3(1.0f, 1.0f, 1.0f);

	std::list<std::unique_ptr<Component>> m_Component;

	std::list<std::unique_ptr<GameObject>> m_ChildGameObject;

	InputIntarface* m_pInput;	// ���̓C���^�[�t�F�[�X
};