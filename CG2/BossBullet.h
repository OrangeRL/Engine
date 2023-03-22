#pragma once
#include "GameObject3D.h"
class BossBullet
{
public:
	void Initialize(ViewProjection* viewProjection, XMMATRIX* matProjection);
	void Update();
	void Draw();

	//WorldTransform GetWorldTransform();
	//��������ꏊ��ݒ�
	Vector3 SetTransform(Vector3 transform);

	bool IsDead()const { return isDelete_; }
private:
	GameObject3D* gameObject = nullptr;

	//������܂ł̎���
	// 60 * ������܂ł̎���:
	static const int32_t deleteTime = 60 * 150;
	Vector3 speed;
	//�^�C�}�[
	int32_t deleteTimer_ = deleteTime;
	//�t���O
	bool isDelete_ = false;
};

