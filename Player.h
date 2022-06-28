#pragma once
#include"Model.h"
#include"WorldTransform.h"
#include "Input.h"
#include "DebugText.h"
#include "ViewProjection.h"
#include "PlayerBullet.h"
#include <memory>
#include <list>
#include "Matrix4.h"

///<summary>
///���L����
/// </summary>

class Player {
public:
	///<summary>
	///������
	/// </summary>
	/// <param name="model">���f��</param>
	/// <param name="textureHandle">�e�N�X�`���n���h��</param>
	void Initialize(Model* model, uint32_t textureHandle);

	///<summary>
	///�X�V
	/// </summary>
	void Update();

	///<summary>
	///��]
	/// </summary>
	void Rotate();

	/// <summary>
	/// �U��
	/// </summary>
	void Attack();

	///<summary>
	///�`��
	/// </summary>
	void Draw(ViewProjection& viewProjection);


	Vector3 Velocity(Vector3 velocity, WorldTransform worldTransform);

	Vector3 GetWorldPosition();

private://�����o�ϐ�
	//���[���h�ϊ��f�[�^
	WorldTransform worldTransform_;
	//���f��
	Model* model_ = nullptr;
	//�e�N�X�`���n���h��
	uint32_t textureHandle_ = 0u;
	//���͂��邽��
	Input* input_ = nullptr;
	//�f�o�b�O�e�L�X�g
	DebugText* debugText_ = nullptr;

	ViewProjection* viewProjection_;

	Matrix4* matrix_ = nullptr;


	//�e
	//PlayerBullet* bullet_ = nullptr;
	//std::unique_ptr<PlayerBullet>bullet_;
	std::list < std::unique_ptr < PlayerBullet >>bullets_;
};