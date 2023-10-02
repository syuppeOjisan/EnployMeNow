
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
	// 実行中のシーンから弾のゲームオブジェクトをリストで取得
	Scene* scene = Manager::GetScene();
	std::vector<Bullet*> bulletList = scene->GetGameObjects<Bullet>();

	// 弾とオブジェクトの当たり判定
	for (Bullet* Bullet : bulletList)
	{
		Vector3 bulletPosition = Bullet->GetPosition();

		Vector3 direction = m_GameObject->GetPosition() - bulletPosition;
		float length = direction.Length();

		// 当たっているなら、エフェクトを再生して弾の効果を反映
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

	// サイズを変更する必要があるかどうか
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
	// サイズが本来の2倍になるまで大きくする
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
	// 本来のサイズになるまで小さくする
	if (m_GameObject->GetScale().x > 1.0f)
	{
		m_GameObject->SetScale((m_GameObject->GetScale() -= changeSpeed));
	}
	else
	{
		sizeChange = false;
	}
}