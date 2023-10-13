#include "GameObject/Item_Z.h"
#include "Component/modelRenderer.h"

// ‰Šú‰»ˆ—
void Item_Z::Init()
{
	// ƒ‚ƒfƒ‹“Ç‚İ‚İ
	AddComponent<ModelRenderer>()->Load("asset\\model\\Item_Z.obj");
}
