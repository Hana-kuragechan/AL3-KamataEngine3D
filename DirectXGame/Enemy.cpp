#include "Enemy.h"


void Enemy::Initialize(Model* model, ViewProjection* viewProjection, const Vector3& position) 
{
	assert(model);
	model_ = model;
	viewProjection_ = viewProjection;

	worldTransform_.Initialize();
	worldTransform_.translation_ = position;
	worldTransform_.rotation_.y = std::numbers::pi_v<float> * 3.0f / 2.0f;
	velocity_ = {-kWalkSpeed_, 0, 0};
	walkTimer_ = 0.0f;
}

void Enemy::Update() { 
	worldTransform_.TransferMatrix(); 
	//移動
	worldTransform_.translation_ = worldTransform_.translation_ + velocity_;
	//上下に動かす
	walkTimer_ += 1.0f / 60.0f;
	float param = std::sin((2 * 3.14f) * walkTimer_ / kWalkMotionTime_);
	float radian = kWalkMotionAngleStart_ + kWalkMotionAngleEnd_ * (param + 1.0f) / 2.0f;
	worldTransform_.rotation_.x = radian * 3.14f / 360.0f;
	// 行列計算
	worldTransform_.UpdateMatrix();
}

void Enemy::Draw() { model_->Draw(worldTransform_, *viewProjection_); }

Vector3 Enemy::GetWorldPosition() { 
	
	Vector3 worldPos;
	worldPos.x = worldTransform_.matWorld_.m[3][0];
	worldPos.y = worldTransform_.matWorld_.m[3][1];
	worldPos.z = worldTransform_.matWorld_.m[3][2];

	return worldPos;
}

AABB Enemy::GetAABB() { 
	
	Vector3 worldPos = GetWorldPosition();
	AABB aabb;

	aabb.min = {worldPos.x - kWidth / 2.0f, worldPos.y - kHeight / 2.0f, worldPos.z - kWidth / 2.0f};
	aabb.max = {worldPos.x + kWidth / 2.0f, worldPos.y + kHeight / 2.0f, worldPos.z + kWidth / 2.0f};
	return aabb;
}

void Enemy::OnCollosion(const Player* player) { (void)player; }
