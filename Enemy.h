#pragma once
#include "Model.h"
#include "Vector3.h"
#include "ViewProjection.h"
#include "WorldTransform.h"
#include "Audio.h"
#include "Input.h"
#include "EnemyBullet.h"
#include <list>
#include "MathUtility.h"

class GameScene;

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

	static const int kFireInterval = 80;
	//接近
	void Approach();

	//pos取得
	Vector3 GetworldPosition();

	// 衝突を検知したら呼び出される
	void OnCollision();

	//const std::list<EnemyBullet*>& GetBullrts() const { return bullets_; }

	const float GetRadius() { return radius_; }

	GameScene* gameScene_ = nullptr;
	void SetGameScene(GameScene* gameScene) { gameScene_ = gameScene; }

	bool IsDead() const { return isDead_; }

	private:
	WorldTransform worldTransform_;
	Model* model_;
	uint32_t texturehandle_ = 0u;
	EnemyBullet* bullet_ = nullptr;
	Audio* audio_ = nullptr;


	//std::list<EnemyBullet*> bullets_;
	int32_t pushTimer = 0;
	const float radius_ = 1.0f;
	bool isDead_ = false;
	static const int32_t kLifeTime = 60 * 5;
	int32_t deathTimer_ = kLifeTime;
	int32_t enemygamagesound = 0;

};
