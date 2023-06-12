#pragma once
#include "Model.h"
#include "Vector3.h"
#include "ViewProjection.h"
#include "WorldTransform.h"

#include "Input.h"
#include "EnemyBullet.h"
#include <list>
#include "MathUtility.h"

class Player;
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

	void Fire();

	Player* player_ = nullptr;
	void SetPlayer(Player* player) { player_ = player; }

	Phase phase_ = Phase::Approach; 

	~Enemy();

	static const int kFireInterval = 60;
	//接近
	void Approach();

	//pos取得
	Vector3 GetworldPosition();

	// 衝突を検知したら呼び出される
	void OnCollision();

	const std::list<EnemyBullet*>& GetBullrts() const { return bullets_; }

	const float GetRadius() { return radius_; }

	private:
	WorldTransform worldTransform_;
	Model* model_;
	uint32_t texturehandle_ = 0u;
	EnemyBullet* bullet_ = nullptr;
	std::list<EnemyBullet*> bullets_;
	int32_t pushTimer = 0;
	const float radius_ = 1.0f;
};
