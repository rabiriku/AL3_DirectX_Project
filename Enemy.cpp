#include "Enemy.h"
#include<assert.h>
#include "ImGuiManager.h"
#include "MathUtility.h"
#include "player.h"
#include "GameScene.h"


void Enemy::Initialize(Model* model, const Vector3& pos) {
	assert(model);
	model_ = model;

	model_ = Model::CreateFromOBJ("enemy", true);

	audio_ = Audio::GetInstance();

	worldTransform_.Initialize();
	//texturehandle_ = TextureManager::Load("white1x1.png");
	//  サウンド読み込み
	enemygamagesound = audio_->LoadWave("short_bomb.wav");

	Vector3 Pos = {pos.x, pos.y, pos.z};
	worldTransform_.translation_ = pos;

	Approach();
}

void Enemy::Update() {
	

	pushTimer--;
	if (pushTimer <= 0) {
		Fire();
		pushTimer = kFireInterval;
	}


	const float KCharaSpeed = -0.2f;
	worldTransform_.UpdateMatrix();

	
	switch (phase_) {
	case Phase::Approach:
	default:

		//移動
		worldTransform_.translation_.z += KCharaSpeed;

		if (worldTransform_.translation_.z < 50.0f) {
			isDead_ = true;
			
		}
		

		break;

	case Phase::Leave:
		//移動
		worldTransform_.translation_.z -= KCharaSpeed;
		
		if (worldTransform_.translation_.z > 80.0f) {
			phase_ = Phase::Approach;
		}

		break;
	}

}

void Enemy::Draw(ViewProjection& view) { 
	model_->Draw(worldTransform_, view);


	
	//for ( EnemyBullet* bullet : bullets_) {
	//	bullet->Draw(view);
	//}
}

void Enemy::Fire() {
	assert(player_);

	const float kBulletSpeed = -2.0f;

	// 敵キャラ→自キャラの差分ベクトルを求める
	Vector3 distance = {};
	distance.x = GetworldPosition().x - player_->GetworldPosition().x;
	distance.y = GetworldPosition().y - player_->GetworldPosition().y;
	distance.z = GetworldPosition().z - player_->GetworldPosition().z;

	// ベクトルの正規化
	float length =
		sqrt((distance.x * distance.x) + (distance.y * distance.y) + (distance.z * distance.z));
	Vector3 dir(distance.x / length, distance.y / length, distance.z / length);

	// ベクトルの長さを、早さに合わせる
	Vector3 velocity(dir.x * kBulletSpeed, dir.y * kBulletSpeed, dir.z * kBulletSpeed);

	// 弾を生成し、初期化
	EnemyBullet* newBullet = new EnemyBullet();
	newBullet->Initialize(model_, worldTransform_.translation_, velocity);
	//bullet_ = newBullet;

	// 弾を登録する
	//bullets_.push_back(newBullet);
	gameScene_->AddEnemyBullet(newBullet);
}

Enemy::~Enemy() {}

void Enemy::Approach() { pushTimer = kFireInterval; }

Vector3 Enemy::GetworldPosition() {

	Vector3 worldPos = {};

	worldPos.x = worldTransform_.translation_.x;
	worldPos.y = worldTransform_.translation_.y;
	worldPos.z = worldTransform_.translation_.z;

	return worldPos;
}
void Enemy::OnCollision() {
	
	isDead_ = true;
	audio_->PlayWave(enemygamagesound);

}

//Enemy::~Enemy() {
//	for (EnemyBullet* bullet : bullets_) {
//		delete bullet;
//	}
//}


