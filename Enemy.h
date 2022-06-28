#pragma once

#include"Model.h"
#include"WorldTransform.h"
#include "Input.h"
#include "DebugText.h"
#include "ViewProjection.h"
#include "EnemyBullet.h"
#include "Matrix4.h"

class Player;

///<summary>
///�G
/// </summary>
class Enemy {
public:
	///<>summary
	///������
	/// </summary>
	/// <param name="model">���f��</param>
	/// <param name="mposition">�������W</param>
	void Initialize(Model* model/*, const Vector3& position*/);

	///<>summary
	///�X�V
	/// </summary>
	void Update();

	///<>summary
	///�`��
	/// </summary>
	/// <param name="viewProjection">�r���[�v���W�F�N�V����</param>
	void Draw(const ViewProjection& viewProjection);

	Vector3 Velocity(Vector3 velocity, WorldTransform worldTransform);

	void approach(WorldTransform& worldtransform, Vector3 appSpeed);
	void leave(WorldTransform& worldtransform, Vector3 leaveSpeed);

	void SetPlayer(Player* player) { player_ = player; };

	Vector3 GetWorldPosition();

	///<>summary
	///�e����
	/// </summary>
	void Fire();

	//���ˊ��o
	static const int kFireInterval = 60;

	//�ڋ߃t�F�[�Y������
	void ApproachInitialize();

private:
	//���[���h�ϊ��f�[�^
	WorldTransform worldTransform_;
	//���f���̃|�C���^(�؂�Ă�����)
	Model* model_ = nullptr;
	//�e�N�X�`���n���h��
	uint32_t textureHandle_ = 0u;
	//���͂��邽��
	Input* input_ = nullptr;

	Matrix4* matrix_ = nullptr;

	enum class Phase {
		Approach,	//�ڋ߂���
		Leave,		//���E����
	};
	Phase phase_ = Phase::Approach;

	std::list < std::unique_ptr < EnemyBullet >>bullets_;

	//���˃^�C�}�[
	int32_t shotTimer = 0;

	Player* player_ = nullptr;
};