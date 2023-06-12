#include "Skydome.h"

void Skydome::Draw(ViewProjection& view) {
	model_->Draw(worldTransform_, view); }


void Skydome::Initialize(Model* model) {
	model_ = model; 
	worldTransform_.Initialize();
}

void Skydome::Update() {}
