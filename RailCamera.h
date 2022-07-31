#pragma once
#include"Model.h"
#include"WorldTransform.h"
#include "Input.h"
#include "DebugText.h"
#include "ViewProjection.h"
#include "Vector3.h"

///<summary>
///���[���J����
///</summary>
class RailCamera
{
public:
	///<summary>
	///������
	///</summary>
	void Initialize(Vector3 position, Vector3 rotation);

	///<summary>
	///�X�V
	///</summary>
	void Update();

	/// <summary>
	/// �r���[�v���W�F�N�V�������擾
	/// </summary>
	/// <returns>�r���[�v���W�F�N�V����</returns>
	const ViewProjection& GetViewProjection() { return viewProjection_; };

	//���[���h�s����擾
	const WorldTransform& GetWorldMatrix()const { return worldTransform_; };

	Vector3 Velocity(Vector3 velocity, Matrix4 matWorld_);
private:
	//���[���h�ϊ��f�[�^
	WorldTransform worldTransform_;
	//�r���[�v���W�F�N�V����
	ViewProjection viewProjection_;
	//���͂��邽��
	Input* input_ = nullptr;
	//�f�o�b�O�e�L�X�g
	DebugText* debugText_ = nullptr;
};