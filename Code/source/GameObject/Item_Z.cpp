#include "GameObject/Item_Z.h"
#include "Component/modelRenderer.h"

// 初期化処理
void Item_Z::Init()
{
	// モデル読み込み
	AddComponent<ModelRenderer>()->Load("asset\\model\\Item_Z.obj");
}
