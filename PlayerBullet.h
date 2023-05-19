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

private:
	WorldTransform world_;
	Model* model_;
	uint32_t texturehandle_;
	Vector3 velocity_;
};
