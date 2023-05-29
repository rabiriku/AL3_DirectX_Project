#pragma once
#include "Model.h"
#include "Vector3.h"
#include "ViewProjection.h"
#include "WorldTransform.h"
/// <summary>
/// 敵
/// </summary>

enum class Phase {
	Approach, // 接近
	Leave,    // 離脱
};

class Enemy {
public:
	void Initialize(Model* model, const Vector3& pos);
	void Update();
	void Draw(ViewProjection& view);
	Phase phase_ = Phase::Approach; 
	
	private:
	WorldTransform world_;
	Model* model_;
	uint32_t texturehandle_;
};
