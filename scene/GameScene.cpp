#include "GameScene.h"
#include "AxisIndicator.h"
#include "TextureManager.h"

#include <cassert>
#include <fstream>


GameScene::GameScene() {}

GameScene::~GameScene() {

	for (EnemyBullet* bullet : bullets_) {
				delete bullet;
			}

	delete model_;
	delete player_;
	delete titlesprite_;
	for (Enemy* enemy : enemy_) {
		    delete enemy;

	}
	delete debugCamera_;
	delete modelSkydome_;
	delete railcamera_;
}

void GameScene::Initialize() {

	dxCommon_ = DirectXCommon::GetInstance();
	input_ = Input::GetInstance();
	audio_ = Audio::GetInstance();



	// テクスチャ読み込み
	//textureHandle_ = TextureManager::Load("player/player.png");

	titletextureHandle_ = TextureManager::Load("title.png");

	gameovertextureHandle_ = TextureManager::Load("gameover.png");

	gamecleartextureHandle_ = TextureManager::Load("gameclear.png");

	 
	// 3Dモデルの生成
	model_ = Model::CreateFromOBJ("player", true);

	titlesprite_ = Sprite::Create(titletextureHandle_, {0, 0});

	gameclearsprite_ = Sprite::Create(gamecleartextureHandle_, {0, 0});

	gameoversprite_ = Sprite::Create(gameovertextureHandle_, {0, 0});

	// ワールドトランスフォーム初期化
	// worldTransform_.Initialize();
	// ビュープロジェクションの初期化
	viewProjection_.Initialize();

	// 自キャラの生成
	player_ = new Player();
	// 自キャラの初期化
	//player_->Initialize(model_, textureHandle_);

	// 敵キャラの生成
	//enemy_ = new Enemy();
	
	//enemy->SetGameScene(this);
	//// enemy_->Initialize(model_, {30.0f, 0.0f, 60.0f});
	//	敵キャラに自キャラのアドレスを渡す
	for (Enemy* enemy : enemy_) {
		    enemy->SetPlayer(player_);
	}
	// 天球生成
	modelSkydome_ = Model::CreateFromOBJ("skydome", true);
	skydome_ = new Skydome();
	skydome_->Initialize(modelSkydome_);

	//title 
	


	// 自機をカメラからずらす
	Vector3 playerPosition(0, 0, 50.0f);
	player_->Initialize(model_, textureHandle_, playerPosition);
	

	// デバックカメラの生成
	debugCamera_ = new DebugCamera(WinApp::kWindowWidth, WinApp::kWindowHeight);

	// 軸方向表示の表示を有効にする
	AxisIndicator::GetInstance()->SetVisible(true);
	// 軸方向表示が参照するビュープロジェクションを指定する（アドレス渡し）
	AxisIndicator::GetInstance()->SetTargetViewProjection(&viewProjection_);

	// レールカメラ生成
	railcamera_ = new RailCamera();
	railcamera_->Initialize({0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f});

	// 自キャラとレールカメラの親子関係
	player_->SetParent(&railcamera_->GetWorldTransform());

	LoadEnemyPopDate();

	//レティクルのテクスチャ
	 TextureManager::Load("target.png");
}



void GameScene::Update() {
	 switch (scene_) {

	 case Scene::title:

		    if (input_->TriggerKey(DIK_SPACE)) {
			    scene_ = Scene::GamePlay;
		    }

			enemyPopCommands.clear();
		   enemyPopCommands.seekg(0,std::ios::beg);
		  
	   
		    player_->reset();
		   
		    break;

	 case Scene::GamePlay:

		    // 自キャラの更新
		 

		    
			    player_->Update(viewProjection_);
		    
		  
			if (player_->playerHp <= 0) {
			    scene_ = Scene::GameOver;
		    }


	 // 敵
	 bullets_.remove_if([](EnemyBullet* bullet) {
		 if (bullet->IsDead()) {
			 delete bullet;
			 return true;
		 }
		 return false;
	 });

	 enemy_.remove_if([](Enemy* enemy) {
		 if (enemy->IsDead()) {
			 delete enemy;
			 return true;
		 }
		 return false;
	 });


	 // 敵キャラ生成の更新
	 UpdateEnemyPopCommands();

	 // 敵キャラの更新
	 for (Enemy* enemy : enemy_) {
		    enemy->Update();
	 }

	 for (EnemyBullet* bullet : bullets_) {
		    bullet->Update();
	 }

	 CheckAllCollisions();
	 // 天球更新
	 skydome_->Update();

	 // レールカメラ更新
	 railcamera_->Update();

#ifdef _DEBUG
	 if (input_->TriggerKey(DIK_BACKSPACE)) {
		    isDebugCameraActive_ = true;
	 }
#endif

	 if (isDebugCameraActive_) {
		    debugCamera_->Update();
		    viewProjection_.matView = debugCamera_->GetViewProjection().matView;
		    viewProjection_.matProjection = debugCamera_->GetViewProjection().matProjection;
		    viewProjection_.TransferMatrix();
	 } else if (!isDebugCameraActive_) {
		    viewProjection_.matView = railcamera_->GetViewProjection().matView;
		    viewProjection_.matProjection = railcamera_->GetViewProjection().matProjection;
		    viewProjection_.TransferMatrix();
	 }

	 break;

	case Scene::GameOver:
	player_->reset();
	 enemy_.clear();
	 bullets_.clear();
	if (input_->TriggerKey(DIK_SPACE)) {
			scene_ = Scene::title;
	}
	
	break;
	
	case Scene::GameClear:

	enemy_.clear();
	bullets_.clear();
	if (input_->TriggerKey(DIK_SPACE)) {
			scene_ = Scene::title;
	}
	
	
		 break;

	}
}

void GameScene::Draw() {

	// コマンドリストの取得
	ID3D12GraphicsCommandList* commandList = dxCommon_->GetCommandList();

#pragma region 背景スプライト描画
	// 背景スプライト描画前処理
	Sprite::PreDraw(commandList);

	/// <summary>
	/// ここに背景スプライトの描画処理を追加できる
	/// </summary>

	// スプライト描画後処理
	Sprite::PostDraw();
	// 深度バッファクリア
	dxCommon_->ClearDepthBuffer();
#pragma endregion

#pragma region 3Dオブジェクト描画
	// 3Dオブジェクト描画前処理
	Model::PreDraw(commandList);

	/// <summary>
	/// ここに3Dオブジェクトの描画処理を追加できる
	/// </summary>
	// model_->Draw(worldTransform_, viewProjection_, textureHandle_);

	// 自キャラの描画

	//switch (scene_) {
	//
	//case GameScene::Scene::title:
	//
	// // コマンドリストの取得
	//
	// break;
	//
	//case GameScene::Scene::GamePlay:


	player_->Draw(viewProjection_);
	 
	 // 敵キャラの描画
	 for (Enemy* enemy : enemy_) {
			enemy->Draw(viewProjection_);
	 }
	 for (EnemyBullet* bullet : bullets_) {
			bullet->Draw(viewProjection_);
	 }

	 // 天球の描画
	 skydome_->Draw(viewProjection_);

	 // 3Dオブジェクト描画後処理
	 Model::PostDraw();
#pragma endregion

#pragma region 前景スプライト描画
	 // 前景スプライト描画前処理
	 Sprite::PreDraw(commandList);

	 /// <summary>
	 /// ここに前景スプライトの描画処理を追加できる
	 /// </summary>

	 player_->DrawUI();


	 if (scene_ == Scene::title) {
			titlesprite_->Draw();
	 }

	 if (scene_ == Scene::GameClear) {
			gameclearsprite_->Draw();
	 }
	
	 if (scene_ == Scene::GameOver) {
		    gameoversprite_->Draw();
	 }

	 // スプライト描画後処理
	 Sprite::PostDraw();

#pragma endregion
	// break;
	//}
}

void GameScene::AddEnemy(Vector3 pos) {
	// 敵キャラの生成
	Enemy* obj = new Enemy();
	// 敵キャラの初期化
	obj->Initialize(model_, pos);
	obj->SetPlayer(player_);
	obj->SetGameScene(this);

	enemy_.push_back(obj);
}

void GameScene::AddEnemyBullet(EnemyBullet* enemyBullet) {
	// リストに登録
	bullets_.push_back(enemyBullet);
}


void GameScene::CheckAllCollisions() {

	// 衝突判定AとBの座標
	Vector3 posA, posB;

	// 自弾リストの取得
	const std::list<PlayerBullet*>& playerBullets = player_->GetBullrts();

	// 敵弾リストの取得
	 const std::list<EnemyBullet*>& enemyBullets = bullets_;

#pragma region // 自キャラと敵弾の当たり判定
	
	// 自キャラの座標
	posA = player_->GetworldPosition();
	// 自キャラと敵弾全ての当たり判定
	for (EnemyBullet* bullet : enemyBullets) {
		    // 敵弾の座標
		    posB = bullet->GetworldPosition();

		    float distance = (posB.x - posA.x) * (posB.x - posA.x) +
		                     (posB.y - posA.y) * (posB.y - posA.y) +
		                     (posB.z - posA.z) * (posB.z - posA.z);

		    // 球と球の交差判定
		    if (distance <= (player_->GetRadius() + bullet->GetRadius()) *(player_->GetRadius() + bullet->GetRadius())) {
			    // 自キャラの衝突時コールバックを呼び出す
			    player_->OnCollision();
			    // 敵弾の衝突時コールバックを呼び出す
			    bullet->OnCollision();
		    }
	}
#pragma endregion

#pragma region // 自弾と敵キャラの当たり判定

		// 敵キャラの座標
	for (Enemy* enemy : enemy_) {

		posA = enemy->GetworldPosition();
		// 敵キャラと自弾全ての当たり判定
		for (PlayerBullet* bullet : playerBullets) {

			// 自弾の座標
			posB = bullet->GetworldPosition();

			float distance = (posB.x - posA.x) * (posB.x - posA.x) +
			                 (posB.y - posA.y) * (posB.y - posA.y) +
			                 (posB.z - posA.z) * (posB.z - posA.z);

			// 球と球の交差判定
			if (distance <= (enemy->GetRadius() + bullet->GetRadius()) * (enemy->GetRadius() + bullet->GetRadius())) {
				// 敵キャラの衝突時コールバックを呼び出す
				enemy->OnCollision();
				// 敵弾の衝突時コールバックを呼び出す
				bullet->OnCollision();
			}
		}
	}
#pragma endregion

#pragma region // 自弾と敵弾の当たり判定
	// 自弾全て
	for (PlayerBullet* playerBullet : playerBullets) {
		posA = playerBullet->GetworldPosition();
		// 敵弾全て
		for (EnemyBullet* enemyBullet: enemyBullets) {
			posB = enemyBullet->GetworldPosition();

			float distance = (posB.x - posA.x) * (posB.x - posA.x) +
			                 (posB.y - posA.y) * (posB.y - posA.y) +
			                 (posB.z - posA.z) * (posB.z - posA.z);

			// 球と球の交差判定
			if (distance <= (playerBullet->GetRadius() + enemyBullet->GetRadius()) * (playerBullet->GetRadius() + enemyBullet->GetRadius())) {
				// 自弾の衝突時コールバックを呼び出す
				playerBullet->OnCollision();
				// 敵弾の衝突時コールバックを呼び出す
				enemyBullet->OnCollision();
			}
		}
	}
#pragma endregion

	}

void GameScene::LoadEnemyPopDate() {
	// ファイルを開く
	std::ifstream file;
	file.open("Resources/enemyPop.csv");
	assert(file.is_open());

	// ファイルの内容を文字列ストリームにコピー
	enemyPopCommands << file.rdbuf();

	// ファイルを閉じる
	file.close();
}

void GameScene::UpdateEnemyPopCommands() {
	// 待機処理
	if (IsWaitFlag_) {
		waitTimer_--;
		if (waitTimer_ <= 0) {
			// 待機フラグ
			IsWaitFlag_ = false;
		}
		return;
	}

	if (Isdeath_) {

		clearTimer_--;

		if (clearTimer_ <= 0) {

				scene_ = Scene::GameClear;
				Isdeath_ = false;
			
		}
	}


	// 1行分の文字列を入れる変数
	std::string line;

	// コマンド実行ループ
	while (getline(enemyPopCommands, line)) {
		// 1行分の文字列をストリームに変換して解析しやすくする
		std::istringstream line_stream(line);

		std::string word;
		// ,区切りで行の先頭文字列を取得
		getline(line_stream, word, ',');

		// "//"から始まる行はコメント
		if (word.find("//") == 0) {
			// コメント行を飛ばす
			continue;
		}

		// POPコマンド
		if (word.find("POP") == 0) {
			// x座標
			getline(line_stream, word, ',');
			float x = (float)std::atof(word.c_str());

			// y座標
			getline(line_stream, word, ',');
			float y = (float)std::atof(word.c_str());

			// z座標
			getline(line_stream, word, ',');
			float z = (float)std::atof(word.c_str());

			// 敵を発生させる
			AddEnemy(Vector3(x, y, z));

		} else if (word.find("WAIT") == 0) {
			getline(line_stream, word, ',');

			// 待ち時間
			int32_t waitTime = atoi(word.c_str());

			// 待機開始
			IsWaitFlag_ = true;
			waitTimer_ = waitTime;

		} else if (word.find("DEATH") == 0) {
			getline(line_stream, word, ',');

			// clear
			
			int32_t cleardeath = atoi(word.c_str());

			if (cleardeath == 1) {

				Isdeath_ = true;
			}
			
			// コマンドループを抜ける
			break;
		}
		
	}
}

//void GameScene::SceneChange(Scene scene){
//	if (input_->PushKey(DIK_SPACE)) {
//		scene_++;
//	}
//
//};

