#pragma once
#include "ViewProjection.h"
#include "WorldTransform.h"
#include "DebugText.h"
#include <cassert>

class RailCamera 
{
  public:
	void Initialize(WorldTransform worldTransform);

	void Update();

	ViewProjection GetViewProjection();

	WorldTransform* GetWorldTransform();


  private:
	// ワールド変換データ
	WorldTransform worldTransform_;

	// ビュープロジェクション
	ViewProjection viewProjection_;

	// オブジェクトの動くスピード
	Vector3 moveSpeed_ = {0.01f, 0.01f, 0.04f};
	Vector3 rotateSpeed_ = {0.05f, 0.001f, 0.05f};
	DebugText* debugText_ = nullptr;
};
