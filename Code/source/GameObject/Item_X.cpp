#include "GameObject/Item_X.h"
#include "Component/modelRenderer.h"

// 初期化処理
void Item_X::Init()
{
	// モデル読み込み
	AddComponent<ModelRenderer>()->Load("asset\\model\\Item_X.obj");
}
