﻿#include "PlayerBullet.h"
#include <cassert>
#include"WorldTransform.h"

void PlayerBullet::Initialize(Model* model, const Vector3& pos, const Vector3& velocity) {
	assert(model);
	model_ = model;

	texturehandle_ = TextureManager::Load("white1x1.png");

	world_.Initialize();
	world_.translation_ = pos;
	velocity_ = velocity;
}

void PlayerBullet::Update() { 
	world_.translation_.x += velocity_.x;
	world_.translation_.y += velocity_.y;
	world_.translation_.z += velocity_.z;
	world_.UpdateMatrix();
	if (--deathTimer_ <= 0) {
		isDead_ = true;
	}
}


void PlayerBullet::Draw(ViewProjection& view) { model_->Draw(world_, view, texturehandle_); }