#include "Matrix4.h"
#include "Vector3.h"
#include "WorldTransform.h"
#include "math.h"

Matrix4 Matrix4::MatIdentity()
{
	//行列の更新
	Matrix4 matIdentity;
	matIdentity.m[0][0] = 1;
	matIdentity.m[1][1] = 1;
	matIdentity.m[2][2] = 1;
	matIdentity.m[3][3] = 1;

	return matIdentity;
}

Matrix4 Matrix4::CreateMatScale(Vector3 scale) {
	//行列の更新

	Matrix4 matScale = MatIdentity();
	matScale.m[0][0] = scale.x;
	matScale.m[1][1] = scale.y;
	matScale.m[2][2] = scale.z;
	matScale.m[3][3] = 1;

	return matScale;
}

Matrix4 Matrix4::CreateMatRotX(Vector3 rotation) {
	//行列の更新
	Matrix4 matRotX = MatIdentity();
	matRotX.m[1][1] = cos(rotation.x);
	matRotX.m[1][2] = sin(rotation.x);
	matRotX.m[2][1] = -sin(rotation.x);
	matRotX.m[2][2] = cos(rotation.x);

	return matRotX;
}

Matrix4 Matrix4::CreateMatRotY(Vector3 rotation) {
	//行列の更新
	Matrix4 matRotY = MatIdentity();
	matRotY.m[0][0] = cos(rotation.y);
	matRotY.m[0][2] = sin(rotation.y);
	matRotY.m[2][0] = -sin(rotation.y);
	matRotY.m[2][2] = cos(rotation.y);

	return matRotY;
}

Matrix4 Matrix4::CreateMatRotZ(Vector3 rotation) {
	//行列の更新
	Matrix4 matRotZ = MatIdentity();
	matRotZ.m[0][0] = cos(rotation.z);
	matRotZ.m[0][1] = sin(rotation.z);
	matRotZ.m[1][0] = -sin(rotation.z);
	matRotZ.m[1][1] = cos(rotation.z);

	return matRotZ;
}

Matrix4 Matrix4::CreateMatTrans(Vector3 translation) {
	//行列の更新
	Matrix4 matTrans = MatIdentity();
	matTrans.m[3][0] = translation.x;
	matTrans.m[3][1] = translation.y;
	matTrans.m[3][2] = translation.z;
	matTrans.m[3][3] = 1;

	return matTrans;
}

Matrix4 Matrix4::CreateMatrix(Vector3 scale, Vector3 rotation, Vector3 translation)
{
	Matrix4 matWorld_ = MatIdentity();

	matWorld_ *= CreateMatScale(scale);
	matWorld_ *= CreateMatScale(rotation);
	matWorld_ *= CreateMatScale(rotation);
	matWorld_ *= CreateMatScale(rotation);
	matWorld_ *= CreateMatScale(translation);

	return matWorld_;
}

Vector3 Matrix4::Velocity(Vector3 velocity, WorldTransform worldTransform) {
	Vector3 v;
	v.x = velocity.x * worldTransform.matWorld_.m[0][0] + velocity.y * worldTransform.matWorld_.m[1][0] + velocity.z * worldTransform.matWorld_.m[2][0];
	v.y = velocity.x * worldTransform.matWorld_.m[0][1] + velocity.y * worldTransform.matWorld_.m[1][1] + velocity.z * worldTransform.matWorld_.m[2][1];
	v.z = velocity.x * worldTransform.matWorld_.m[0][2] + velocity.y * worldTransform.matWorld_.m[1][2] + velocity.z * worldTransform.matWorld_.m[2][2];

	return v;
}