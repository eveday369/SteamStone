/*

		캐릭터를 생성하는 곳입니당

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

	//캐릭터의 상태값변화
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

	CAnimation			*anim;					//애니메이션 클래스
	CEffect				*effect;				//이펙트 클래스

	int					m_nMaxHP;
	int					m_nHP;
	int					m_nMaxMP;
	int					m_nMP;
	//char				mClassName[7];
	Ogre::SceneManager	*mSceneMgr;				//씬매니저
	Ogre::UTFString		mName;					//캐릭터 이름
	Ogre::Entity		*mChar;					//신밧드
	Ogre::SceneNode		*mTargetNode;			//타겟노드
	Ogre::SceneNode		*mSightNode;			//사이트노드
	Ogre::SceneNode		*mCameraNode;			//카메라노드
	Ogre::MovableText	*TextID;				//아이디출력
	Ogre::SceneNode		*TextIDNode;			//아이디의 노드
	Ogre::Vector3		mDirection;				//캐릭터 이동 방향
	Ogre::Vector3		mCurrentSpeed;			//캐릭터 최근 속도
	Ogre::Vector3		mCurrentPosition;		//캐릭터 최근 위치
	DWORD				mCurrentTime;			//최근에 갱신한 시간.
	DWORD				mRecvTime;
	State				mState;					//캐릭터의 상태값
	bool				mHit;					//맞고 있는가
	bool				mSkill[3];				//스킬3개
	int					mJob;					//직업
	bool				mGuard;					//가드중인가?
	int					MotionNumber;			//공격모션 넘버
	bool				mDown;					//다운되었는가
	bool				mMove;					//이동하는중인가
	bool				mGround;				//지상이냐 공중이냐
	bool				mLanding;				//착지중이냐
	int					mCurrentState;			//최근 상태값

	float				m_fSkill1Cool;
	float				m_fSkill2Cool;
	float				m_fSkill3Cool;
	float				m_fSkill1NowCool;
	float				m_fSkill2NowCool;
	float				m_fSkill3NowCool;

	DWORD				m_dwPKey;				//캐릭터 구분할 때 쓰자

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
	void						ReleaseAll();														//모든 포인터 삭제
	void						update(const Ogre::FrameEvent &evt);								//캐릭터의 상태를 관리
	void						jump(Ogre::Real deltaTime);										//점프
	void						move(Ogre::Real deltaTime);															//이동
	void						initialize();														//초기화
	void						UpdateAnimation(const Ogre::FrameEvent &evt);						//애니메이션을 재생
	bool						mapCollision(Ogre::Vector3 RayDirection, int QueryMask);			//맵 충돌체크
	void						SeeToDirection();													//캐릭터가 향하는 방향으로 보게 하는 함수

	//Get, Set
	void						SetAttack(int _MotionNumber) {MotionNumber = _MotionNumber;}
	int							GetAttack() {return MotionNumber;}
	void						SetGuard(bool guard) {mGuard = guard;}

	void						SetSkill(int Num, bool OnOff) {mSkill[Num] = OnOff;}	//임시로 애니메이션 시키려고 만들었다 나중에 확인
	bool						GetSkill(int SkillNumber) {return mSkill[SkillNumber];} //궁극기 체크용
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