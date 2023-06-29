#pragma once
#include "array"
#include "DirectXMath.h"
#include "vector"
#include "memory"
#include "string"
#include "list"
#include "Vector3.h"

class JsonLoader
{
private:	//�G�C���A�X
	//DirectX::���ȗ�
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMMATRIX = DirectX::XMMATRIX;

public:
	//struct LevelData
	//{
	//	//�I�u�W�F�N�g1���̃f�[�^
	//	struct ObjectData
	//	{
	//		// �t�@�C����
	//		std::string fileName;
	//		// ���s�ړ�
	//		Vector3 translation;
	//		// ��]�p
	//		Vector3 rotation;
	//		// �X�P�[�����O
	//		Vector3 scaling;
	//	};
	//	std::vector<ObjectData> objects;
	//};

	struct ObjectData {
		// �t�@�C����
		std::string file_name;
		// ���s�ړ�
		Vector3 translation;
		// ��]�p
		Vector3 rotation;
		// �X�P�[�����O
		Vector3 scaling;
	};

	//�����o�֐�
public:
	void LoadFile(const std::string fileName);

	//�Q�b�^�[
	size_t GetObjectData() { return objects.size(); }
	std::string GetFileName(int number) { return objects[number].file_name; }
	Vector3 GetPosition(int number) { return objects[number].translation; }
	Vector3 GetRotation(int number) { return objects[number].rotation; }
	Vector3 GetScale(int number) { return objects[number].scaling; }

	//�����o�ϐ�
private:
	std::vector<ObjectData>objects;
	//LevelData* levelData = nullptr;
};