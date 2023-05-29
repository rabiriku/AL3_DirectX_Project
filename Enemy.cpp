#include "Enemy.h"
#include <cassert>
#include"WorldTransform.h"

void Enemy::Initialize(Model* model, const Vector3& pos) {
	assert(model);
	model_ = model;

	texturehandle_ = TextureManager::Load("sample.png");

	world_.Initialize();
	world_.translation_ = pos;
}

void Enemy::Update() {
	world_.translation_.z -= 0.3f;
	world_.UpdateMatrix();
	
}

void Enemy::Draw(ViewProjection& view) { model_->Draw(world_, view, texturehandle_); }
