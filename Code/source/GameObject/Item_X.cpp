#include "GameObject/Item_X.h"
#include "Component/modelRenderer.h"

// ����������
void Item_X::Init()
{
	// ���f���ǂݍ���
	AddComponent<ModelRenderer>()->Load("asset\\model\\Item_X.obj");
}
