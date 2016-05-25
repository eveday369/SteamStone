/*

		ĳ���͸� �����ϴ� ���Դϴ�

*/

#pragma once

#include "BaseApplication.h"
#include "Animation.h"
#include "MovableText.h"
#include "Typedef_Packet.h"
#include "Energy.h"
#include "Effect.h"
#include "stdafx.h"

class CCharacter{
protected:
	//values

	//ĳ������ ���°���ȭ
	enum State{	
		ATTACK,
		ATTACK2,
		ATTACK3,
		DOWN,
		GUARD,
		IDLE,
		JUMP,
		RUN,
		SKILL1,
		SKILL2,
		SKILL3,
		LANDING,
		HIT,
		DEAD,
		NONE
	};

	CAnimation			*anim;					//�ִϸ��̼� Ŭ����
	CEffect				*effect;				//����Ʈ Ŭ����

	int					m_nMaxHP;
	int					m_nHP;
	int					m_nMaxMP;
	int					m_nMP;
	//char				mClassName[7];
	Ogre::SceneManager	*mSceneMgr;				//���Ŵ���
	Ogre::UTFString		mName;					//ĳ���� �̸�
	Ogre::Entity		*mChar;					//�Ź��
	Ogre::SceneNode		*mTargetNode;			//Ÿ�ٳ��
	Ogre::SceneNode		*mSightNode;			//����Ʈ���
	Ogre::SceneNode		*mCameraNode;			//ī�޶���
	Ogre::MovableText	*TextID;				//���̵����
	Ogre::SceneNode		*TextIDNode;			//���̵��� ���
	Ogre::Vector3		mDirection;				//ĳ���� �̵� ����
	Ogre::Vector3		mCurrentSpeed;			//ĳ���� �ֱ� �ӵ�
	Ogre::Vector3		mCurrentPosition;		//ĳ���� �ֱ� ��ġ
	DWORD				mCurrentTime;			//�ֱٿ� ������ �ð�.
	DWORD				mRecvTime;
	State				mState;					//ĳ������ ���°�
	bool				mHit;					//�°� �ִ°�
	bool				mSkill[3];				//��ų3��
	int					mJob;					//����
	bool				mGuard;					//�������ΰ�?
	int					MotionNumber;			//���ݸ�� �ѹ�
	bool				mDown;					//�ٿ�Ǿ��°�
	bool				mMove;					//�̵��ϴ����ΰ�
	bool				mGround;				//�����̳� �����̳�
	bool				mLanding;				//�������̳�
	int					mCurrentState;			//�ֱ� ���°�

	float				m_fSkill1Cool;
	float				m_fSkill2Cool;
	float				m_fSkill3Cool;
	float				m_fSkill1NowCool;
	float				m_fSkill2NowCool;
	float				m_fSkill3NowCool;

	DWORD				m_dwPKey;				//ĳ���� ������ �� ����

	Energy				*m_cBillboard;

	int					AnimationTimer;
	bool				bInput;
	bool				bEffect;
	Ogre::Real			mTimer;
private:
public:
	CCharacter(Ogre::String Name, Ogre::SceneManager *sceneMgr, int JobNum);
	~CCharacter();

	//get, set
	void						ResetCharacter();
	inline Ogre::SceneNode*		getTargetNode()							{return mTargetNode;}
	inline Ogre::SceneNode*		getSightNode()							{return mSightNode;}
	inline Ogre::SceneNode*		getCameraNode()							{return	mCameraNode;}
	inline State				getState()								{return mState;}
	inline Ogre::Vector3		getDirection()							{return mDirection;}
	inline void					setDirection(Ogre::Vector3 Dir)			{ mDirection = Dir; }
	inline Ogre::Vector3		getPosition()							{ return getTargetNode()->getPosition(); }
	inline void					setPosition(Ogre::Vector3 Pos)			{ getTargetNode()->setPosition(Pos); }
	inline Ogre::Vector3		getCurrentSpeed()						{ return mCurrentSpeed; }
	inline void					setCurrentSpeed(Ogre::Vector3 speed)	{ mCurrentSpeed = speed; }
	inline Ogre::Vector3		getCurrentPosition()					{ return mCurrentPosition; }
	inline void					setCurrentPosition(Ogre::Vector3 pos)	{ mCurrentPosition = pos; }
	inline DWORD				getCurrentTime()						{ return mCurrentTime; }
	inline void					setCurrentTime(DWORD time)				{ mCurrentTime = time; }
	inline DWORD				getRecvTime()							{ return mRecvTime; }
	inline void					setRecvTime(DWORD time)					{ mRecvTime = time; }
	inline Ogre::MovableObject*	getTextID()		{return TextID;}
	inline Ogre::SceneNode*		getTextIDNode()	{return TextIDNode;}
	inline void					initTextID();
	inline void					releaseTextID();


	inline Ogre::String			GetName()								{ return mName; }
	inline void					SetName(Ogre::String szName)			{ mName = szName; }
	inline DWORD				GetPKey()								{ return m_dwPKey; }
	inline void					SetPKey(DWORD pKey)						{ m_dwPKey = pKey; }
	inline void					SetHit(bool _hit)						{mHit = _hit;}
	inline void					SetJob(int Job) {mJob = Job;}
	inline int					GetJob() {return mJob;}					

	inline void					setMaxHP(int HP)		{ m_nMaxHP = HP; }
	inline int					getMaxHP()				{ return m_nMaxHP; }
	inline void					setHP(int HP)			{ m_nHP = HP; }
	inline int					getHP()					{ return m_nHP; }
	inline void					setMaxMP(int MP)		{ m_nMaxMP = MP; }
	inline int					getMaxMP()				{ return m_nMaxMP; }
	inline void					setMP(int MP)			{ m_nMP = MP; }
	inline int					getMP()					{ return m_nMP; }
	
	//function
	void						ReleaseAll();														//��� ������ ����
	void						update(const Ogre::FrameEvent &evt);								//ĳ������ ���¸� ����
	void						jump(Ogre::Real deltaTime);										//����
	void						move(Ogre::Real deltaTime);															//�̵�
	void						initialize();														//�ʱ�ȭ
	void						UpdateAnimation(const Ogre::FrameEvent &evt);						//�ִϸ��̼��� ���
	bool						mapCollision(Ogre::Vector3 RayDirection, int QueryMask);			//�� �浹üũ
	void						SeeToDirection();													//ĳ���Ͱ� ���ϴ� �������� ���� �ϴ� �Լ�

	//Get, Set
	void						SetAttack(int _MotionNumber) {MotionNumber = _MotionNumber;}
	int							GetAttack() {return MotionNumber;}
	void						SetGuard(bool guard) {mGuard = guard;}

	void						SetSkill(int Num, bool OnOff) {mSkill[Num] = OnOff;}	//�ӽ÷� �ִϸ��̼� ��Ű���� ������� ���߿� Ȯ��
	bool						GetSkill(int SkillNumber) {return mSkill[SkillNumber];} //�ñر� üũ��
	//void						ResetTimer() {mTimer = 0;}

	//Job Animation
	void						WalkerAnimation(Ogre::Real Timer);
	void						PsykerAnimation(Ogre::Real Timer);
	void						GunnerAnimation(Ogre::Real Timer);

	void						isMove(bool TrueOrFalse)	{mMove = TrueOrFalse;}
	void						isGround(bool TrueOrFalse)	{mGround = TrueOrFalse;}
	void						isLanding(bool TrueOrFalse)	{mGround = TrueOrFalse;}

	float						GetSkill1Cool()				{ return m_fSkill1Cool; }
	void						SetSkill1Cool(float f)		{ m_fSkill1Cool = f; }
	float						GetSkill2Cool()				{ return m_fSkill2Cool; }
	void						SetSkill2Cool(float f)		{ m_fSkill2Cool = f; }
	float						GetSkill3Cool()				{ return m_fSkill3Cool; }
	void						SetSkill3Cool(float f)		{ m_fSkill3Cool = f; }
	float						GetSkill1NowCool()			{ return m_fSkill1NowCool; }
	void						SetSkill1NowCool(float f)	{ m_fSkill1NowCool = f; }
	float						GetSkill2NowCool()			{ return m_fSkill2NowCool; }
	void						SetSkill2NowCool(float f)	{ m_fSkill2NowCool = f; }
	float						GetSkill3NowCool()			{ return m_fSkill3NowCool; }
	void						SetSkill3NowCool(float f)	{ m_fSkill3NowCool = f; }
};