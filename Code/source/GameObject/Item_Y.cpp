#include "GameObject/Item_Y.h"
#include "Component/modelRenderer.h"

// ����������
void Item_Y::Init()
{
	// ���f���ǂݍ���
	AddComponent<ModelRenderer>()->Load("asset\\model\\Item_Y.obj");
}
