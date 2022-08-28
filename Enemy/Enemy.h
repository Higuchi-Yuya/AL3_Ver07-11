#pragma once
#include "Model.h"
#include "WorldTransform.h"
#include <cassert>

#include "Affine_trans.h"
#include "EnemyBullet.h"

enum class Phase {
	Approach, //�ڋ߂���
	Laeve,    //���E����
	Provocation, //���U��Ȃ���ڋ�
};

enum class Pattern { 
	Straight , //�����܂������ɐڋ߂���
	ProvocationWay5, //���U�铮���Ő���Ȃ���߂Â�(Way5)
	ProvocationWay3,//���U�铮���Ő���Ȃ���߂Â�(Way3)
	ProvocationWay5MoveX, //���U�铮���Ő���Ȃ���߂Â�(Way5)
	ProvocationWay3MoveX,//���U�铮���Ő���Ȃ���߂Â�(Way3)
};

class Player;

class GameScene;

class Enemy
{
  public:
	//�G�Ŏg���֗��֐�
	float Rad(float range);
	Vector3 Leap(const Vector3& start, const Vector3& end, float t);

	//�G�̊�{�I�Ȋ֐�
	void Initialize(Model* model, Vector3& position);
	void Update();
	void Draw(const ViewProjection& viewProjection);

	

	//�ڋ߃t�F�[�Y�֘A�̊֐�
	void Approach_Initialize();
	void Approach_Update();

	//���E�t�F�[�Y�֘A�̊֐�
	void Laeve_Update();

	//��U��ڋ߃t�F�[�Y�֘A�̊֐�
	void ProvocationWay5_Update();
	void ProvocationWay3_Update();
	void ProvocationWay5MoveX_Update();
	void ProvocationWay3MoveX_Update();

	// �G�L�����Ɏ��L�����̃A�h���X��n��
	void SetPlayer(Player* player) { player_ = player; }

	void SetGameScene(GameScene* gameScene) { gameScene_ = gameScene; }

	// �G�L�����̍s���p�^�[�����Z�b�g
	void SetEnemyPattern(Pattern pattern) { pattern_ = pattern; }



	//�e�̔��ˊ֐�
	void Fire();
	void Way5Fire();
	void Way3Fire();
	void HomingFire();

	// ���[���h���W���擾����
	Vector3 GetWorldPosition();

	// �Փ˂����o������Ăяo�����R�[���o�b�N�֐�
	void OnCollision();

	// �e���X�g���擾
	//const std::list<std::unique_ptr<EnemyBullet>>& GetBullets() { return bullets_; }

	bool IsDead() const { return isDead_; }

  public:
	//���ˊԊu
	static const int kFireInterval = 60;
  private:
	//���[���h�ϊ��f�[�^
	WorldTransform worldTransform_;

	//���f��
	Model* model_ = nullptr;

	float enemy_speed_x = 0.1f;
	float enemy_speed_y = 0.1f;
	float enemy_speed_z = 0.1f;
	
	// ��]���x
	Vector3 enemy_rotation_speed = { 0.005,0.005,0.002 };

	//���˃^�C�}�[
	int32_t fireTimer_ = 300;

	//�A�t�B���p
	Affine_trans* trans = nullptr;

	//�f�X�t���O
	bool isDead_ = false;

	//���ړ��t���O
	bool moveXFlag = true;

	//��]�t���O
	bool rotate_flag = true;

	//�e�N�X�`���n���h��
	uint32_t textureHandle_ = 0u;

	//�t�F�[�Y
	Phase phase_ = Phase::Approach;

	// �s���p�^�[��
	Pattern pattern_;

	// �ړ������p�ϐ�
	Vector3 enemy_initpos_ = { 0,0,0 };
	Vector3 enemy_limit_ = { 15,20,0 };

	// ���L����
	Player* player_ = nullptr;

	// �Q�[���V�[��
	GameScene* gameScene_ = nullptr;
};
