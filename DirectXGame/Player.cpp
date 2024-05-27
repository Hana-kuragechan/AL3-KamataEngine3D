#include "Player.h"
#include <cassert>
#include <Input.h>
#include<algorithm>
void Player::Initialize(Model* model, uint32_t textureHandle, ViewProjection* viewProjection, const Vector3& position) {
	assert(model);
	model_ = model;
	textureHandle_ = textureHandle;
	worldTransform_.Initialize();
	worldTransform_.translation_=position;
	viewProjection_ = viewProjection;
	/*worldTransform_.rotation_.y = std::numbers::pi_v<float> / 2.0f;*/

}

void Player::Update() { 
	worldTransform_.TransferMatrix(); 
	if (Input::GetInstance()->PushKey(DIK_RIGHT)||
		Input::GetInstance()->PushKey(DIK_LEFT)) {
		Vector3 acceleration = {};

		if (Input::GetInstance()->PushKey(DIK_RIGHT)) {
			if (velocity_.x < 0.0f) {
				velocity_.x *= (1.0f - kAttenuation);
			}
			acceleration.x += kAcceleration;
		} else if (Input::GetInstance()->PushKey(DIK_LEFT)) {
			if (velocity_.x > 0.0f) {
				velocity_.x *= (1.0f - kAttenuation);
			}
			acceleration.x -= kAcceleration;
		}
		velocity_.x += acceleration.x;
		velocity_.x = std::clamp(velocity_.x, -kLimitRunSpeed, kLimitRunSpeed);
	} 
	else {
		velocity_.x *= (1.0f - kAttenuation);
		
	}
	worldTransform_.translation_.x += velocity_.x;
	worldTransform_.UpdateMatrix();
}

void Player::Draw() { model_->Draw(worldTransform_, *viewProjection_, textureHandle_); }