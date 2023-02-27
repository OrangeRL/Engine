#pragma once
#include "GameObject3D.h"
#include "Input.h"
class EnemyBullet
{
public:
	void Initialize(ViewProjection* viewProjection, XMMATRIX* matProjection, const wchar_t* textureFileName);
	void Update(WorldTransform enemy);
	void Draw();
	//��������ꏊ��ݒ�
	Vector3 SetTransform(Vector3 transform);
	//�e�̎�ނ�ݒ�
	int SetBullet(int bulletNum);
private:
	GameObject3D* gameObject = nullptr;
	//�L�[����
	Input& input = Input::GetInstance();
	//�e�̎��	0=�v���C���[�_��, 1=���ʃJ�[�e����
	int bulletNum;
};

