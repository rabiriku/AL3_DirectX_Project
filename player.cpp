#include "Player.h"
#include "ImGui.h"
#include <assert.h>


Player::~Player() {
	for (PlayerBullet* bullet : bullets_) {
		delete bullet;
	}

	for (Player* player : player_) {
		delete player;
	}

	delete sprite2DReticle_;
}



void Player::Initialize(Model* model, uint32_t textureHandle, Vector3 playerPosition) {
	// NULLポインタチェック
	assert(model);

	// 受け渡し
	model_ = model;

	textureHandle_ = textureHandle;
	// ワールド初期化
	worldTransform_.Initialize();

	// シングルトンインスタンスを取得する
	input_ = Input::GetInstance();
	audio_ = Audio::GetInstance();

	worldTransform_.translation_.x = playerPosition.x;
	worldTransform_.translation_.y = playerPosition.y;
	worldTransform_.translation_.z = playerPosition.z;

	worldTransform3DReticle_.Initialize();

	// レティクル用テクスチャ取得
	uint32_t textureReticle_ = TextureManager::Load("target.png");

	uint32_t hptexturehandle_ = TextureManager::Load("heart.png");

	// スプライト生成
	sprite2DReticle_ = Sprite::Create(textureReticle_, {640, 360}, {1, 1, 1, 1}, {0.5f, 0.5f});
	playerHpsprite_ = Sprite::Create(hptexturehandle_, {0, 650});
	playerHpsprite2_ = Sprite::Create(hptexturehandle_, {60, 650});
	playerHpsprite3_ = Sprite::Create(hptexturehandle_, {120, 650});
	playerHpsprite4_ = Sprite::Create(hptexturehandle_, {180, 650});
	playerHpsprite5_ = Sprite::Create(hptexturehandle_, {240, 650});

	// サウンド読み込み
	damagesound_ = audio_->LoadWave("playerhit.wav");
}


Vector3 Player::GetworldPosition() {

	Vector3 worldPos = {};

	worldPos.x = worldTransform_.matWorld_.m[3][0];
	worldPos.y = worldTransform_.matWorld_.m[3][1];
	worldPos.z = worldTransform_.matWorld_.m[3][2];

	return worldPos;
}


void Player::OnCollision() { 
	playerHp -= 1.0; 
	audio_->PlayWave(damagesound_);
}

Vector3 Player::GetWorldTransform3DReticle() {
	Vector3 worldPos = {};
	worldPos.x = worldTransform3DReticle_.matWorld_.m[3][0];
	worldPos.y = worldTransform3DReticle_.matWorld_.m[3][1];
	worldPos.z = worldTransform3DReticle_.matWorld_.m[3][2];
	return worldPos;
}

// 親子関係
void Player::SetParent(const WorldTransform* parent) { worldTransform_.parent_ = parent; }

void Player::DrawUI() {
	sprite2DReticle_->Draw(); 
	if (playerHp > 0) {
		playerHpsprite_->Draw();
	} 
	if (playerHp > 1) {
		playerHpsprite2_->Draw();
	} 
	if (playerHp > 2) {
		playerHpsprite3_->Draw();
	} 
	if (playerHp > 3) {
		playerHpsprite4_->Draw();
	} 
	if (playerHp>4){
		playerHpsprite5_->Draw();
	}
	
}


void Player::Update(ViewProjection& viewProjection) {
	bullets_.remove_if([](PlayerBullet* bullet) {
		if (bullet->IsDead()) {
			delete bullet;
			return true;
		}
		return false;
	});


	if (playerHp <= 0) {
		isDead_ = true;
	}
	
	
	// キャラクターの移動ベクトル
	Vector3 move = {0, 0, 0};
	// キャラクターの移動速さ
	const float kCharacterSpeed = 0.4f;
	// 押した方向で移動ベクトルを変更(左右)
	if (input_->PushKey(DIK_A)) {
		move.x -= kCharacterSpeed;
	} else if (input_->PushKey(DIK_D)) {
		move.x += kCharacterSpeed;
	} else if (input_->PushKey(DIK_W)) {
		move.y += kCharacterSpeed;
	} else if (input_->PushKey(DIK_S)) {
		move.y -= kCharacterSpeed;
	}

	// 旋回
	//const float matRotSpeed = 0.02f;
	//if (input_->PushKey(DIK_A)) {
	//	worldTransform_.rotation_.y -= matRotSpeed;
	//}
	//if (input_->PushKey(DIK_D)) {
	//	worldTransform_.rotation_.y += matRotSpeed;
	//}

	// 攻撃
	Attack();

	// 弾更新
	for (PlayerBullet* bullet : bullets_) {
		bullet->Update();
	}

	// 範囲制限
	const float kMoveLimitX = 35.0f;
	const float kMoveLimitY = 20.0f;

	// 範囲を超えない処理
	worldTransform_.translation_.x = max(worldTransform_.translation_.x, -kMoveLimitX);
	worldTransform_.translation_.x = min(worldTransform_.translation_.x, +kMoveLimitX);

	worldTransform_.translation_.y = max(worldTransform_.translation_.y, -kMoveLimitY);
	worldTransform_.translation_.y = min(worldTransform_.translation_.y, +kMoveLimitY);

	// 座標移動(ベクトルの加算)
	worldTransform_.translation_.x += move.x;
	worldTransform_.translation_.y += move.y;
	worldTransform_.translation_.z += move.z;

	worldTransform_.UpdateMatrix();

	// 自機から3Dレティクルへの距離
	const float kDistancePlayerTo3DReticle = 50.0f;

	// 自機から3Dレティクルへのオフセット(Z+向き)
	Vector3 offset = {0, 0, 1.0f};

	// 自機のワールド行列の回転を反映
	offset = TransformNormal(offset, worldTransform_.matWorld_);

	// ベクトルの長さを整える
	offset = Normalize(offset) * kDistancePlayerTo3DReticle;

	// 3Dレティクルの座標を設定
	worldTransform3DReticle_.translation_ = GetworldPosition() + offset;
	worldTransform3DReticle_.UpdateMatrix();

	// 3Dレティクルのワールド座標から2Dレティクルのスクリーン座標を計算
	Vector3 positionReticle = {
	    worldTransform3DReticle_.matWorld_.m[3][0], worldTransform3DReticle_.matWorld_.m[3][1],
	    worldTransform3DReticle_.matWorld_.m[3][2]};

	// ビューポート行列
	Matrix4x4 matViewport =
	    MakeViewportMatrix(0, 0, WinApp::kWindowWidth, WinApp::kWindowHeight, 0, 1);

	// ビュー行列とプロジェクション行列、ビューポート行列を合成する
	Matrix4x4 matViewProjectionViewport =
	    viewProjection.matView * viewProjection.matProjection * matViewport;

	// ワールド→スクリーン座標変換(ここで3Dから2Dになる)
	positionReticle = Transform(positionReticle, matViewProjectionViewport);

	// スプライトのレティクルに座標設定
	sprite2DReticle_->SetPosition(Vector2(positionReticle.x, positionReticle.y));

	// マウスカーソルのスクリーン座標からワールド座標を取得して3Dレティクル配置
	POINT mousePosition;
	// マウス座標(スクリーン座標)を取得する
	GetCursorPos(&mousePosition);

	// クライアントエリア座標に変換する
	HWND hwnd = WinApp::GetInstance()->GetHwnd();
	ScreenToClient(hwnd, &mousePosition);

	// マウス座標を2Dレティクルのスプライトに代入する
	sprite2DReticle_->SetPosition({(float)(mousePosition.x), (float)(mousePosition.y)});

	// ビュープロジェクションビューポート合成行列
	Matrix4x4 matVPV = matViewProjectionViewport;

	// 合成行列の逆行列を計算する
	Matrix4x4 matInverseVPV = Inverse(matVPV);

	// スクリーン座標
	Vector3 posNear = Vector3((float)(mousePosition.x), (float)(mousePosition.y), 0);
	Vector3 posFar = Vector3((float)(mousePosition.x), (float)(mousePosition.y), 1);

	// スクリーン座標系からワールド座標系へ
	posNear = Transform(posNear, matInverseVPV);
	posFar = Transform(posFar, matInverseVPV);

	// マウスレイの方向
	Vector3 mouseDirection = posFar - posNear;
	mouseDirection = Normalize(mouseDirection);

	// カメラから照準オブジェクトの距離
	const float kDistanceTestObject = 80;
	worldTransform3DReticle_.translation_ = posNear + mouseDirection * kDistanceTestObject;
	worldTransform3DReticle_.UpdateMatrix();


#ifdef _DEBUG
	// InGui
	ImGui::SetNextWindowPos({0, 0});
	ImGui::SetNextWindowSize({300, 100});

	// 座標の設定
	ImGui::Begin("Player");
	float sliderValue[3] = {
	    worldTransform_.translation_.x, worldTransform_.translation_.y,
	    worldTransform_.translation_.z};

	ImGui::SliderFloat3("position", sliderValue, -20.0f, 20.0f);
	ImGui::SliderFloat("hp", &playerHp, 0.f, 10.0f);
	//ImGui::Sliderbool("isDead", &isDead_, 0.f, 10.0f);
	worldTransform_.translation_ = {sliderValue[0], sliderValue[1], sliderValue[2]};
	ImGui::End();
#endif // DEBUG
}



void Player::Draw(ViewProjection& viewProjection) {
	model_->Draw(worldTransform_, viewProjection);

	model_->Draw(worldTransform3DReticle_, viewProjection);
	for (PlayerBullet* bullet : bullets_) {
		bullet->Draw(viewProjection);
	}
}



void Player::Attack() {
	if (input_->TriggerKey(DIK_SPACE)) {
		//弾の速度
		const float kBulletSpeed = 1.0f;
		Vector3 velocity(0, 0, kBulletSpeed);


		//velocity = TransformNormal(velocity,worldTransform_.matWorld_);

		// 自機から昇順オブジェクトへのベクトル
		velocity = GetWorldTransform3DReticle() - GetworldPosition();
		velocity = Normalize(velocity) * kBulletSpeed;

		// 弾を生成し、初期化
		PlayerBullet* newBullet = new PlayerBullet();
	
		newBullet->Initialize(model_, GetworldPosition(), velocity);


		// 弾を登録する
		bullets_.push_back(newBullet);
	}
}

void Player::reset() {


	playerHp = 5.0f;

	isDead_ = false;


}


