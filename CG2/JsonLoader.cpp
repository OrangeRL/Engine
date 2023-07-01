#include "JsonLoader.h"
#include <fstream>
#include "fstream"
#include "cassert"
#include <json.hpp>
#define PI 3.1415

void JsonLoader::LoadFile(const std::string fileName)
{

	//�A�����ăt���p�X�𓾂�
	const std::string fullpath = std::string("Resources/levels/") + fileName;
	//�t�@�C���X�g���[��
	std::ifstream file;

	//�t�@�C�����J��
	file.open(fullpath);
	//�t�@�C���I�[�v�����s���`�F�b�N
	if (file.fail())
	{
		assert(0);
	}

	//JSON�����񂩂�𓀂����f�[�^
	nlohmann::json deserialized;

	//��
	file >> deserialized;

	//���������x���f�[�^�t�@�C�����`�F�b�N
	assert(deserialized.is_object());
	assert(deserialized.contains("name"));
	assert(deserialized["name"].is_string());

	//"name"�𕶎���Ƃ��Ď擾
	std::string name = deserialized["name"].get<std::string>();
	//���������x���f�[�^�t�@�C�����`�F�b�N
	assert(name.compare("scene") == 0);

	//���x���f�[�^�i�[�p�C���X�^���X�𐶐�
	//levelData = new LevelData();
	//"objects"�̑S�I�u�W�F�N�g�𑖍�
	for (nlohmann::json& object : deserialized["objects"])
	{
		assert(object.contains("type"));
		//��ʂ��擾
		std::string type = object["type"].get<std::string>();

		//MESH
		if (type.compare("MESH") == 0)
		{
			//�v�f�ǉ�
			objects.emplace_back(ObjectData{});
			//���ǉ������v�f�̎Q�Ƃ𓾂�
			ObjectData& objectData = objects.back();
			if (object.contains("file_name"))
			{
				//�t�@�C����
				objectData.file_name = object["file_name"];
			}

			if (object.contains("move_points"))
			{
				nlohmann::json& move_points = object["move_points"];
				//���s�ړ�
				objectData.point_1.x = (float)move_points["point_1"][1];
				objectData.point_1.y = (float)move_points["point_1"][2];
				objectData.point_1.z = -(float)move_points["point_1"][0];

				objectData.point_2.x = (float)move_points["point_2"][1];
				objectData.point_2.y = (float)move_points["point_2"][2];
				objectData.point_2.z = -(float)move_points["point_2"][0];

				objectData.point_3.x = (float)move_points["point_3"][1];
				objectData.point_3.y = (float)move_points["point_3"][2];
				objectData.point_3.z = -(float)move_points["point_3"][0];
			}
			//�g�����X�t�H�[���̃p�����[�^�ǂݍ���
			nlohmann::json& transform = object["transform"];
			//���s�ړ�
			objectData.translation.x = (float)transform["translation"][1];
			objectData.translation.y = (float)transform["translation"][2];
			objectData.translation.z = -(float)transform["translation"][0];
			//objectData.translation.m128_f32[3] = 1.0f;
			//��]�p
			objectData.rotation.x = -(float)transform["rotation"][1];
			objectData.rotation.y = -(float)transform["rotation"][2];
			objectData.rotation.z = (float)transform["rotation"][0];
			//objectData.rotation.m128_f32[3] = 0.0f;
			//�X�P�[�����O
			objectData.scaling.x = (float)transform["scaling"][1];
			objectData.scaling.y = (float)transform["scaling"][2];
			objectData.scaling.z = (float)transform["scaling"][0];
			//objectData.scaling.m128_f32[3] = 0.0f;
			//TODO:�R���C�_�[�̃p�����[�^�ǂݍ���

			if (object.contains("children"))
			{

			}
		}
	}
}
