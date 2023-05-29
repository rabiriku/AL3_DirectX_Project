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
	const float KCharaSpeed = -0.2f;
	world_.UpdateMatrix();

	switch (phase_) {
	case Phase::Approach:
	default:
		//移動
		world_.translation_.z += KCharaSpeed;

		if (world_.translation_.z < 0.0f) {
			phase_ = Phase::Leave;
		}
		break;

	case Phase::Leave:
		//移動
		world_.translation_.x += KCharaSpeed;
		break;
	}
	
}

void Enemy::Draw(ViewProjection& view) { model_->Draw(world_, view, texturehandle_); }
