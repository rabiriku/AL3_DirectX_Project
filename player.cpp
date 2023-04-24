#include "player.h"
#include <cassert>


void Player::initialize(Model* model, uint32_t textureHandle) { 
	assert(model);


	model_ = model;
	worldTransform_.Initialize();
	textureHundle_ = textureHandle;
}

void Player::update() { worldTransform_.TransferMatrix(); }

void Player::Draw(ViewProjection& viewProjection) {
	model_->Draw(worldTransform_, viewProjection, textureHundle_);
 }
