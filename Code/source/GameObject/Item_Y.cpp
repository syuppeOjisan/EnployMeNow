#include "GameObject/Item_Y.h"
#include "Component/modelRenderer.h"

// ‰Šú‰»ˆ—
void Item_Y::Init()
{
	// ƒ‚ƒfƒ‹“Ç‚İ‚İ
	AddComponent<ModelRenderer>()->Load("asset\\model\\Item_Y.obj");
}
