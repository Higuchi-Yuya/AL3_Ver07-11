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
	// �������̊֐�
	void Initialize();

	// �`��̊֐�
	void Draw(int num, Vector2 pos);
private:
	// �X�v���C�g
	std::unique_ptr<Sprite> scoresprite_[10];

	// �X�R�A�p�e�N�X�`��
	uint32_t scoreTextureHandle_ = 0;

	// �������p�ϐ�
	Vector4 color = { 1, 1, 1, 1 };
	Vector2 anker = { 0, 0 };
	Vector2 size = { 32,48 };
	//�X�v���C�g�ꖇ������̃T�C�Y
	const Vector2 texsize = { 64,96 };
	Vector2 texBase = { 0,0 };
	
	//�X�v���C�g�̈ʒu
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

