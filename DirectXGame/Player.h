#include "Model.h"
#include "WorldTransform.h"
#include<numbers>

enum class LRDirection {
	kRight,
	kLeft,
};

// マップとの当たり判定情報
struct CollisionMapInfo {

	bool hitCeiling = false; // 天井
	bool hitLanding = false; // 着地
	bool hitWall = false; // 壁
	Vector3 move;
};

// 角
enum Corner {
	kRightBottom, // 右下
	kLeftBottom,  // 左下
	kRightTop,    // 右上
	kLeftTop,     // 左上
	kNumCorner    // 要素数
};


class MapChipField;
class Enemy;

class Player {
public:
	/// <summary>
	/// 初期化処理
	/// </summary>
	void Initialize(Model* model,  ViewProjection* viewProjection,const Vector3&position);
	/// <summary>
	/// 更新
	/// </summary>
	void Update();
	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

	WorldTransform& GetWorldTransform() { return worldTransform_; };
	const Vector3& GetVelocity() const { return velocity_; }
	void SetMapChipField(MapChipField* mapChipField) { mapChipField_=mapChipField; };
	void MoveResult(CollisionMapInfo& info); 
	void CeilingCollision(const CollisionMapInfo& info);
	void WallCollision(const CollisionMapInfo& info);
	void SwitchingOnGround(const CollisionMapInfo& info);
	Vector3 GetWorldPosition();
	AABB GetAABB();
	void OnCollosion(const Enemy*enemy);

private:

	LRDirection lrDirection_ = LRDirection::kRight;
	float turnFirstRotationY_ = 0.0f;
	float turnTimer_ = 0.0f;
	static inline const float kTimeTurn = 0.3f;
	bool onGround_ = true;
	WorldTransform worldTransform_;
	Model* model_ = nullptr;
	ViewProjection* viewProjection_ = nullptr;
	Vector3 velocity_ = {};

	static inline const float kAcceleration = 1.0f;
	static inline const float kAttenuation = 0.1f;
	static inline const float kLimitRunSpeed=0.3f;
	static inline const float kGravityAcceleration = 0.1f;
	static inline const float kLimitFallSpeed = 0.7f;
	static inline const float kJumpAcceleration = 1.1f;
	static inline const float kBlank = 0.1f;

	//着地時の速度減衰率
	static inline const float kAttenuationLanding = 0.1f;
	static inline const float kAdjustLanding = 0.2f;
	//着地時の速度減衰率
	static inline const float kAttenuationWall = 0.1f;
	//マップチップによるフィールド
	MapChipField* mapChipField_ = nullptr;
	//キャラクターの当たり判定サイズ
	static inline const float kWidth = 1.8f;
	static inline const float kHeight = 1.8f;
	
	void ColisionMap(CollisionMapInfo& info);
	void ColisionMapTop(CollisionMapInfo& info);
	void ColisionMapBottom(CollisionMapInfo& info);
	void ColisionMapRight(CollisionMapInfo& info);
	void ColisionMapLeft(CollisionMapInfo& info);
	
	Vector3 CornerPosition(const Vector3& center, Corner corner);
};