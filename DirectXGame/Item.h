#pragma once
#include "Model.h"
#include "WorldTransform.h"
#include<numbers>

class Player;
class Item {
public:
	
	/// <summary>
	/// 初期化処理
	/// </summary>
	void Initialize(Model* model, ViewProjection* viewProjection, const Vector3& position);
	/// <summary>
	/// 更新
	/// </summary>
	void Update();
	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

	AABB GetAABB();
	void OnCollosion(const Player* player);
	Vector3 GetWorldPosition();
	bool IsGet() const { return isGet_; }
	private:
	WorldTransform worldTransform_;
	Model* model_ = nullptr;
	ViewProjection* viewProjection_ = nullptr;

	// キャラクターの当たり判定サイズ
	static inline const float kWidth = 1.8f;
	static inline const float kHeight = 1.8f;

	bool isGet_ = false;
};
