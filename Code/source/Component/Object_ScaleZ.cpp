
#include "Component/Object_ScaleZ.h"

#include "Scene/Base/scene.h"
#include "System/manager.h"
#include "GameObject/bullet.h"
#include "GameObject/player.h"

void Object_ScaleZ::Init()
{
	if (this->m_GameObject->GetComponent<ModelRenderer>())
	{
		this->m_GameObject->GetComponent<ModelRenderer>()->Load("asset\\model\\box_z.obj");
	}
	this->m_ChangeSpeed = 0.05f;
}

void Object_ScaleZ::Update()
{
	// ���s���̃V�[������e�̃Q�[���I�u�W�F�N�g�����X�g�Ŏ擾
	Scene* scene = Manager::GetScene();
	std::vector<Bullet*> bulletList = scene->GetGameObjects<Bullet>();

	// �e�ƃI�u�W�F�N�g�̓����蔻��
	for (Bullet* bulletObj : bulletList)
	{
		Vector3 bulletPosition = bulletObj->GetPosition();

		Vector3 direction = m_GameObject->GetPosition() - bulletPosition;
		float length = direction.Length();

		// �������Ă���Ȃ�A�G�t�F�N�g���Đ����Ēe�̌��ʂ𔽉f
		if (length < 2.0f)
		{
			if (bulletObj->GetBulletKind() == CHANGE_Z_AXSIS)
			{
				m_SizeChange = true;
				m_BulletMode = bulletObj->GetBulletMode();
			}

			bulletObj->SetDestroy();
			return;
		}
	}

	// �T�C�Y��ύX����K�v�����邩�ǂ���
	if (m_SizeChange)
	{
		switch (m_BulletMode)
		{
		case SIZE_UP:
			SizeIncrease();
			break;

		case SIZE_DOWN:
			SizeDecrease();
			break;

		case NONE:
			break;
		}
	}
}

void Object_ScaleZ::SizeIncrease()
{
	// �T�C�Y���{����3�{�ɂȂ�܂ő傫������
	if (m_GameObject->GetScale().z < 3.0f)
	{
		m_GameObject->SetScale((m_GameObject->GetScale() += Vector3(0, 0, m_ChangeSpeed)));
	}
	else
	{
		m_SizeChange = false;
	}
}

void Object_ScaleZ::SizeDecrease()
{
	// �{���̃T�C�Y�ɂȂ�܂ŏ���������
	if (m_GameObject->GetScale().z > 1.0f)
	{
		m_GameObject->SetScale((m_GameObject->GetScale() -= Vector3(0, 0, m_ChangeSpeed)));
	}
	else
	{
		m_SizeChange = false;
	}
}