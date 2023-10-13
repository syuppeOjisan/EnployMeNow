#include "GameObject/Item_X.h"
#include "Component/modelRenderer.h"

// ‰Šú‰»ˆ—
void Item_X::Init()
{
	// ƒ‚ƒfƒ‹“Ç‚İ‚İ
	AddComponent<ModelRenderer>()->Load("asset\\model\\Item_X.obj");
}
