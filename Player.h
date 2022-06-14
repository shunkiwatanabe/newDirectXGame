#pragma once
#include"Model.h"
#include"WorldTransform.h"
#include "Input.h"
#include "DebugText.h"
#include "ViewProjection.h"
#include "PlayerBullet.h"
#include <memory>
#include <list>

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

	/*Matrix4 CreateMatScale(Vector3 scale);
	Matrix4 CreateMatRotX(Vector3 rotation);
	Matrix4 CreateMatRotY(Vector3 rotation);
	Matrix4 CreateMatRotZ(Vector3 rotation);
	Matrix4 CreateMatTrans(Vector3 translation);

	Matrix4 CreateMatVector(Vector3 velocity, Matrix4 matWorld);*/

	Vector3 Velocity(Vector3 velocity, WorldTransform worldTransform);

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

	//�e
	//PlayerBullet* bullet_ = nullptr;
	//std::unique_ptr<PlayerBullet>bullet_;
	std::list < std::unique_ptr < PlayerBullet >>bullets_;
};