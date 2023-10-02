
#include "Component/Object_SizeChange.h"

#include "Scene/Base/scene.h"
#include "System/manager.h"
#include "GameObject/bullet.h"

void Object_SizeChange::Init()
{
	this->bulletMode = NONE;
	this->changeSpeed = Vector3(0.05f, 0.05f, 0.05f);
}

void Object_SizeChange::Update()
{
	// ���s���̃V�[������e�̃Q�[���I�u�W�F�N�g�����X�g�Ŏ擾
	Scene* scene = Manager::GetScene();
	std::vector<Bullet*> bulletList = scene->GetGameObjects<Bullet>();

	// �e�ƃI�u�W�F�N�g�̓����蔻��
	for (Bullet* Bullet : bulletList)
	{
		Vector3 bulletPosition = Bullet->GetPosition();

		Vector3 direction = m_GameObject->GetPosition() - bulletPosition;
		float length = direction.Length();

		// �������Ă���Ȃ�A�G�t�F�N�g���Đ����Ēe�̌��ʂ𔽉f
		if (length < 2.0f)
		{
			{
				sizeChange = true;
				bulletMode = Bullet->GetBulletMode();
			}


			Bullet->SetDestroy();
			return;
		}
	}

	// �T�C�Y��ύX����K�v�����邩�ǂ���
	if (sizeChange)
	{
		switch (bulletMode)
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

void Object_SizeChange::SizeIncrease()
{
	// �T�C�Y���{����2�{�ɂȂ�܂ő傫������
	if (m_GameObject->GetScale().x < 5.0f)
	{
		m_GameObject->SetScale((m_GameObject->GetScale() += changeSpeed));
	}
	else
	{
		sizeChange = false;
	}
}

void Object_SizeChange::SizeDecrease()
{
	// �{���̃T�C�Y�ɂȂ�܂ŏ���������
	if (m_GameObject->GetScale().x > 1.0f)
	{
		m_GameObject->SetScale((m_GameObject->GetScale() -= changeSpeed));
	}
	else
	{
		sizeChange = false;
	}
}