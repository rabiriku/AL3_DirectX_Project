#include "player.h"
#include "ImGui.h"
#include <cassert>

void Player::Initialize(Model* model, uint32_t textureHandle) {
	// NULL�|�C���^�`�F�b�N
	assert(model);

	// �󂯓n��
	model_ = model;
	textureHandle_ = textureHandle;

	// ���[���h������
	worldTransform_.Initialize();

	// �V���O���g���C���X�^���X���擾����
	input_ = Input::GetInstance();
}

void Player::Update() {
	// �L�����N�^�[�̈ړ��x�N�g��
	Vector3 move = {0, 0, 0};
	// �L�����N�^�[�̈ړ�����
	const float kCharacterSpeed = 0.4f;
	// �����������ňړ��x�N�g����ύX(���E)
	if (input_->PushKey(DIK_LEFT)) {
		move.x -= kCharacterSpeed;
	} else if (input_->PushKey(DIK_RIGHT)) {
		move.x += kCharacterSpeed;
	}

	// �͈͐���
	const float kMoveLimitX = 20.0f;
	const float kMoveLimitY = 20.0f;

	// �͈͂𒴂��Ȃ�����
	worldTransform_.translation_.x = max(worldTransform_.translation_.x, -kMoveLimitX);
	worldTransform_.translation_.x = min(worldTransform_.translation_.x, +kMoveLimitX);

	worldTransform_.translation_.y = max(worldTransform_.translation_.y, -kMoveLimitY);
	worldTransform_.translation_.y = min(worldTransform_.translation_.y, +kMoveLimitY);

	// ���W�ړ�(�x�N�g���̉��Z)
	worldTransform_.translation_.x += move.x;
	worldTransform_.translation_.y += move.y;
	worldTransform_.translation_.z += move.z;

	worldTransform_.matWorld_ = MakeAffineMatrix(
	    worldTransform_.scale_, worldTransform_.rotation_, worldTransform_.translation_);

	// �s���萔�o�b�t�@�ɓ]��
	worldTransform_.TransferMatrix();

#ifdef _DEBUG
	// InGui
	ImGui::SetNextWindowPos({0, 0});
	ImGui::SetNextWindowSize({300, 100});

	// ���W�̐ݒ�
	ImGui::Begin("Player");
	float sliderValue[3] = {
	    worldTransform_.translation_.x, worldTransform_.translation_.y,
	    worldTransform_.translation_.z};
	ImGui::SliderFloat3("position", sliderValue, -20.0f, 20.0f);
	worldTransform_.translation_ = {sliderValue[0], sliderValue[1], sliderValue[2]};
	ImGui::End();
#endif // DEBUG
}

void Player::Draw(ViewProjection& viewProjection) {
	model_->Draw(worldTransform_, viewProjection, textureHandle_);
}