
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
	// 実行中のシーンから弾のゲームオブジェクトをリストで取得
	Scene* scene = Manager::GetScene();
	std::vector<Bullet*> bulletList = scene->GetGameObjects<Bullet>();

	// 弾とオブジェクトの当たり判定
	for (Bullet* bulletObj : bulletList)
	{
		Vector3 bulletPosition = bulletObj->GetPosition();

		Vector3 direction = m_GameObject->GetPosition() - bulletPosition;
		float length = direction.Length();

		// 当たっているなら、エフェクトを再生して弾の効果を反映
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

	// サイズを変更する必要があるかどうか
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
	// サイズが本来の3倍になるまで大きくする
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
	// 本来のサイズになるまで小さくする
	if (m_GameObject->GetScale().z > 1.0f)
	{
		m_GameObject->SetScale((m_GameObject->GetScale() -= Vector3(0, 0, m_ChangeSpeed)));
	}
	else
	{
		m_SizeChange = false;
	}
}