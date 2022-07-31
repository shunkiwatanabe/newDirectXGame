#include "RailCamera.h"

Vector3 RailCamera::Velocity(Vector3 velocity, Matrix4 matWorld_)
{
	Vector3 v;
	v.x = velocity.x * matWorld_.m[0][0] + velocity.y * matWorld_.m[1][0] + velocity.z * matWorld_.m[2][0];
	v.y = velocity.x * matWorld_.m[0][1] + velocity.y * matWorld_.m[1][1] + velocity.z * matWorld_.m[2][1];
	v.z = velocity.x * matWorld_.m[0][2] + velocity.y * matWorld_.m[1][2] + velocity.z * matWorld_.m[2][2];

	return v;
}

void RailCamera::Initialize(Vector3 position, Vector3 rotation)
{
	//���[���h�g�����X�t�H�[���̏�����
	//worldTransform_.Initialize();

	worldTransform_.translation_ = position;
	worldTransform_.rotation_ = rotation;

	viewProjection_.farZ = 2000.0f;
	viewProjection_.Initialize();
	debugText_ = DebugText::GetInstance();
}

void RailCamera::Update()
{
	Vector3 move = { 0,0,0 };

	//�ړ��X�s�[�h
	float kRailSpeed = 0.1f;

	//���W���ړ�������(1�t���[���̈ړ��ʂ𑫂�����)
	worldTransform_.translation_.x -= kRailSpeed;

	//��]�̃X�s�[�h
	const float rotateSpeed = 0.05f;
	//�p�x�����Z
	worldTransform_.rotation_.y += rotateSpeed;

	

	viewProjection_.eye = worldTransform_.translation_;

	//�ړ�(�x�N�g�����Z)
	worldTransform_.translation_ += Vector3(0, 0, 0.1f);
	//���[���h�g�����X�t�H�[�����X�V
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

	//���[���h�s��̕��s�ړ��������擾(���[���h���W)
	viewProjection_.eye.x = worldTransform_.matWorld_.m[3][0];
	viewProjection_.eye.y = worldTransform_.matWorld_.m[3][1];
	viewProjection_.eye.z = worldTransform_.matWorld_.m[3][2];

	//���[���h�O���x�N�g��
	Vector3 forward(0, 0, 1);
	//���[���J�����̉�]�𔽉f
	forward = Velocity(forward, worldTransform_.matWorld_);
	//���_����O���ɓK���ȋ����i�񂾈ʒu�������_
	viewProjection_.target.x = viewProjection_.eye.x + forward.x;
	viewProjection_.target.y = viewProjection_.eye.y + forward.y;
	viewProjection_.target.z = viewProjection_.eye.z + forward.z;
	//���[���h����x�N�g��
	Vector3 up(0, 1, 1);
	//���[���J�����̉�]�𔽉f(��[�邩�߂�̏���x�N�g��)
	viewProjection_.up = Velocity(up, worldTransform_.matWorld_);
	//�r���[�v���W�F�N�V�������X�V
	viewProjection_.UpdateMatrix();
	////�s��̍X�V
	//Matrix4 viewMatIdentity;
	//viewMatIdentity.m[0][0] = 1;
	//viewMatIdentity.m[1][1] = 1;
	//viewMatIdentity.m[2][2] = 1;
	//viewMatIdentity.m[3][3] = 1;

	//Matrix4 matEye = viewMatIdentity;
	//matEye.m[0][0] = viewProjection_.eye.x;
	//matEye.m[1][1] = viewProjection_.eye.y;
	//matEye.m[2][2] = viewProjection_.eye.z;

	//Matrix4 matTargetZ = viewMatIdentity;
	//matTargetZ.m[0][0] = cos(viewProjection_.target.z);
	//matTargetZ.m[0][1] = sin(viewProjection_.target.z);
	//matTargetZ.m[1][0] = -sin(viewProjection_.target.z);
	//matTargetZ.m[1][1] = cos(viewProjection_.target.z);

	//Matrix4 matTargetX = viewMatIdentity;
	//matTargetX.m[1][1] = cos(viewProjection_.target.x);
	//matTargetX.m[1][2] = sin(viewProjection_.target.x);
	//matTargetX.m[2][1] = -sin(viewProjection_.target.x);
	//matTargetX.m[2][2] = cos(viewProjection_.target.x);

	//Matrix4 matTargetY = viewMatIdentity;
	//matTargetY.m[0][0] = cos(viewProjection_.target.y);
	//matTargetY.m[0][2] = sin(viewProjection_.target.y);
	//matTargetY.m[2][0] = -sin(viewProjection_.target.y);
	//matTargetY.m[2][2] = cos(viewProjection_.target.y);

	//Matrix4 matUp = viewMatIdentity;
	//matUp.m[3][0] = viewProjection_.up.x;
	//matUp.m[3][1] = viewProjection_.up.y;
	//matUp.m[3][2] = viewProjection_.up.z;
	//matUp.m[3][3] = 1;

	//viewProjection_.matView = viewMatIdentity;
	//viewProjection_.matView *= matEye;

	//viewProjection_.matView *= matTargetZ;
	//viewProjection_.matView *= matTargetX;
	//viewProjection_.matView *= matTargetY;

	//viewProjection_.matView *= matUp;

	//viewProjection_.TransferMatrix();

	DebugText* debugText_ = DebugText::GetInstance();

	debugText_->SetPos(20, 100);
	debugText_->Printf("pos:(%f,%f,%f)", viewProjection_.eye.x, viewProjection_.eye.y, viewProjection_.eye.z);
}