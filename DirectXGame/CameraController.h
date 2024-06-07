#pragma once
#include <ViewProjection.h>

class Player;

class CameraController {
public:
	void Initialize();
	void Update();
	void SetTarget(Player* target) { target_ = target; };
	void Reset();
	ViewProjection& GetViewProjection() { return viewProjection_; };
	struct Rect {
		float left = 0.0f;
		float right = 1.0f;
		float bottom = 0.0f;
		float top = 1.0f;
	};
	Rect movebleArea_ = {0, 100, 0, 100};
	void SetMovebleArea( Rect area) { movebleArea_ = area; }

private:
	ViewProjection viewProjection_;
	Player* target_ = nullptr;
	Vector3 targetOffset_ = {0, 0, -15.0f};
	
	Vector3 goal_ = {};
	static inline const float kInterpolationRate = 3.0f;
};
