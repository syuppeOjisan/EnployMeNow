#include "GameObject/Item_Y.h"
#include "Component/modelRenderer.h"

// 初期化処理
void Item_Y::Init()
{
	// モデル読み込み
	AddComponent<ModelRenderer>()->Load("asset\\model\\Item_Y.obj");
}
