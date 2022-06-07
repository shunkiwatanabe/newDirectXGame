#pragma once
#include"Model.h"
#include"WorldTransform.h"
#include "Input.h"
#include "DebugText.h"
#include "ViewProjection.h"

///<>summary
///���L�����̒e
/// </summary>

class PlayerBullet
{
public:
	///<>summary
	///������
	/// </summary>
	/// <param name="model">���f��</param>
	/// <param name="mposition">�������W</param>
	void Initialize(Model* model, const Vector3& position, const Vector3& velocity);

	///<>summary
	///�X�V
	/// </summary>
	void Update();

	///<>summary
	///�`��
	/// </summary>
	/// <param name="viewProjection">�r���[�v���W�F�N�V����</param>
	void Draw(const ViewProjection& viewProjection);

	bool IsDead()const { return isDead_; }

private:
	//���[���h�ϊ��f�[�^
	WorldTransform worldTransform_;
	//���f���̃|�C���^(�؂�Ă�����)
	Model* model_ = nullptr;
	//�e�N�X�`���n���h��
	uint32_t textureHandle_ = 0u;

	//���x
	Vector3 velocity_;

	//����<frm>
	static const int32_t kLifeTime = 60 * 5;

	//�f�X�^�C�}�[
	int32_t deathTimer_ = kLifeTime;
	//�f�X�t���O
	bool isDead_ = false;
};