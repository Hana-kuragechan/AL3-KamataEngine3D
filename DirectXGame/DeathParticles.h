#pragma once
#include "WorldTransform.h"
#include<array>
#include"ObjectColor.h"

class Model;
class ViewProjection;

class DeathParticles {

	public:
	void Initialize(Model* model, ViewProjection* viewProjection, const Vector3& position);
	void Update();
	void Draw();

private:
	
	Model* model_ = nullptr;
	ViewProjection* viewProjection_ = nullptr;
	static inline const uint32_t kNumParticles = 8;
	std::array<WorldTransform, kNumParticles> worldTransforms_;

	static inline const float kDuration = 2.0f;
	static inline const float kSpeed = 0.1f;
	static inline const float kAngleUnit = 2.0f * 3.14f / kNumParticles;
	bool isFinished_ = false;
	float counter_ = 0.0f;

	ObjectColor objectColor_;
	Vector4 color_;
};
