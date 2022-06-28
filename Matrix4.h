#pragma once

class Vector3;
struct WorldTransform;

/// <summary>
/// �s��
/// </summary>
class Matrix4 {
public:
	// �sx��
	float m[4][4];

	// �R���X�g���N�^
	Matrix4();
	// �������w�肵�Ă̐���
	Matrix4(
		float m00, float m01, float m02, float m03,
		float m10, float m11, float m12, float m13,
		float m20, float m21, float m22, float m23,
		float m30, float m31, float m32, float m33);

	// ������Z�q�I�[�o�[���[�h
	Matrix4& operator*=(const Matrix4& m2);

	Matrix4 CreateMatScale(Vector3 scale, WorldTransform worldTransform_);
	Matrix4 CreateMatRotX(Vector3 rotation, WorldTransform worldTransform_);
	Matrix4 CreateMatRotY(Vector3 rotation, WorldTransform worldTransform_);
	Matrix4 CreateMatRotZ(Vector3 rotation, WorldTransform worldTransform_);
	Matrix4 CreateMatTrans(Vector3 translation, WorldTransform worldTransform_);
	Matrix4 CreateMatrix(Vector3 scale, Vector3 rotation, Vector3 translation, WorldTransform worldTransform_);

	Vector3 Velocity(Vector3 velocity, WorldTransform worldTransform);
};
