/*
	��ų�� �Ŵ��� ������� �����մϴ�

	   ��Ŀ					�ų�				   ����Ŀ
	1 �÷�ġ��			  ������				�� â
	2 ����ġ��			������ ��ȯ				  �����̾�
	3 �г��� �ϰ�		������ �ѹ�				  �����Ķ�
*/
#include "BaseApplication.h"

#pragma once

class CSkill{
protected:
	int		mSkillNumber;	//��ų�ѹ�
	int		mDamage;		//�����
	int		mCooltime;		//��Ÿ��
	float	mDistance;		//�̵��Ÿ�
	int		mAttackframe;	//�������� ���� ������

private:
	//int timer_count;
	//Ogre::Timer *timer;
	bool mSkillActivate;		//���� �ٸ� ��ų�� ��� �����Ѱ�
	//bool mSkillNum[9];			//���� ��ų�� �ش��ϴ� �ѹ� - �Ʒ� �Լ� �������

public:
	CSkill();
	~CSkill();

	//function
	bool WalkerSkill_1(Ogre::SceneNode *TargetNode, Ogre::Real deltaTime, Ogre::Vector3 Dir);				//�÷�ġ��
	bool WalkerSkill_2(Ogre::SceneNode *TargetNode, Ogre::Real deltaTime, Ogre::Vector3 Dir);				//���ġ��
	bool WalkerSkill_3(Ogre::SceneNode *TargetNode, Ogre::Real deltaTime, Ogre::Vector3 Dir);				//�г��� �ϰ�

	bool GunnerSkill_1(Ogre::SceneNode *TargetNode, Ogre::Real deltaTime, Ogre::Vector3 Dir);				//������
	bool GunnerSkill_2(Ogre::SceneNode *TargetNode, Ogre::Real deltaTime, Ogre::Vector3 Dir);				//������ ��ȯ
	bool GunnerSkill_3(Ogre::SceneNode *TargetNode, Ogre::Real deltaTime, Ogre::Vector3 Dir);				//������ �ѹ�

	bool PsykerSkill_1(Ogre::SceneNode *TargetNode, Ogre::Real deltaTime, Ogre::Vector3 Dir);				//�� â
	bool PsykerSkill_2(Ogre::SceneNode *TargetNode, Ogre::Real deltaTime, Ogre::Vector3 Dir);				//�����̾�
	bool PsykerSkill_3(Ogre::SceneNode *TargetNode, Ogre::Real deltaTime, Ogre::Vector3 Dir);				//�����Ķ�

	bool Run(int SkillNum);		//SkillNum - ����� ��ų�� �ѹ�

	//get, set
	void		SetSkillNumber(int number)	{mSkillNumber = number;}
	int			GetSkillNumber()			{return mSkillNumber;}
	void		SetDamage(int damage)		{mDamage = damage;}
	int			GetDamage()					{return mDamage;}
	void		SetCooltime(int cooltime)	{mCooltime = cooltime;}
	int			GetCooltime()				{return mCooltime;}
	void		SetDistance(float distance)	{mDistance = distance;}
	float		GetDistance()				{return mDistance;}
	void		SetAttackframe(int frame)	{mAttackframe = frame;}
	int			GetAttackframe()			{return mAttackframe;}
};