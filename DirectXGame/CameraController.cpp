#define NOMINMAX
#include "CameraController.h"
#include "Player.h"
#include <algorithm>

void CameraController::Initialize() { viewProjection_.Initialize(); }

void CameraController::Update() {

	const WorldTransform& targetWorldTransform = target_->GetWorldTransform();
	/*viewProjection_.translation_.x = targetWorldTransform.translation_.x + targetOffset_.x;
	viewProjection_.translation_.y = targetWorldTransform.translation_.y + targetOffset_.y;
	viewProjection_.translation_.z = targetWorldTransform.translation_.z + targetOffset_.z;*/
	
	goal_.x = targetWorldTransform.translation_.x + targetOffset_.x;
    goal_.y = targetWorldTransform.translation_.y + targetOffset_.y;
    goal_.z = targetWorldTransform.translation_.z + targetOffset_.z;
	viewProjection_.translation_ = Leap(viewProjection_.translation_, goal_, kInterpolationRate);

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
