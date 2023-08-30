#pragma once
#include "Model.h"
#include "Vector3.h"
#include "ViewProjection.h"
#include "WorldTransform.h"

class EnemyBullet {
	public:
	void Initialize(Model* model, const Vector3& pos, const Vector3& velocity);
	void Update();
	void Draw(ViewProjection& view);
	bool IsDead() const { return isDead_; }

	Vector3 GetworldPosition();

	// 衝突を検知したら呼び出される
	void OnCollision();

	const float GetRadius() { return radius_; }

private:
	WorldTransform worldTransform_;
	Model* model_=nullptr;
	uint32_t texturehandle_=0;
	Vector3 velocity_ = {};
	static const int32_t kLifeTime = 60 * 5;
	int32_t deathTimer_ = kLifeTime;
	bool isDead_ = false;
	const float radius_ = 1.0f;

};
