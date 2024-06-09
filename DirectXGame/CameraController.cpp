
#include "CameraController.h"
#include "Player.h"
#include <algorithm>

void CameraController::Initialize() { viewProjection_.Initialize(); }

void CameraController::Update() {

	const WorldTransform& targetWorldTransform = target_->GetWorldTransform();
	/*viewProjection_.translation_.x = targetWorldTransform.translation_.x + targetOffset_.x;
	viewProjection_.translation_.y = targetWorldTransform.translation_.y + targetOffset_.y;
	viewProjection_.translation_.z = targetWorldTransform.translation_.z + targetOffset_.z;*/

	targetCoordinates_.x = targetWorldTransform.translation_.x + targetOffset_.x + target_->GetVelocity().x * kVelocityBias;
	targetCoordinates_.y = targetWorldTransform.translation_.y + targetOffset_.y + target_->GetVelocity().y * kVelocityBias;
	targetCoordinates_.z = targetWorldTransform.translation_.z + targetOffset_.z + target_->GetVelocity().z * kVelocityBias;
	viewProjection_.translation_ = Leap(viewProjection_.translation_, targetCoordinates_, kInterpolationRate);

	viewProjection_.translation_.x = std::max(viewProjection_.translation_.x, target_->GetWorldTransform().translation_.x + Margin.left);
	viewProjection_.translation_.x = std::min(viewProjection_.translation_.x, target_->GetWorldTransform().translation_.x + Margin.right);
	viewProjection_.translation_.x = std::max(viewProjection_.translation_.x, target_->GetWorldTransform().translation_.y + Margin.bottom);
	viewProjection_.translation_.x = std::min(viewProjection_.translation_.x, target_->GetWorldTransform().translation_.y + Margin.top);


	viewProjection_.translation_.x = std::clamp(viewProjection_.translation_.x, movebleArea_.left, movebleArea_.right);
	viewProjection_.translation_.y = std::clamp(viewProjection_.translation_.y, movebleArea_.bottom, movebleArea_.top);

	viewProjection_.UpdateMatrix();
}

void CameraController::Reset() {
	const WorldTransform& targetWorldTransform = target_->GetWorldTransform();
	viewProjection_.translation_.x = targetWorldTransform.translation_.x + targetOffset_.x;
	viewProjection_.translation_.y = targetWorldTransform.translation_.y + targetOffset_.y;
	viewProjection_.translation_.z = targetWorldTransform.translation_.z + targetOffset_.z;
}
