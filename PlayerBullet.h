#pragma once
#include "Model.h"
#include "Vector3.h"
#include "ViewProjection.h"
#include "WorldTransform.h"

class PlayerBullet {
public:
	void Initialize(Model* model, const Vector3& pos, const Vector3& velocity);
	void Update();
	void Draw(ViewProjection& view);
	bool IsDead() const { return isDead_; }
	// 衝突を検知したら呼び出される
	void OnCollision();

	Vector3 GetworldPosition();

	const float GetRadius() { return radius_; }

private:
	WorldTransform worldTransform_;
	Model* model_ = nullptr;
	uint32_t texturehandle_;
	Vector3 velocity_;
	static const int32_t kLifeTime = 60 * 5;
	int32_t deathTimer_ = kLifeTime;
	bool isDead_ = false;
	const float radius_ = 1.0f;
};
