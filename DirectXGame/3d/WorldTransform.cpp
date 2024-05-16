#include "WorldTransform.h"

void WorldTransform::UpdateMatrix() { matWorld_ = MakeAffineMatrix(scale_, translation_, rotation_);
	TransferMatrix();
}