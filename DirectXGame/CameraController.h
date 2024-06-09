#pragma once
#define NOMINMAX
#include <ViewProjection.h>

class Player;

struct Rect {
	float left = 0.0f;
	float right = 1.0f;
	float bottom = 0.0f;
	float top = 1.0f;
};

class CameraController {
public:
	void Initialize();
	void Update();
	void SetTarget(Player* target) { target_ = target; };
	void Reset();
	ViewProjection& GetViewProjection() { return viewProjection_; };
	
	void SetMovebleArea( Rect area) { movebleArea_ = area; }

private:
	ViewProjection viewProjection_;
	Player* target_ = nullptr;
	Vector3 targetOffset_ = {0, 0, -25.0f};
	Vector3 targetCoordinates_ = {};
	Rect movebleArea_ = {0, 100, 0, 100};

	static inline const float kInterpolationRate = 0.5f;
	static inline const float kVelocityBias = 3.0f;

	static inline const Rect Margin = {-10, 10, -30, 5};
};
