#pragma once
#include "Model.h"
#include "WorldTransform.h"
#include <numbers>
class Enemy {
public:
	/// <summary>
	/// 初期化処理
	/// </summary>
	void Initialize(Model* model, uint32_t textureHandle, ViewProjection* viewProjection, const Vector3& position);
	/// <summary>
	/// 更新
	/// </summary>
	void Update();
	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

private:
	WorldTransform worldTransform_;
	Model* model_ = nullptr;
	uint32_t textureHandle_ = 0u;
	ViewProjection* viewProjection_ = nullptr;
	static inline const float kWalkSpeed_ = 0.08f;
	static inline const float kWalkMotionAngleStart_ = -40.0f;
	static inline const float kWalkMotionAngleEnd_ = 40.0f;
	static inline const float kWalkMotionTime_ = 1.0f;
	float walkTimer_ = 0.0f;
	Vector3 velocity_ = {};
};
