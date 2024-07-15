#include "Title.h"

void Title::Initialize(Model* model, ViewProjection* viewProjection) {
	assert(model);
	model_ = model;
	viewProjection_ = viewProjection;

	worldTransform_.Initialize();

}

void Title::Update() { worldTransform_.UpdateMatrix(); }

void Title::Draw() { model_->Draw(worldTransform_, *viewProjection_); 

}
