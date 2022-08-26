#pragma once
#include"Sprite.h"
#include"TextureManager.h"
#include <cassert>
#include <math.h>
#include <memory>
#include <list>
class ScoreSprite
{
public:
	// 初期化の関数
	void Initialize();

	// 描画の関数
	void Draw(int num, Vector2 pos);
private:
	// スプライト
	std::unique_ptr<Sprite> scoresprite_[10];

	// スコア用テクスチャ
	uint32_t scoreTextureHandle_ = 0;

	// 初期化用変数
	Vector4 color = { 1, 1, 1, 1 };
	Vector2 anker = { 0, 0 };
	Vector2 size = { 32,48 };
	//スプライト一枚あたりのサイズ
	const Vector2 texsize = { 64,96 };
	Vector2 texBase = { 0,0 };
	
	//スプライトの位置
	Vector2 pos = { 800, 200 };
	Vector2 pos_[5] =
	{ 
		{800, 200},
		{200,200},
		{200,200},
		{200,200},
		{200,200},
	};
};

