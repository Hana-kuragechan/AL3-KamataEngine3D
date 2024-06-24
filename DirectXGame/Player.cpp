#define NOMINMAX
#include "Player.h"
#include "MapChipField.h"
#include <Input.h>
#include <algorithm>
#include <cassert>

void Player::Initialize(Model* model, uint32_t textureHandle, ViewProjection* viewProjection, const Vector3& position) {
	assert(model);
	model_ = model;
	textureHandle_ = textureHandle;
	viewProjection_ = viewProjection;

	worldTransform_.Initialize();
	worldTransform_.translation_ = position;
	worldTransform_.rotation_.y = std::numbers::pi_v<float> / 2.0f;
}

void Player::Update() {

	worldTransform_.TransferMatrix();

	if (onGround_) {

		// 移動入力
		// 左右移動操作
		if (Input::GetInstance()->PushKey(DIK_RIGHT) || Input::GetInstance()->PushKey(DIK_LEFT)) {

			// 左右加速
			Vector3 acceleration = {};
			if (Input::GetInstance()->PushKey(DIK_RIGHT)) {
				// 左移動中の右入力
				if (velocity_.x < 0.0f) {
					// 速度と逆方向に入力中は急ブレーキ
					velocity_.x *= (1.0f - kAttenuation);
				}
				if (lrDirection_ != LRDirection::kRight) {
					lrDirection_ = LRDirection::kRight;
					turnFirstRotationY_ = worldTransform_.rotation_.y;
					turnTimer_ = 1.0f;
				}
				acceleration.x += kAcceleration;
			} else if (Input::GetInstance()->PushKey(DIK_LEFT)) {
				// 右移動中の左入力
				if (velocity_.x > 0.0f) {
					// 速度と逆方向に入力中は急ブレーキ
					velocity_.x *= (1.0f - kAttenuation);
				}
				if (lrDirection_ != LRDirection::kLeft) {
					lrDirection_ = LRDirection::kLeft;
					turnFirstRotationY_ = worldTransform_.rotation_.y;
					turnTimer_ = 1.0f;
				}
				acceleration.x -= kAcceleration;
			}
			// 加速/減速
			velocity_.x += acceleration.x;
			// 最大速度制限
			velocity_.x = std::clamp(velocity_.x, -kLimitRunSpeed, kLimitRunSpeed);
		} else {
			// 非入力時は移動減衰をかける
			velocity_.x *= (1.0f - kAttenuation);
		}

		if (Input::GetInstance()->PushKey(DIK_UP)) {
			// ジャンプ初速
			velocity_ = Vector3Add(velocity_, Vector3(0, kJumpAcceleration, 0));
		}

	} else {
		// 落下速度
		velocity_ = Vector3Add(velocity_, Vector3(0, -kGravityAcceleration, 0));
		// 落下速度制限
		velocity_.y = std::max(velocity_.y, -kLimitFallSpeed);
	}

	// 移動量を加味して衝突判定
	// 判定結果を反映して移動させる
	// 天井に接触している場合の処理
	// 壁に接触している場合の処理

	// 設置状態の切り替え
	// 着地フラグ
	bool landing = false;
	// 地面との当たり判定
	if (velocity_.y < 0) {
		// Y座標が地面以下になったら着地
		if (worldTransform_.translation_.y <= 2.0f) {
			landing = true;
		}
	}

	if (onGround_) {
		if (velocity_.y > 0.0f) {
			onGround_ = false;
		}
	} else {
		if (landing) {
			worldTransform_.translation_.y = 2.0f;
			velocity_.x *= (1.0f - kAttenuation);
			velocity_.y = 0.0f;
			onGround_ = true;
		}
	}

	CollisionMapInfo info;
	info.move = velocity_;
	ColisionMap(info);
	SwitchingOnGround(info);
	velocity_ = info.move;

	worldTransform_.translation_.x += velocity_.x;
	worldTransform_.translation_.y += velocity_.y;
	worldTransform_.translation_.z += velocity_.z;

	// 旋回制御
	if (turnTimer_ > 0.0f) {
		turnTimer_ -= 1 / 60;
		// 左右の自キャラ角度テーブル
		float destinationRotationYTable[]{std::numbers::pi_v<float> / 2.0f, std::numbers::pi_v<float> * 3.0f / 2.0f};
		// 状況に応じた角度を取得
		float destinationRotationY = destinationRotationYTable[static_cast<uint32_t>(lrDirection_)];
		// 自キャラの角度を設定
		worldTransform_.rotation_.y = float((1.0 - turnTimer_) * destinationRotationY + turnTimer_ * turnFirstRotationY_);
	}

	// 行列計算
	worldTransform_.UpdateMatrix();
}

void Player::Draw() { model_->Draw(worldTransform_, *viewProjection_, textureHandle_); }

void Player::MoveResult(CollisionMapInfo& info) {

	worldTransform_.translation_.x += info.move.x;
	worldTransform_.translation_.y += info.move.y;
	worldTransform_.translation_.z += info.move.z;
}

void Player::CeilingCollision(const CollisionMapInfo& info) {
	if (info.hitCeiling) {
		velocity_.y = 0;
	}
}
void Player::WallCollision(const CollisionMapInfo& info) {
	if (info.hitWall) {
		velocity_.x *= (1.0f - kAttenuationWall);
	}
}

void Player::SwitchingOnGround(const CollisionMapInfo& info) {
	if (onGround_) {
		// ジャンプ開始
		if (velocity_.y > 0.0f) {
			onGround_ = false;
		} else {

			// 移動後の4つの角の座標
			std::array<Vector3, Corner::kNumCorner> positionNew;

			for (uint32_t i = 0; i < positionNew.size(); ++i) {
				positionNew[i] = CornerPosition(worldTransform_.translation_ + info.move, static_cast<Corner>(i));
			}

			MapChipType mapChipType;

			// 真下の当たり判定を行う
			bool hit = false;
			// 左下点の判定
			IndexSet indexSet;

			indexSet = mapChipField_->GetMapChipIndexSetByPosition(positionNew[kLeftBottom] + Vector3(0, -kAdjustLanding, 0));
			mapChipType = mapChipField_->GetMapChipTypeByIndex(indexSet.xIndex, indexSet.yIndex);

			if (mapChipType == MapChipType::kBlock) {
				hit = true;
			}

			// 右下点の判定
			indexSet = mapChipField_->GetMapChipIndexSetByPosition(positionNew[kRightBottom] + Vector3(0, -kAdjustLanding, 0));
			mapChipType = mapChipField_->GetMapChipTypeByIndex(indexSet.xIndex, indexSet.yIndex);

			if (mapChipType == MapChipType::kBlock) {
				hit = true;
			}

			// ブロックにヒット？
			if (!hit) {
				// 空中状態に切り替える
				onGround_ = false;
			}
		}
	} else {
		if (info.hitLanding) {
			onGround_ = true;
			velocity_.x *= (1.0f - kAttenuationLanding);
			velocity_.y = 0.0f;
		}
	}
}

void Player::ColisionMap(CollisionMapInfo& info) {

	ColisionMapTop(info);
	ColisionMapBottom(info);
	ColisionMapLeft(info);
	ColisionMapRight(info);
}

// 上の当たり判定
void Player::ColisionMapTop(CollisionMapInfo& info) {
	// 上昇していなければ早期リターン
	if (info.move.y <= 0) {
		return;
	}
	// 移動後の4つの角の座標
	std::array<Vector3, kNumCorner> positionNew;
	for (uint32_t i = 0; i < positionNew.size(); ++i) {

		Vector3 result;
		result.x = worldTransform_.translation_.x + info.move.x;
		result.y = worldTransform_.translation_.y + info.move.y;
		result.z = worldTransform_.translation_.z + info.move.z;

		positionNew[i] = CornerPosition(result, static_cast<Corner>(i));
	}

	MapChipType type;
	// 真上の当たり判定を行う
	bool hit = false;
	IndexSet indexSet;
	indexSet = mapChipField_->GetMapChipIndexSetByPosition(positionNew[int(Corner::kLeftTop)]);
	type = mapChipField_->GetMapChipTypeByIndex(indexSet.xIndex, indexSet.yIndex);
	if (type == MapChipType::kBlock) {
		hit = true;
	}
	indexSet = mapChipField_->GetMapChipIndexSetByPosition(positionNew[int(Corner::kRightTop)]);
	type = mapChipField_->GetMapChipTypeByIndex(indexSet.xIndex, indexSet.yIndex);

	if (type == MapChipType::kBlock) {
		hit = true;
	}

	// ブロックにヒット？
	if (hit) {
		indexSet = mapChipField_->GetMapChipIndexSetByPosition(positionNew[kRightTop]);
		Rect rect = mapChipField_->GetRectByIndex(indexSet.xIndex, indexSet.yIndex);
		info.move.y = std::max(0.0f, rect.bottom - worldTransform_.translation_.y - kHeight / 2 - kBlank);
		info.hitCeiling = true;
	}

}

// 下の当たり判定
void Player::ColisionMapBottom(CollisionMapInfo& info) {
	// 下降あり？
	if (info.move.y >= 0) {
		return;
	}
	// 移動後の4つの角の座標
	std::array<Vector3, kNumCorner> positionNew;
	for (uint32_t i = 0; i < positionNew.size(); ++i) {

		Vector3 result;
		result.x = worldTransform_.translation_.x + info.move.x;
		result.y = worldTransform_.translation_.y + info.move.y;
		result.z = worldTransform_.translation_.z + info.move.z;

		positionNew[i] = CornerPosition(result, static_cast<Corner>(i));
	}

	MapChipType type;
	// 真下の当たり判定を行う
	bool hit = false;
	IndexSet indexSet;
	indexSet = mapChipField_->GetMapChipIndexSetByPosition(positionNew[int(Corner::kLeftBottom)]);
	type = mapChipField_->GetMapChipTypeByIndex(indexSet.xIndex, indexSet.yIndex);
	if (type == MapChipType::kBlock) {
		hit = true;
	}
	indexSet = mapChipField_->GetMapChipIndexSetByPosition(positionNew[int(Corner::kRightBottom)]);
	type = mapChipField_->GetMapChipTypeByIndex(indexSet.xIndex, indexSet.yIndex);
	if (type == MapChipType::kBlock) {
		hit = true;
	}

	// ブロックにヒット？
	if (hit) {
		indexSet = mapChipField_->GetMapChipIndexSetByPosition(positionNew[kRightBottom]);
		Rect rect = mapChipField_->GetRectByIndex(indexSet.xIndex, indexSet.yIndex);
		info.move.y = std::min(0.0f, (rect.top - worldTransform_.translation_.y) + kHeight / 2 + kBlank);
		info.hitLanding = true;
	}
}
//
// 右の当たり判定(壁)
void Player::ColisionMapRight(CollisionMapInfo& info) {

	if (info.move.x <= 0) {
		return;
	}
	// 移動後の4つの角の座標
	std::array<Vector3, kNumCorner> positionNew;
	for (uint32_t i = 0; i < positionNew.size(); ++i) {

		Vector3 result;
		result.x = worldTransform_.translation_.x + info.move.x;
		result.y = worldTransform_.translation_.y + info.move.y;
		result.z = worldTransform_.translation_.z + info.move.z;

		positionNew[i] = CornerPosition(result, static_cast<Corner>(i));
	}
	MapChipType type;
	// 右方向の当たり判定を行う
	bool hit = false;
	IndexSet indexSet;
	indexSet = mapChipField_->GetMapChipIndexSetByPosition(positionNew[int(Corner::kRightTop)]);
	type = mapChipField_->GetMapChipTypeByIndex(indexSet.xIndex, indexSet.yIndex);
	if (type == MapChipType::kBlock) {
		hit = true;
	}
	indexSet = mapChipField_->GetMapChipIndexSetByPosition(positionNew[int(Corner::kRightBottom)]);
	type = mapChipField_->GetMapChipTypeByIndex(indexSet.xIndex, indexSet.yIndex);

	if (type == MapChipType::kBlock) {
		hit = true;
	}

	// ブロックにヒット？
	if (hit) {
		indexSet = mapChipField_->GetMapChipIndexSetByPosition(positionNew[kRightTop]);
		Rect rect = mapChipField_->GetRectByIndex(indexSet.xIndex, indexSet.yIndex);
		info.move.x = std::max(0.0f, rect.left - worldTransform_.translation_.x - kWidth / 2 - kBlank);
		info.hitWall = true;
	}
}
//左の当たり判定(壁	)
 void Player::ColisionMapLeft(CollisionMapInfo& info) {
 
 if (info.move.x >= 0) {
		 return;
	 }
	 // 移動後の4つの角の座標
	 std::array<Vector3, kNumCorner> positionNew;
	 for (uint32_t i = 0; i < positionNew.size(); ++i) {

		 Vector3 result;
		 result.x = worldTransform_.translation_.x + info.move.x;
		 result.y = worldTransform_.translation_.y + info.move.y;
		 result.z = worldTransform_.translation_.z + info.move.z;

		 positionNew[i] = CornerPosition(result, static_cast<Corner>(i));
	 }
	 MapChipType type;
	 // 左方向の当たり判定を行う
	 bool hit = false;
	 IndexSet indexSet;
	 indexSet = mapChipField_->GetMapChipIndexSetByPosition(positionNew[int(Corner::kLeftTop)]);
	 type = mapChipField_->GetMapChipTypeByIndex(indexSet.xIndex, indexSet.yIndex);
	 if (type == MapChipType::kBlock) {
		 hit = true;
	 }
	 indexSet = mapChipField_->GetMapChipIndexSetByPosition(positionNew[int(Corner::kLeftBottom)]);
	 type = mapChipField_->GetMapChipTypeByIndex(indexSet.xIndex, indexSet.yIndex);

	 if (type == MapChipType::kBlock) {
		 hit = true;
	 }

	 // ブロックにヒット？
	 if (hit) {
		 indexSet = mapChipField_->GetMapChipIndexSetByPosition(positionNew[kRightTop]);
		 Rect rect = mapChipField_->GetRectByIndex(indexSet.xIndex, indexSet.yIndex);
		 info.move.x = std::min(0.0f, rect.left - worldTransform_.translation_.x + kWidth / 2 + kBlank);
		 info.hitWall = true;
	 }
 
 }

Vector3 Player::CornerPosition(const Vector3& center, Corner corner) {
	Vector3 offsetTable[kNumCorner] = {
	    {+kWidth / 2.0f, -kHeight / 2.0f, 0}, //  kRightBottom
	    {-kWidth / 2.0f, -kHeight / 2.0f, 0}, //  kLeftBotttom
	    {+kWidth / 2.0f, +kHeight / 2.0f, 0}, //  kRightTop
	    {-kWidth / 2.0f, +kHeight / 2.0f, 0}, //  kLeftTop
	};

	Vector3 result;
	result.x = center.x + offsetTable[static_cast<uint32_t>(corner)].x;
	result.y = center.y + offsetTable[static_cast<uint32_t>(corner)].y;
	result.z = center.z + offsetTable[static_cast<uint32_t>(corner)].z;
	return result;
}
