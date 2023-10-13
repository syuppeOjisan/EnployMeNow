#include <SimpleMath.h>
using namespace::DirectX::SimpleMath;

#include "GameObject/ItemBase.h"

void ItemBase::Update()
{
	// ƒAƒCƒeƒ€‚ð‰ñ“]‚³‚¹‚é
	Vector3 rotation{ 0,0,0 };
	Matrix mxRotOffset = Matrix::CreateRotationY(DirectX::XMConvertToRadians(3.0f));
	rotation = Vector3::Transform(GetRotation(), mxRotOffset);
	this->SetRotation(rotation);
}
