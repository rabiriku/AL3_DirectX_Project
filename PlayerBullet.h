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

private:
	WorldTransform world_;
	Model* model_;
	uint32_t texturehandle_;
	Vector3 velocity_;
	static const int32_t kLifeTime = 60 * 5;
	int32_t deathTimer_ = kLifeTime;
	bool isDead_ = false;
	
};
