#include "JsonLoader.h"
#include <fstream>
#include "fstream"
#include "cassert"
#include <json.hpp>
#define PI 3.1415

void JsonLoader::LoadFile(const std::string fileName)
{

	//連結してフルパスを得る
	const std::string fullpath = std::string("Resources/levels/") + fileName;
	//ファイルストリーム
	std::ifstream file;

	//ファイルを開く
	file.open(fullpath);
	//ファイルオープン失敗をチェック
	if (file.fail())
	{
		assert(0);
	}

	//JSON文字列から解凍したデータ
	nlohmann::json deserialized;

	//解凍
	file >> deserialized;

	//正しいレベルデータファイルかチェック
	assert(deserialized.is_object());
	assert(deserialized.contains("name"));
	assert(deserialized["name"].is_string());

	//"name"を文字列として取得
	std::string name = deserialized["name"].get<std::string>();
	//正しいレベルデータファイルかチェック
	assert(name.compare("scene") == 0);

	//レベルデータ格納用インスタンスを生成
	//levelData = new LevelData();
	//"objects"の全オブジェクトを走査
	for (nlohmann::json& object : deserialized["objects"])
	{
		assert(object.contains("type"));
		//種別を取得
		std::string type = object["type"].get<std::string>();

		//MESH
		if (type.compare("MESH") == 0)
		{
			//要素追加
			objects.emplace_back(ObjectData{});
			//今追加した要素の参照を得る
			ObjectData& objectData = objects.back();
			if (object.contains("file_name"))
			{
				//ファイル名
				objectData.file_name = object["file_name"];
			}

			if (object.contains("move_points"))
			{
				nlohmann::json& move_points = object["move_points"];
				//平行移動
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
			//トランスフォームのパラメータ読み込み
			nlohmann::json& transform = object["transform"];
			//平行移動
			objectData.translation.x = (float)transform["translation"][1];
			objectData.translation.y = (float)transform["translation"][2];
			objectData.translation.z = -(float)transform["translation"][0];
			//objectData.translation.m128_f32[3] = 1.0f;
			//回転角
			objectData.rotation.x = -(float)transform["rotation"][1];
			objectData.rotation.y = -(float)transform["rotation"][2];
			objectData.rotation.z = (float)transform["rotation"][0];
			//objectData.rotation.m128_f32[3] = 0.0f;
			//スケーリング
			objectData.scaling.x = (float)transform["scaling"][1];
			objectData.scaling.y = (float)transform["scaling"][2];
			objectData.scaling.z = (float)transform["scaling"][0];
			//objectData.scaling.m128_f32[3] = 0.0f;
			//TODO:コライダーのパラメータ読み込み

			if (object.contains("children"))
			{

			}
		}
	}
}
