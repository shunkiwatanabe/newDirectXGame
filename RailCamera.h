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
	void Initialize(WorldTransform worldTransform, Vector3 radian);

	///<summary>
	///�X�V
	///</summary>
	void Update();
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