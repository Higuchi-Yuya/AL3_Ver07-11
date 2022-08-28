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

	bool cameraMoveFlag() const { return cameraMoveFlag_; }

	void SetcameraMoveFlag(bool flag);

  private:
	// ワールド変換データ
	WorldTransform worldTransform_;

	// ビュープロジェクション
	ViewProjection viewProjection_;

	// オブジェクトの動くスピード
	Vector3 moveSpeed_ = {0.01f, 0.01f, 0.02f};
	Vector3 rotateSpeed_ = {0.05f, 0.001f, 0.05f};
	DebugText* debugText_ = nullptr;

	// カメラオブジェの移動フラグ
	bool cameraMoveFlag_ = false;
};
