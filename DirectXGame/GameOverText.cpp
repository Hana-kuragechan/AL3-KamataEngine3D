#include "GameOverText.h"

void GameOverText::Initialize(Model* model, ViewProjection* viewProjection) {
	assert(model);
	model_ = model;
	viewProjection_ = viewProjection;

	worldTransform_.Initialize();
}

void GameOverText::Update() { worldTransform_.UpdateMatrix(); }

void GameOverText::Draw() { 
	model_->Draw(worldTransform_, *viewProjection_); 
}
