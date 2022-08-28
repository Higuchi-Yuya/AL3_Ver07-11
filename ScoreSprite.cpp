#include "ScoreSprite.h"


void ScoreSprite::Initialize()
{
	// スコアのテクスチャ
	scoreTextureHandle_ = TextureManager::Load("score_font.png");

	// スプライトの初期化
	for (int i = 0; i < 10; i++)
	{
		texBase.x = 64 * i;
		scoresprite_[i].reset(Sprite::Create(scoreTextureHandle_, pos, color, anker));
		scoresprite_[i].get()->SetSize(size_);
		scoresprite_[i].get()->SetTextureRect(texBase, texsize);
	}
}

void ScoreSprite::Draw(int num, Vector2 pos)
{
	scoresprite_[num].get()->SetPosition(pos);
	scoresprite_[num].get()->Draw();
}

void ScoreSprite::SetSize(Vector2 size)
{
	size_ = size;
}
