#include "Matrix4.h"
#include "Vector3.h"
#include "WorldTransform.h"
#include "math.h"

Matrix4 Matrix4::CreateMatScale(Vector3 scale, WorldTransform worldTransform_) {
	//行列の更新
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
	return matScale;
}

Matrix4 Matrix4::CreateMatRotX(Vector3 rotation, WorldTransform worldTransform_) {
	//行列の更新
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

	return matRotX;
}

Matrix4 Matrix4::CreateMatRotY(Vector3 rotation, WorldTransform worldTransform_) {
	//行列の更新
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

	return matRotY;
}

Matrix4 Matrix4::CreateMatRotZ(Vector3 rotation, WorldTransform worldTransform_) {
	//行列の更新
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

	return matRotZ;
}

Matrix4 Matrix4::CreateMatTrans(Vector3 translation, WorldTransform worldTransform_) {
	//行列の更新
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

	return matTrans;
}

Matrix4 Matrix4::CreateMatrix(Vector3 scale, Vector3 rotation, Vector3 translation, WorldTransform worldTransform_)
{
	Matrix4 matScale = CreateMatScale(scale, worldTransform_);
	Matrix4 matRotZ = CreateMatRotZ(rotation, worldTransform_);
	Matrix4 matRotX = CreateMatRotX(rotation, worldTransform_);
	Matrix4 matRotY = CreateMatRotY(rotation, worldTransform_);
	Matrix4 matTrans = CreateMatTrans(translation, worldTransform_);

	worldTransform_.matWorld_ *= matScale;
	worldTransform_.matWorld_ *= matRotZ;
	worldTransform_.matWorld_ *= matRotX;
	worldTransform_.matWorld_ *= matRotY;
	worldTransform_.matWorld_ *= matTrans;
	worldTransform_.TransferMatrix();

	return worldTransform_.matWorld_;
}

Vector3 Matrix4::Velocity(Vector3 velocity, WorldTransform worldTransform) {
	Vector3 v;
	v.x = velocity.x * worldTransform.matWorld_.m[0][0] + velocity.y * worldTransform.matWorld_.m[1][0] + velocity.z * worldTransform.matWorld_.m[2][0];
	v.y = velocity.x * worldTransform.matWorld_.m[0][1] + velocity.y * worldTransform.matWorld_.m[1][1] + velocity.z * worldTransform.matWorld_.m[2][1];
	v.z = velocity.x * worldTransform.matWorld_.m[0][2] + velocity.y * worldTransform.matWorld_.m[1][2] + velocity.z * worldTransform.matWorld_.m[2][2];

	return v;
}
