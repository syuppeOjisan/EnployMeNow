#include "GameObject/Item_Z.h"
#include "Component/modelRenderer.h"

// ����������
void Item_Z::Init()
{
	// ���f���ǂݍ���
	AddComponent<ModelRenderer>()->Load("asset\\model\\Item_Z.obj");
}
