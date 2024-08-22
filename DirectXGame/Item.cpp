#include "Item.h"
#define NOMINMAX
#include "MapChipField.h"
#include <Input.h>
#include <algorithm>
#include <cassert>
void Item::Initialize(Model* model, ViewProjection* viewProjection, const Vector3& position) {

	assert(model);
	model_ = model;
	viewProjection_ = viewProjection;

	worldTransform_.Initialize();
	worldTransform_.translation_ = position;
	worldTransform_.rotation_.y = std::numbers::pi_v<float> / 2.0f;
}

void Item::Update() { 
	worldTransform_.TransferMatrix();
	worldTransform_.UpdateMatrix();
}

void Item::Draw() { 
		model_->Draw(worldTransform_, *viewProjection_);
	
}

Vector3 Item::GetWorldPosition() {
	Vector3 worldPos{};
	worldPos.x = worldTransform_.matWorld_.m[3][0];
	worldPos.y = worldTransform_.matWorld_.m[3][1];
	worldPos.z = worldTransform_.matWorld_.m[3][2];

	return worldPos;
}

AABB Item::GetAABB() { 
	Vector3 worldPos = GetWorldPosition();
	AABB aabb{};

	aabb.min = {worldPos.x - kWidth / 2.0f, worldPos.y - kHeight / 2.0f, worldPos.z - kWidth / 2.0f};
	aabb.max = {worldPos.x + kWidth / 2.0f, worldPos.y + kHeight / 2.0f, worldPos.z + kWidth / 2.0f};
	return aabb;  }

void Item::OnCollosion(const Player* player) {
	(void)player;
	isGet_ = true;
}


