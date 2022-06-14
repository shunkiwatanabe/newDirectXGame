#include "TextureManager.h"
#include <cassert>
#include "AxisIndicator.h"
#include "PrimitiveDrawer.h"
#include "Enemy.h"

Matrix4 Enemy::CreateMatScale(Vector3 scale)
{
	//�s��̍X�V
	Matrix4 matIdentity;
	matIdentity.m[0][0] = 1;
	matIdentity.m[1][1] = 1;
	matIdentity.m[2][2] = 1;
	matIdentity.m[3][3] = 1;

	Matrix4 matScale = matIdentity;
	matScale.m[0][0] = worldTransform_.scale_.x;
	matScale.m[1][1] = worldTransform_.scale_.y;
	matScale.m[2][2] = worldTransform_.scale_.z;

	worldTransform_.matWorld_ = matIdentity;
	//worldTransform_.matWorld_ *= matScale;
	return matScale;
}

Matrix4 Enemy::CreateMatRotX(Vector3 rotation)
{
	//�s��̍X�V
	Matrix4 matIdentity;
	matIdentity.m[0][0] = 1;
	matIdentity.m[1][1] = 1;
	matIdentity.m[2][2] = 1;
	matIdentity.m[3][3] = 1;

	Matrix4 matRotX = matIdentity;
	matRotX.m[1][1] = cos(worldTransform_.rotation_.x);
	matRotX.m[1][2] = sin(worldTransform_.rotation_.x);
	matRotX.m[2][1] = -sin(worldTransform_.rotation_.x);
	matRotX.m[2][2] = cos(worldTransform_.rotation_.x);

	worldTransform_.matWorld_ = matIdentity;
	//worldTransform_.matWorld_ *= matRotX;

	return matRotX;
}

Matrix4 Enemy::CreateMatRotY(Vector3 rotation)
{
	//�s��̍X�V
	Matrix4 matIdentity;
	matIdentity.m[0][0] = 1;
	matIdentity.m[1][1] = 1;
	matIdentity.m[2][2] = 1;
	matIdentity.m[3][3] = 1;

	Matrix4 matRotY = matIdentity;
	matRotY.m[0][0] = cos(worldTransform_.rotation_.y);
	matRotY.m[0][2] = sin(worldTransform_.rotation_.y);
	matRotY.m[2][0] = -sin(worldTransform_.rotation_.y);
	matRotY.m[2][2] = cos(worldTransform_.rotation_.y);

	worldTransform_.matWorld_ = matIdentity;
	//worldTransform_.matWorld_ *= matRotY;

	return matRotY;
}
Matrix4 Enemy::CreateMatRotZ(Vector3 rotation)
{
	//�s��̍X�V
	Matrix4 matIdentity;
	matIdentity.m[0][0] = 1;
	matIdentity.m[1][1] = 1;
	matIdentity.m[2][2] = 1;
	matIdentity.m[3][3] = 1;

	Matrix4 matRotZ = matIdentity;
	matRotZ.m[0][0] = cos(worldTransform_.rotation_.z);
	matRotZ.m[0][1] = sin(worldTransform_.rotation_.z);
	matRotZ.m[1][0] = -sin(worldTransform_.rotation_.z);
	matRotZ.m[1][1] = cos(worldTransform_.rotation_.z);

	worldTransform_.matWorld_ = matIdentity;
	//worldTransform_.matWorld_ *= matRotZ;

	return matRotZ;
}

Matrix4 Enemy::CreateMatTrans(Vector3 tranlation)
{
	//�s��̍X�V
	Matrix4 matIdentity;
	matIdentity.m[0][0] = 1;
	matIdentity.m[1][1] = 1;
	matIdentity.m[2][2] = 1;
	matIdentity.m[3][3] = 1;

	Matrix4 matTrans = matIdentity;
	matTrans.m[3][0] = worldTransform_.translation_.x;
	matTrans.m[3][1] = worldTransform_.translation_.y;
	matTrans.m[3][2] = worldTransform_.translation_.z;
	matTrans.m[3][3] = 1;

	worldTransform_.matWorld_ = matIdentity;
	//worldTransform_.matWorld_ *= matTrans;

	return matTrans;
}

void Enemy::approach(WorldTransform& worldtransform, Vector3 appSpeed)
{
	//�ړ�(�x�N�g�����Z)
	worldTransform_.translation_ += appSpeed;
	//����̈ʒu�ɒB�����痣�E
	if (worldTransform_.translation_.z < 0)
	{
		phase_ = Phase::Leave;
	}
}

void Enemy::leave(WorldTransform& worldtransform, Vector3 leaveSpeed)
{
	//�ړ�(�x�N�g�������Z)
	worldTransform_.translation_ += leaveSpeed;
}

void Enemy::Initialize(Model* model/*, const Vector3& position*/)
{
	//NULL�|�C���^�`�F�b�N
	assert(model);

	model_ = model;

	//�e�N�X�`���ǂݍ���
	textureHandle_ = TextureManager::Load("black.png");


	worldTransform_.translation_ = { 0,2,10 };
	//��-���h�g�����X�t�H�[���̏�����
	worldTransform_.Initialize();

	////�����Ŏ󂯎�����������W���Z�b�g
	//worldTransform_.translation_ = position;
}

void Enemy::Update()
{

	////�ړ�����
	//�ړ��̑���
	const float kEnemySpeed = 0.2f;
	//���W���ړ�������(1�t���[���̈ړ��ʂ𑫂�����)
	worldTransform_.translation_.z -= kEnemySpeed;

	worldTransform_.matWorld_ *= CreateMatScale(worldTransform_.scale_);
	worldTransform_.matWorld_ *= CreateMatRotZ(worldTransform_.rotation_);
	worldTransform_.matWorld_ *= CreateMatRotX(worldTransform_.rotation_);
	worldTransform_.matWorld_ *= CreateMatRotY(worldTransform_.rotation_);
	worldTransform_.matWorld_ *= CreateMatTrans(worldTransform_.translation_);

	/*//�s��̍X�V
	Matrix4 matIdentity;
	matIdentity.m[0][0] = 1;
	matIdentity.m[1][1] = 1;
	matIdentity.m[2][2] = 1;
	matIdentity.m[3][3] = 1;

	Matrix4 matScale = matIdentity;
	matScale.m[0][0] = worldTransform_.scale_.x;
	matScale.m[1][1] = worldTransform_.scale_.y;
	matScale.m[2][2] = worldTransform_.scale_.z;

	Matrix4 matRotZ = matIdentity;
	matRotZ.m[0][0] = cos(worldTransform_.rotation_.z);
	matRotZ.m[0][1] = sin(worldTransform_.rotation_.z);
	matRotZ.m[1][0] = -sin(worldTransform_.rotation_.z);
	matRotZ.m[1][1] = cos(worldTransform_.rotation_.z);

	Matrix4 matRotX = matIdentity;
	matRotX.m[1][1] = cos(worldTransform_.rotation_.x);
	matRotX.m[1][2] = sin(worldTransform_.rotation_.x);
	matRotX.m[2][1] = -sin(worldTransform_.rotation_.x);
	matRotX.m[2][2] = cos(worldTransform_.rotation_.x);

	Matrix4 matRotY = matIdentity;
	matRotY.m[0][0] = cos(worldTransform_.rotation_.y);
	matRotY.m[0][2] = sin(worldTransform_.rotation_.y);
	matRotY.m[2][0] = -sin(worldTransform_.rotation_.y);
	matRotY.m[2][2] = cos(worldTransform_.rotation_.y);

	Matrix4 matTrans = matIdentity;
	matTrans.m[3][0] = worldTransform_.translation_.x;
	matTrans.m[3][1] = worldTransform_.translation_.y;
	matTrans.m[3][2] = worldTransform_.translation_.z;
	matTrans.m[3][3] = 1;

	worldTransform_.matWorld_ = matIdentity;
	worldTransform_.matWorld_ *= matScale;

	worldTransform_.matWorld_ *= matRotZ;
	worldTransform_.matWorld_ *= matRotX;
	worldTransform_.matWorld_ *= matRotY;

	worldTransform_.matWorld_ *= matTrans;*/

	worldTransform_.TransferMatrix();


	Vector3 approachSpeed = { 0,0,0 };
	Vector3 leaveSpeed = { -0.05f,0.05f,0 };
	//switch���ɂ�����
	switch (phase_)
	{

	case Phase::Approach:
	default:
		approach(worldTransform_, approachSpeed);
		break;
	case Phase::Leave:
		leave(worldTransform_, leaveSpeed);
		break;
	}

}

void Enemy::Draw(const ViewProjection& viewProjection)
{
	model_->Draw(worldTransform_, viewProjection, textureHandle_);
}