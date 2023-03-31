#pragma once
#include "DirectXMath.h"
#include "Vector3.h"

class Collision
{
private:
	//�����蔻��̌v�Z�ɕK�v�ȍ\����
	struct CubeVertex
	{
		Vector3 c;	//���S
		//DirectX::XMFLOAT3 v[8];	//�e���_�̍��W
		Vector3 min;	//-x,-y,-z
		Vector3 max;	//x,y,z
	};
	//�����蔻��̌v�Z�ɕK�v�ȍ\����
	struct SphereVertex
	{
		Vector3 c;	//���S
		Vector3 r;	//���a
		Vector3 min;
		Vector3 max;
	};
public:
	//�Փ˔�����s���I�u�W�F�N�g���Z�b�g(�����̂Ƌ�)
	void SetObject(Vector3 cubePos, Vector3 cubeScale);
	//�X�V
	bool Update(Vector3 spherePos, Vector3 sphereScale);

	//�Q�b�^�[
	Vector3 GetPosition() { return cubeVertex1.c; }
	Vector3 GetMin() { return cubeVertex1.min; }
	Vector3 GetMax() { return cubeVertex1.max; }
private:
	//�����蔻��ɕK�v�ȕϐ�
	CubeVertex cubeVertex1;
	SphereVertex sphereVertex1;

	/*DirectX::XMFLOAT3 cubePos;
	DirectX::XMFLOAT3 cubeScale;*/

	bool hit;
};