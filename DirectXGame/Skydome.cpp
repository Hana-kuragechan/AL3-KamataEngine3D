#include "Skydome.h"

Skydome::Skydome() { 
	
}

Skydome::~Skydome() {}

void Skydome::Initialize(Model* model, ViewProjection* viewProjection) { 
	assert(model); 
	model_ = model;
	viewProjection_ = viewProjection;
	worldTransform_.Initialize();
	
}

void Skydome::Update() {}

void Skydome::Draw() { 
	model_->Draw(worldTransform_, *viewProjection_);
}
