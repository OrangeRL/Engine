#include "EnemyBullet.h"

void EnemyBullet::Initialize(ViewProjection* viewProjection, XMMATRIX* matProjection, const wchar_t* textureFileName)
{
	gameObject = new GameObject3D();
	gameObject->PreLoadTexture(textureFileName);
	gameObject->SetViewProjection(viewProjection);
	gameObject->SetMatProjection(matProjection);
	gameObject->Initialize();

}

void EnemyBullet::Update(WorldTransform player)
{
	if (--deleteTimer_ <= 0) {
		isDelete_ = true;
	}


	//�v���C���[��_��
	if (bulletNum == 0){

	}
	//���ʂɃJ�[�e���`��
	else if (bulletNum == 1) {
		gameObject->worldTransform.translation.z -= 0.1f;
	}
	gameObject->Update();
}

void EnemyBullet::Draw()
{
	gameObject->Draw();
}

WorldTransform EnemyBullet::GetWorldTransform()
{
	return gameObject->worldTransform;
}

Vector3 EnemyBullet::SetTransform(Vector3 transform)
{
	this->gameObject->worldTransform.translation = transform;

	return gameObject->worldTransform.translation;
}

int EnemyBullet::SetBullet(int bulletNum)
{
	this->bulletNum = bulletNum;

	return this->bulletNum;
}
