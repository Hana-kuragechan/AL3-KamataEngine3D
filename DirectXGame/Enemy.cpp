#include "Enemy.h"

void Enemy::Initialize(Model* model, uint32_t textureHandle, ViewProjection* viewProjection, const Vector3& position) {
	assert(model);
	model_ = model;
	textureHandle_ = textureHandle;
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

void Enemy::Draw() { model_->Draw(worldTransform_, *viewProjection_, textureHandle_); }
