#include "RailCamera.h"

void RailCamera::Initialize(WorldTransform worldTransform, Vector3 radian)
{
	worldTransform_ = worldTransform;
	worldTransform_.rotation_ = radian;

	viewProjection_.Initialize();
	debugText_ = DebugText::GetInstance();
}

void RailCamera::Update()
{
	Vector3 move = { 0,0,0 };

	float kRailSpeed = 0.1f;

	//�����������ňړ��x�N�g����ύX
	if (input_->PushKey(DIK_B))
	{
		worldTransform_.translation_ = { 0,0,kRailSpeed };
	}
	else if (input_->PushKey(DIK_N))
	{
		worldTransform_.translation_ = { 0,0,-kRailSpeed };
	}

	const float rotateSpeed = 0.05f;
	if (input_->PushKey(DIK_RIGHT))
	{
		worldTransform_.rotation_.y += rotateSpeed;
	}
	else if (input_->PushKey(DIK_LEFT))
	{
		worldTransform_.rotation_.y -= rotateSpeed;
	}

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

	viewProjection_.eye = worldTransform_.translation_;
	//���[���h�O���x�N�g��
	Vector3 forward(0, 0, 1);
	//���[���J�����̉�]�𔽉f
	forward;
	//���_����O���ɓK���ȋ����i�񂾈ʒu�������_
	viewProjection_.target = viewProjection_.eye += forward;
	//���[���h����x�N�g��
	Vector3 up(0, 1, 1);
	//���[���J�����̉�]�𔽉f(��[�邩�߂�̏���x�N�g��)
	viewProjection_.up;
	//�r���[�v���W�F�N�V�������X�V
	
	debugText_->SetPos(50, 50);
	debugText_->Printf("eye:(%f,%f,%f)", viewProjection_.eye.x, viewProjection_.eye.y, viewProjection_.eye.z);
}