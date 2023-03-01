#pragma once
#include "GameObject3D.h"
#include "Input.h"
class EnemyBullet
{
public:
	void Initialize(ViewProjection* viewProjection, XMMATRIX* matProjection, const wchar_t* textureFileName);
	void Update(Vector3 player, Vector3 enemy);
	void Draw();

	WorldTransform GetWorldTransform();
	//��������ꏊ��ݒ�
	Vector3 SetTransform(Vector3 transform);
	//�e�̎�ނ�ݒ�
	int SetBullet(int bulletNum);

	bool IsDead()const { return isDelete_; }
private:
	GameObject3D* gameObject = nullptr;
	//�L�[����
	Input& input = Input::GetInstance();
	//�e�̎��	0=�v���C���[�_��, 1=���ʃJ�[�e����
	int bulletNum;

	//������܂ł̎���
	// 60 * ������܂ł̎���:
	static const int32_t deleteTime = 60 * 25;
	//�^�C�}�[
	int32_t deleteTimer_ = deleteTime;
	//�t���O
	bool isDelete_ = false;
};

