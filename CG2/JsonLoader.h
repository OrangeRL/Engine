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
private:	//エイリアス
	//DirectX::を省略
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMMATRIX = DirectX::XMMATRIX;

public:
	//struct LevelData
	//{
	//	//オブジェクト1個分のデータ
	//	struct ObjectData
	//	{
	//		// ファイル名
	//		std::string fileName;
	//		// 平行移動
	//		Vector3 translation;
	//		// 回転角
	//		Vector3 rotation;
	//		// スケーリング
	//		Vector3 scaling;
	//	};
	//	std::vector<ObjectData> objects;
	//};

	struct ObjectData {
		// ファイル名
		std::string file_name;
		// 平行移動
		Vector3 translation;
		// 回転角
		Vector3 rotation;
		// スケーリング
		Vector3 scaling;
		//動き
		Vector3 point_1;
		Vector3 point_2;
		Vector3 point_3;
	};

	//メンバ関数
public:
	void LoadFile(const std::string fileName);

	//ゲッター
	size_t GetObjectData() { return objects.size(); }
	std::string GetFileName(int number) { return objects[number].file_name; }
	Vector3 GetPosition(int number) { return objects[number].translation; }
	Vector3 GetRotation(int number) { return objects[number].rotation; }
	Vector3 GetScale(int number) { return objects[number].scaling; }
	Vector3 GetPoint1(int number) { return objects[number].point_1; }
	Vector3 GetPoint2(int number) { return objects[number].point_2; }
	Vector3 GetPoint3(int number) { return objects[number].point_3; }

	//メンバ変数
private:
	std::vector<ObjectData>objects;
	//LevelData* levelData = nullptr;
};