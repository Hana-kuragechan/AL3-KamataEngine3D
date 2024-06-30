#pragma once
#include "Model.h"
#include "WorldTransform.h"
#include <numbers>
class Player;
class Enemy {
public:
	/// <summary>
	/// 初期化処理
	/// </summary>
	void Initialize(Model* model,  ViewProjection* viewProjection, const Vector3& position);
	/// <summary>
	/// 更新
	/// </summary>
	void Update();
	/// <summary>
	/// 描画
	/// </summary>
	void Draw();
	Vector3 GetWorldPosition();
	AABB GetAABB();
	void OnCollosion(const Player* player);

private:
	WorldTransform worldTransform_;
	Model* model_ = nullptr;
	ViewProjection* viewProjection_ = nullptr;
	static inline const float kWalkSpeed_ = 0.08f;
	static inline const float kWalkMotionAngleStart_ = -40.0f;
	static inline const float kWalkMotionAngleEnd_ = 40.0f;
	static inline const float kWalkMotionTime_ = 1.0f;
	float walkTimer_ = 0.0f;
	Vector3 velocity_ = {};
	// キャラクターの当たり判定サイズ
	static inline const float kWidth = 1.8f;
	static inline const float kHeight = 1.8f;
};
