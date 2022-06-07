#include "TextureManager.h"
#include <cassert>
#include "AxisIndicator.h"
#include "PrimitiveDrawer.h"
#include "Player.h"

//Matrix4 CreateMatScale(Vector3 scale);
//Matrix4 CreateMatRotation(Vector3 rotation);
//Matrix4 CreateMatTranslation(Vector3 transration);
//Matrix4 CreateMatWorld(Vector3 scale, Vector3 rotation, Vector3 translation);


void Player::Initialize(Model* model, uint32_t textureHandle)
{
	//NULL�|�C���^�`�F�b�N
	assert(model);

	//�����Ƃ��Ď󂯎�����f�[�^�������o�ϐ��ɋL�^����
	model_ = model;
	textureHandle_ = textureHandle;

	//�V���O���g���C���X�^���X���擾����
	input_ = Input::GetInstance();
	debugText_ = DebugText::GetInstance();

	//���[���h�ϊ��̏�����
	worldTransform_.Initialize();
}

void Player::Update()
{
	//�L�����N�^�[�̈ړ��x�N�g��
	Vector3 move = { 0,0,0 };

	//�ړ��x�N�g����ύX���鏈��
	//�L�����N�^�[�̈ړ�����
	const float kCharacterSpeed = 0.2f;

	//�����������ňړ��x�N�g����ύX
	if (input_->PushKey(DIK_A)) {
		move = { -kCharacterSpeed,0,0 };
	}
	else if (input_->PushKey(DIK_D)) {
		move = { kCharacterSpeed,0,0 };
	}

	//�����������ňړ��x�N�g����ύX
	if (input_->PushKey(DIK_S)) {
		move = { 0,-kCharacterSpeed,0 };
	}
	else if (input_->PushKey(DIK_W)) {
		move = { 0,kCharacterSpeed,0 };
	}

	//�L�����N�^�[�U������
	Attack();

	//�e�X�V
	if (bullet_)
	{
		bullet_->Update();
	}

	//���W�ړ�(�x�N�g���̉��Z)
	worldTransform_.translation_.x += move.x;
	worldTransform_.translation_.y += move.y;
	worldTransform_.translation_.z += move.z;

	//�ړ����E���W
	const float kMoveLimitX = 34.0f;
	const float kMoveLimitY = 18.0f;

	//�͈͂𒴂��Ȃ�����
	worldTransform_.translation_.x = max(worldTransform_.translation_.x, -kMoveLimitX);
	worldTransform_.translation_.x = min(worldTransform_.translation_.x, +kMoveLimitX);
	worldTransform_.translation_.y = max(worldTransform_.translation_.y, -kMoveLimitY);
	worldTransform_.translation_.y = min(worldTransform_.translation_.y, +kMoveLimitY);

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
	//matIdentity.m[3][3] = 1;

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

	/*Matrix4 matScale = CreateMatScale(worldTransform_.scale_);
	Matrix4 matRotation = CreateMatRotation(worldTransform_.rotation_);
	Matrix4 matTransration = CreateMatTranslation(worldTransform_.translation_);*/

	worldTransform_.matWorld_ = matIdentity;
	worldTransform_.matWorld_ *= matScale;

	worldTransform_.matWorld_ *= matRotZ;
	worldTransform_.matWorld_ *= matRotX;
	worldTransform_.matWorld_ *= matRotY;

	worldTransform_.matWorld_ *= matTrans;

	worldTransform_.TransferMatrix();

	debugText_->SetPos(50, 50);
	debugText_->Printf("translation:(%f,%f,%f)", worldTransform_.translation_.x, worldTransform_.translation_.y, worldTransform_.translation_.z);
}

void Player::Rotate()
{
	const float rotateSpeed = 0.05f;
	if (input_->PushKey(DIK_RIGHT))
	{
		worldTransform_.rotation_.y += rotateSpeed;
	}
	else if (input_->PushKey(DIK_LEFT))
	{
		worldTransform_.rotation_.y -= rotateSpeed;
	}

	worldTransform_.TransferMatrix();
}

void Player::Attack()
{
	if (input_->PushKey(DIK_SPACE))
	{
		//�e�𐶐����A������
		PlayerBullet* newBullet = new PlayerBullet();
		newBullet->Initialize(model_, worldTransform_.translation_);

		//�e��o�^����
		bullet_ = newBullet;
	}
	//worldTransform_.TransferMatrix();
}

void Player::Draw(ViewProjection viewProjection)
{
	model_->Draw(worldTransform_, viewProjection, textureHandle_);

	//�e�`��
	if (bullet_)
	{
		bullet_->Draw(viewProjection);
	}
}

//Matrix4 CreateMatScale(Vector3 scale)
//{
//	Matrix4 matIdentity;
//	matIdentity.m[0][0] = 1;
//	matIdentity.m[1][1] = 1;
//	matIdentity.m[2][2] = 1;
//	matIdentity.m[3][3] = 1;
//
//	Matrix4 matScale = matIdentity;
//	matScale.m[0][0] = scale.x;
//	matScale.m[1][1] = scale.y;
//	matScale.m[2][2] = scale.z;
//	matScale.m[3][3] = 1;
//
//	return matScale;
//}

//Matrix4 CreateMatRotation(Vector3 rotation)
//{
//	Matrix4 matIdentity;
//	matIdentity.m[0][0] = 1;
//	matIdentity.m[1][1] = 1;
//	matIdentity.m[2][2] = 1;
//	matIdentity.m[3][3] = 1;
//
//	Matrix4 matRotZ = matIdentity;
//	matRotZ.m[0][0] = cos(rotation.z);
//	matRotZ.m[0][1] = sin(rotation.z);
//	matRotZ.m[1][0] = -sin(rotation.z);
//	matRotZ.m[1][1] = cos(rotation.z);
//
//	Matrix4 matRotX = matIdentity;
//	matRotX.m[1][1] = cos(rotation.x);
//	matRotX.m[1][2] = sin(rotation.x);
//	matRotX.m[2][1] = -sin(rotation.x);
//	matRotX.m[2][2] = cos(rotation.x);
//
//	Matrix4 matRotY = matIdentity;
//	matRotY.m[0][0] = cos(rotation.y);
//	matRotY.m[0][2] = sin(rotation.y);
//	matRotY.m[2][0] = -sin(rotation.y);
//	matRotY.m[2][2] = cos(rotation.y);
//
////	return matRotX * matRotY * matRotZ;
//}

//Matrix4 CreateMatTranslation(Vector3 translation)
//{
//	Matrix4 matIdentity;
//	matIdentity.m[0][0] = 1;
//	matIdentity.m[1][1] = 1;
//	matIdentity.m[2][2] = 1;
//	matIdentity.m[3][3] = 1;
//
//	Matrix4 matTranslation = matIdentity;
//	matTranslation.m[0][0] = translation.x;
//	matTranslation.m[1][1] = translation.y;
//	matTranslation.m[2][2] = translation.z;
//	matTranslation.m[3][3] = 1;
//
//	return matTranslation;
//}

//Matrix4 CreateMatWorld(Matrix4 matWorld, Vector3 scale,Vector3 rotation,Vector3 translation)
//{
//	Matrix4 matIdentity;
//	matWorld = matIdentity;
//	matWorld *= scale;
//
//	matWorld *= matRotZ;
//	matWorld *= matRotX;
//	matWorld *= matRotY;
//
//	matWorld *= matTrans;
//}