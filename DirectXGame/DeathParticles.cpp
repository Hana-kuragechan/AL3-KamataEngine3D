#include "DeathParticles.h"
#include <Model.h>
#include <algorithm>
#include <cassert>

void DeathParticles::Initialize(Model* model, ViewProjection* viewProjection, const Vector3& position) {
	assert(model);
	model_ = model;
	viewProjection_ = viewProjection;
	for (auto& worldTransform : worldTransforms_) {
		worldTransform.Initialize();
		worldTransform.translation_ = position;
	}
	objectColor_.Initialize();
	color_ = {1, 1, 1, 1};
}

void DeathParticles::Update() { 
	
	for (uint32_t i = 0; i < kNumParticles; ++i) {
		Vector3 velocity = {kSpeed, 0, 0};
		float angle = kAngleUnit * i;
		Matrix4x4 matrixRotation = MakeRotateZMatrix(angle);
		velocity = Transform(velocity, matrixRotation);
		worldTransforms_[i].translation_ += velocity;
	}
	if (isFinished_) {
		return;
	}
	counter_ += 1.0f / 60.0f;
	if (counter_ >= kDuration) {
		counter_ = kDuration;
		isFinished_ = true;
	}
	
	color_.w = std::clamp(1.0f - counter_, 0.0f, 1.0f);

	/*color_.w -= 0.01f;
	color_.w = std::clamp(color_.w, 0.0f, 1.0f);*/
	objectColor_.SetColor(color_);
	objectColor_.TransferMatrix();

	for (auto& worldTransform : worldTransforms_) {
		worldTransform.UpdateMatrix();
	}
	

}

void DeathParticles::Draw() {
	if (isFinished_) {
		return;
	}
	for (auto& worldTransform : worldTransforms_) {
		model_->Draw(worldTransform,*viewProjection_,&objectColor_);
	}

}
