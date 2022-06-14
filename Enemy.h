#pragma once

#include"Model.h"
#include"WorldTransform.h"
#include "Input.h"
#include "DebugText.h"
#include "ViewProjection.h"

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

	Matrix4 CreateMatScale(Vector3 scale);
	Matrix4 CreateMatRotX(Vector3 rotation);
	Matrix4 CreateMatRotY(Vector3 rotation);
	Matrix4 CreateMatRotZ(Vector3 rotation);
	Matrix4 CreateMatTrans(Vector3 translation);

private:
	//���[���h�ϊ��f�[�^
	WorldTransform worldTransform_;
	//���f���̃|�C���^(�؂�Ă�����)
	Model* model_ = nullptr;
	//�e�N�X�`���n���h��
	uint32_t textureHandle_ = 0u;
};