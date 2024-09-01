#include "GameClearText.h"

void GameClearText::Initialize(Model* model, ViewProjection* viewProjection) {
	assert(model);
	model_ = model;
	viewProjection_ = viewProjection;

	worldTransform_.Initialize();
}

void GameClearText::Update() { worldTransform_.UpdateMatrix(); }

void GameClearText::Draw() { model_->Draw(worldTransform_, *viewProjection_); }
