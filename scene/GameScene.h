﻿#pragma once

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

private: // メンバ変数
	DirectXCommon* dxCommon_ = nullptr;
	Input* input_ = nullptr;
	Audio* audio_ = nullptr;

	// 3Dモデル
	Model* model_ = nullptr;

	// 自キャラ
	Player* player_ = nullptr;

	//敵
	Enemy* enemy_ = nullptr;

	//天球
	Skydome* skydome_ = nullptr;

	Model* modelSkydome_ = nullptr;

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
};