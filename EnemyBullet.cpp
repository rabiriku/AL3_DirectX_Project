#include "EnemyBullet.h"
#include <cassert>
#include"WorldTransform.h"
void EnemyBullet::Initialize(Model* model, const Vector3& pos, const Vector3& velocity) 
{
	assert(model);
	model_ = Model::CreateFromOBJ("weapon", true);


	texturehandle_ = TextureManager::Load("uvChecker.png");

	 worldTransform_.Initialize();
	 worldTransform_.translation_ = pos;
	 velocity_ = velocity;


}

void EnemyBullet::Update() {
	 worldTransform_.translation_.x += velocity_.x;
	 worldTransform_.translation_.y += velocity_.y;
	 worldTransform_.translation_.z += velocity_.z;
	 worldTransform_.UpdateMatrix();
	if (--deathTimer_ <= 0) {
		isDead_ = true;
	}
}

void EnemyBullet::Draw(ViewProjection& view) {
	model_->Draw(worldTransform_, view);
}

Vector3 EnemyBullet::GetworldPosition() {
	// ワールド座標を入れる変数
	Vector3 worldPos = {};

	worldPos.x = worldTransform_.translation_.x;
	worldPos.y = worldTransform_.translation_.y;
	worldPos.z = worldTransform_.translation_.z;

	return worldPos;
}


void EnemyBullet::OnCollision() { isDead_ = true; }