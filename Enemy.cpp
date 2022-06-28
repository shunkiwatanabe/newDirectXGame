#include "TextureManager.h"
#include <cassert>
#include "AxisIndicator.h"
#include "PrimitiveDrawer.h"
#include "Enemy.h"
#include "Player.h"

//Matrix4 Enemy::CreateMatScale(Vector3 scale)
//{
//	//�s��̍X�V
//	Matrix4 matIdentity;
//	matIdentity.m[0][0] = 1;
//	matIdentity.m[1][1] = 1;
//	matIdentity.m[2][2] = 1;
//	matIdentity.m[3][3] = 1;
//
//	Matrix4 matScale = matIdentity;
//	matScale.m[0][0] = worldTransform_.scale_.x;
//	matScale.m[1][1] = worldTransform_.scale_.y;
//	matScale.m[2][2] = worldTransform_.scale_.z;
//
//	worldTransform_.matWorld_ = matIdentity;
//	//worldTransform_.matWorld_ *= matScale;
//	return matScale;
//}

//Matrix4 Enemy::CreateMatRotX(Vector3 rotation)
//{
//	//�s��̍X�V
//	Matrix4 matIdentity;
//	matIdentity.m[0][0] = 1;
//	matIdentity.m[1][1] = 1;
//	matIdentity.m[2][2] = 1;
//	matIdentity.m[3][3] = 1;
//
//	Matrix4 matRotX = matIdentity;
//	matRotX.m[1][1] = cos(worldTransform_.rotation_.x);
//	matRotX.m[1][2] = sin(worldTransform_.rotation_.x);
//	matRotX.m[2][1] = -sin(worldTransform_.rotation_.x);
//	matRotX.m[2][2] = cos(worldTransform_.rotation_.x);
//
//	worldTransform_.matWorld_ = matIdentity;
//	//worldTransform_.matWorld_ *= matRotX;
//
//	return matRotX;
//}

//Matrix4 Enemy::CreateMatRotY(Vector3 rotation)
//{
//	//�s��̍X�V
//	Matrix4 matIdentity;
//	matIdentity.m[0][0] = 1;
//	matIdentity.m[1][1] = 1;
//	matIdentity.m[2][2] = 1;
//	matIdentity.m[3][3] = 1;
//
//	Matrix4 matRotY = matIdentity;
//	matRotY.m[0][0] = cos(worldTransform_.rotation_.y);
//	matRotY.m[0][2] = sin(worldTransform_.rotation_.y);
//	matRotY.m[2][0] = -sin(worldTransform_.rotation_.y);
//	matRotY.m[2][2] = cos(worldTransform_.rotation_.y);
//
//	worldTransform_.matWorld_ = matIdentity;
//	//worldTransform_.matWorld_ *= matRotY;
//
//	return matRotY;
//}

//Matrix4 Enemy::CreateMatRotZ(Vector3 rotation)
//{
//	//�s��̍X�V
//	Matrix4 matIdentity;
//	matIdentity.m[0][0] = 1;
//	matIdentity.m[1][1] = 1;
//	matIdentity.m[2][2] = 1;
//	matIdentity.m[3][3] = 1;
//
//	Matrix4 matRotZ = matIdentity;
//	matRotZ.m[0][0] = cos(worldTransform_.rotation_.z);
//	matRotZ.m[0][1] = sin(worldTransform_.rotation_.z);
//	matRotZ.m[1][0] = -sin(worldTransform_.rotation_.z);
//	matRotZ.m[1][1] = cos(worldTransform_.rotation_.z);
//
//	worldTransform_.matWorld_ = matIdentity;
//	//worldTransform_.matWorld_ *= matRotZ;
//
//	return matRotZ;
//}

//Matrix4 Enemy::CreateMatTrans(Vector3 tranlation)
//{
//	�s��̍X�V
//	Matrix4 matIdentity;
//	matIdentity.m[0][0] = 1;
//	matIdentity.m[1][1] = 1;
//	matIdentity.m[2][2] = 1;
//	matIdentity.m[3][3] = 1;
//
//	Matrix4 matTrans = matIdentity;
//	matTrans.m[3][0] = worldTransform_.translation_.x;
//	matTrans.m[3][1] = worldTransform_.translation_.y;
//	matTrans.m[3][2] = worldTransform_.translation_.z;
//	matTrans.m[3][3] = 1;
//
//	worldTransform_.matWorld_ = matIdentity;
//	worldTransform_.matWorld_ *= matTrans;
//
//	return matTrans;
//}

Vector3 Enemy::Velocity(Vector3 velocity, WorldTransform worldTransform)
{
	Vector3 v;
	v.x = velocity.x * worldTransform.matWorld_.m[0][0] + velocity.y * worldTransform.matWorld_.m[1][0] + velocity.z * worldTransform.matWorld_.m[2][0];
	v.y = velocity.x * worldTransform.matWorld_.m[0][1] + velocity.y * worldTransform.matWorld_.m[1][1] + velocity.z * worldTransform.matWorld_.m[2][1];
	v.z = velocity.x * worldTransform.matWorld_.m[0][2] + velocity.y * worldTransform.matWorld_.m[1][2] + velocity.z * worldTransform.matWorld_.m[2][2];

	return v;
}

void Enemy::approach(WorldTransform& worldtransform, Vector3 appSpeed)
{
	//�ړ�(�x�N�g�����Z)
	worldTransform_.translation_ += appSpeed;
	//����̈ʒu�ɒB�����痣�E
	/*if (worldTransform_.translation_.z < 0)
	{
		phase_ = Phase::Leave;
	}*/
	//���˃^�C�}�[�J�E���g�_�E��
	shotTimer--;
	//�w�莞�ԂɒB����
	if (shotTimer < 0)
	{
		//�e�𔭎� 
		Fire();
		//���˃^�C�}�[��������
		shotTimer = kFireInterval;
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

	worldTransform_.translation_ = { 10,2,30 };
	//��-���h�g�����X�t�H�[���̏�����
	worldTransform_.Initialize();


	//�e�𔭎�
	//Fire();

	//�ڋ߃t�F�[�Y������
	ApproachInitialize();

}

void Enemy::Update()
{
	//�f�X�t���O�̗����������폜
	bullets_.remove_if([](std::unique_ptr<EnemyBullet>& bullet)
		{
			return bullet->IsDead();
		});

	////�ړ�����
	//�ړ��̑���
	float kEnemySpeed = 0.02f;
	//���W���ړ�������(1�t���[���̈ړ��ʂ𑫂�����)
	worldTransform_.translation_.x -= kEnemySpeed;

	//�e�X�V
	for (std::unique_ptr<EnemyBullet>& bullet : bullets_)
	{
		bullet->Update();
	}

	Vector3 approachSpeed = { 0,0,0 };
	Vector3 leaveSpeed = { -0.05f,0.05f,0 };


	//switch���ɂ�����
	/*switch (phase_)
	{

	case Phase::Approach:
	default:
		approach(worldTransform_, approachSpeed);
		break;
	case Phase::Leave:
		leave(worldTransform_, leaveSpeed);
		break;
	}*/

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

	worldTransform_.matWorld_ *= matTrans;

	worldTransform_.TransferMatrix();

	approach(worldTransform_, approachSpeed);
	//worldTransform_.matWorld_ = matrix_->CreateMatrix(worldTransform_.scale_, worldTransform_.rotation_, worldTransform_.translation_, worldTransform_);

	//worldTransform_.matWorld_ *= matrix_->CreateMatScale(worldTransform_.scale_,worldTransform_);
	//worldTransform_.matWorld_ *= matrix_->CreateMatRotZ(worldTransform_.rotation_, worldTransform_);
	//worldTransform_.matWorld_ *= matrix_->CreateMatRotX(worldTransform_.rotation_, worldTransform_);
	//worldTransform_.matWorld_ *= matrix_->CreateMatRotY(worldTransform_.rotation_, worldTransform_);
	//worldTransform_.matWorld_ *= matrix_->CreateMatTrans(worldTransform_.translation_, worldTransform_);
}

void Enemy::Draw(const ViewProjection& viewProjection)
{
	model_->Draw(worldTransform_, viewProjection, textureHandle_);

	//�e�`��
	for (std::unique_ptr<EnemyBullet>& bullet : bullets_)
	{
		bullet->Draw(viewProjection);
	}
}

void Enemy::Fire()
{
	//�e�̑��x
	const float kBulletSpeed = -0.1f;
	Vector3 velocity(0, 0, kBulletSpeed);

	////���x�x�N�g�������@�̌����ɍ��킹�ĉ�]������
	//velocity = Velocity(velocity, worldTransform_);

	/*player_->GetWorldPosition();
	GetWorldPosition();
	Vector3 velocity = {
		player_->GetWorldPosition().x - GetWorldPosition().x,
		player_->GetWorldPosition().y - GetWorldPosition().y,
		player_->GetWorldPosition().z - GetWorldPosition().z
	};

	velocity.normalize();
	velocity.x *= kBulletSpeed;
	velocity.y *= kBulletSpeed;
	velocity.z *= kBulletSpeed;*/

	player_->GetWorldPosition();
	GetWorldPosition();
	velocity = {
		player_->GetWorldPosition().x - GetWorldPosition().x,
		player_->GetWorldPosition().y - GetWorldPosition().y,
		player_->GetWorldPosition().z - GetWorldPosition().z
	};

	float length = sqrtf(velocity.x * velocity.x + velocity.y * velocity.y + velocity.z * velocity.z);
	velocity /= length;

	velocity *= -kBulletSpeed;

	//�e�𐶐����A������
	std::unique_ptr<EnemyBullet>newBullet = std::make_unique<EnemyBullet>();
	newBullet->Initialize(model_, worldTransform_.translation_, velocity);

	//�e��o�^����
	bullets_.push_back(std::move(newBullet));
}

void Enemy::ApproachInitialize()
{
	//���˃^�C�}�[��������
	shotTimer = kFireInterval;
}

Vector3 Enemy::GetWorldPosition()
{
	//���[���h���W������ϐ�
	Vector3 worldPos;
	//���[���h�s��̕��s�ړ��������擾(���[���h���W)
	worldPos.x = worldTransform_.translation_.x;
	worldPos.y = worldTransform_.translation_.y;
	worldPos.z = worldTransform_.translation_.z;

	return worldPos;
}

void Enemy::OnCollision()
{
	//�������Ȃ�
}