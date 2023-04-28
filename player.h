#pragma once
/// <summary>
/// ���L����
/// </summary>

#include "Model.h"
#include "ViewProjection.h"
#include "WorldTransform.h"

#include "Input.h"

class Player {
public:
	/// <summary>
	/// ������
	/// </summary>
	void Initialize(Model* model, uint32_t textureHandle);

	/// <summary>
	/// �X�V
	/// </summary>
	void Update();

	/// <summary>
	/// �`��
	/// </summary>
	void Draw(ViewProjection& viewProjection);

private:
	// ���[���h�ϊ��f�[�^
	WorldTransform worldTransform_;

	// ���f��
	Model* model_ = nullptr;

	// �e�N�X�`���n���h��
	uint32_t textureHandle_ = 0u;

	// �L�[�{�[�h����
	Input* input_ = nullptr;
};