#pragma once
#include <vector>

struct StatusInfo
{
	float mfMaxHP;
	float mfHP;
	float mfAttack;
	float mfDefence;
};

class Status
{
public:

	Status();
	~Status();

	void Update();

	//初期ステータスを設定する
	void InitializeStatus(
		float _maxHP,
		float _hp,
		float _attack,
		float _defence
	);

	void ApplyDamage(Status* _pAttacker, Status* _pDefencer);	//ダメージ計算→履行

	StatusInfo* GetStatusInfo() { return mpStatusInfo; }	//ステータス情報を返す
private:
	StatusInfo* mpStatusInfo;	//ステータス情報
};

Status::Status(){}
Status::~Status() {}

void Status::Update(){}

//ステータス情報の初期化
void Status::InitializeStatus(
	float _maxHP,
	float _hp,
	float _attack,
	float _defence)
{
	mpStatusInfo = new StatusInfo();
	mpStatusInfo->mfMaxHP = _maxHP;
	mpStatusInfo->mfHP = _hp;
	mpStatusInfo->mfAttack = _attack;
	mpStatusInfo->mfDefence = _defence;
}

//ダメージ計算→履行
void Status::ApplyDamage(Status* _pAttacker, Status* _pDefencer)
{
	StatusInfo* pAttackerStatus = _pAttacker->GetStatusInfo();
	StatusInfo* pDefenderStatus = _pDefencer->GetStatusInfo();

	float fDamage = pAttackerStatus->mfAttack - pDefenderStatus->mfDefence;	//実際のダメージを算出
	pDefenderStatus->mfHP -= fDamage;	//ダメージをHPに反映
}