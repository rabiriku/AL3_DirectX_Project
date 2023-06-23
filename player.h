#pragma once
/// <summary>
/// 自キャラ
/// </summary>

#include "Model.h"
#include "ViewProjection.h"
#include "WorldTransform.h"

#include "Input.h"
#include "PlayerBullet.h"
#include <list>
#include "MathUtility.h"

class Player {
public:
	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize(Model* model, uint32_t textureHandle, Vector3 playerPosition);

	/// <summary>
	/// 更新
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw(ViewProjection& viewProjection);

	/// <summary>
	/// 攻撃
	/// </summary>
	void Attack();

	/// <summary>
	/// デストラクタ
	/// </summary>
	~Player();

	//リスト化
	std::list<PlayerBullet*> bullets_;

	//position取得
	Vector3 GetworldPosition();

	//衝突を検知したら呼び出される
	void OnCollision();

	void SetParent(const WorldTransform* parent);

	const float GetRadius() { return radius_; }

	const std::list<PlayerBullet*>& GetBullrts() const { return bullets_; }


	// ワールド変換データ
	WorldTransform worldTransform_;

	// モデル
	Model* model_ = nullptr;

	// テクスチャハンドル
	uint32_t textureHandle_ = 0u;

	// キーボード入力
	Input* input_ = nullptr;

	// 弾
	PlayerBullet* bullet_ = nullptr;
	
	const float radius_ = 1.0f;
};