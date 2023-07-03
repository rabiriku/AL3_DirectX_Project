#pragma once

#include "Audio.h"
#include "DirectXCommon.h"
#include "Input.h"
#include "Model.h"
#include "SafeDelete.h"
#include "Sprite.h"
#include "ViewProjection.h"
#include "WorldTransform.h"

#include "DebugCamera.h"
#include "Player.h"
#include "Enemy.h"
#include "Skydome.h"
#include "RailCamera.h"

#include <sstream>

/// <summary>
/// ゲームシーン
/// </summary>
class GameScene {

public: // メンバ関数
	/// <summary>
	/// コンストクラタ
	/// </summary>
	GameScene();

	/// <summary>
	/// デストラクタ
	/// </summary>
	~GameScene();

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();

	/// <summary>
	/// 毎フレーム処理
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

	void CheckAllCollisions();

	void AddEnemyBullet(EnemyBullet* enemyBullet);

	void AddEnemy(Vector3 pos);

	void LoadEnemyPopDate();

	void UpdateEnemyPopCommands();

private: // メンバ変数
	DirectXCommon* dxCommon_ = nullptr;
	Input* input_ = nullptr;
	Audio* audio_ = nullptr;

	// 3Dモデル
	Model* model_ = nullptr;

	// 自キャラ
	Player* player_ = nullptr;

	//敵
	//Enemy* enemy_ = nullptr;
	std::list<Enemy*> enemy_;
	//天球
	Skydome* skydome_ = nullptr;

	Model* modelSkydome_ = nullptr;

	//レールカメラ
	RailCamera* railcamera_ = nullptr;

	// デバックカメラ
	DebugCamera* debugCamera_ = nullptr;

	/// <summary>
	/// ゲームシーン用
	/// </summary>

	// テクスチャハンドル
	uint32_t textureHandle_ = 0;

	//WorldTransform worldTransform_;

	// ワールドトランスフォーム
	 WorldTransform worldTransform_;
	// ビュープロジェクション
	ViewProjection viewProjection_;

	// デバックカメラ有効
	bool isDebugCameraActive_ = false;

	//敵関連リスト
	std::list<EnemyBullet*> bullets_;
	//std::list<EnemyBullet*> enemyBullets_;
	const std::list<EnemyBullet*>& GetBullrts() const { return bullets_; }

	//敵発生コマンド
	std::stringstream enemyPopCommands;

	// 待機中フラグ
	bool IsWaitFlag_ = false;

	// 待機タイマー
	int waitTimer_ = 0;
};