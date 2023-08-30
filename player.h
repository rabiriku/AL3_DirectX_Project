#pragma once
/// <summary>
/// 自キャラ
/// </summary>

#include "Model.h"
#include "ViewProjection.h"
#include "WorldTransform.h"
#include "Audio.h"
#include "Input.h"
#include "PlayerBullet.h"
#include <list>
#include "MathUtility.h"
#include "Sprite.h"
#include "WinApp.h"

class Player {
public:
	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize(Model* model, uint32_t textureHandle, Vector3 playerPosition);

	/// <summary>
	/// 更新
	/// </summary>
	void Update(ViewProjection& viewProjection);

	/// <summary>
	/// 描画
	/// </summary>
	void Draw(ViewProjection& viewProjection);

	/// <summary>
	/// 攻撃
	/// </summary>
	void Attack();

	void reset();
	
	bool Hpparo() const { return playerHp; }

	bool IsDead() const { return isDead_; }

	
	/// <summary>
	/// デストラクタ
	/// </summary>
	~Player();

	//リスト化
	std::list<Player*> player_;
	std::list<PlayerBullet*> bullets_;

	//position取得
	Vector3 GetworldPosition();

	Vector3 GetWorldTransform3DReticle();

	//衝突を検知したら呼び出される
	void OnCollision();

	void SetParent(const WorldTransform* parent);

	const float GetRadius() { return radius_; }

	const std::list<PlayerBullet*>& GetBullrts() const { return bullets_; }

	void DrawUI();

	//void DrawHp();
	// ワールド変換データ
	WorldTransform worldTransform_;

	//3Dレティクル用ワールドトランスフォーム
	WorldTransform worldTransform3DReticle_;

	// モデル
	Model* model_ = nullptr;

	// テクスチャハンドル
	uint32_t textureHandle_ = 0u;

	// キーボード入力
	Input* input_ = nullptr;

	Audio* audio_ = nullptr;

	// 弾
	PlayerBullet* bullet_ = nullptr;
	
	const float radius_ = 1.0f;

	// 2Dレティクル用スプライト
	Sprite* sprite2DReticle_ = nullptr;

	Sprite* playerHpsprite_ = nullptr;
	Sprite* playerHpsprite2_ = nullptr;
	Sprite* playerHpsprite3_ = nullptr;
	Sprite* playerHpsprite4_ = nullptr;
	Sprite* playerHpsprite5_ = nullptr;

	uint32_t damagesound_ = 0;

	float playerHp = 5.0f;

	bool isDead_ = false;
};