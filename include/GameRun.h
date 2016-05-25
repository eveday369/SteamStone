#pragma once

#include "BaseApplication.h"
#include "Energy.h"
#include "Skill.h"
#include "stdafx.h"
#include <hash_map>
#include <vector>

class CCharacter;

class CObject{
protected:

private:
	Ogre::SceneManager	*mSceneMgr;				//씬매니저
	Ogre::Entity		*mChar;					//
	Ogre::String		mEntityName; 
	Ogre::SceneNode		*mTargetNode;			//타겟노드
	Ogre::Vector3		mCurrentSpeed;			//최근 속도
	Ogre::Vector3		mCurrentPosition;		//최근 위치
	DWORD				mCurrentTime;			//최근에 갱신한 시간.
	OBJECT_TYPE			ObjectType;				//오브젝트의 타입
public:
	CObject(Ogre::SceneManager *sceneMgr, Ogre::Vector3 pos, DWORD dwTime, OBJECT_TYPE _ObjectType);
	~CObject();

	void					Initialize(DWORD _ObjectNum, float direction = 1);

	void					update(const Ogre::FrameEvent &evt);

	inline Ogre::Vector3	GetPosition()	{return mTargetNode->getPosition();}
	inline void				SetPosition(Ogre::Vector3 Position) {mTargetNode->setPosition(Position);}
	inline Ogre::Vector3	GetCurrentSpeed()						{ return mCurrentSpeed; }
	inline void				SetCurrentSpeed(Ogre::Vector3 speed)	{ mCurrentSpeed = speed; }
	inline Ogre::Vector3	GetCurrentPosition()					{ return mCurrentPosition; }
	inline void				SetCurrentPosition(Ogre::Vector3 pos)	{ mCurrentPosition = pos; }
	inline DWORD			GetCurrentTime()						{ return mCurrentTime; }
	inline void				SetCurrentTime(DWORD time)				{ mCurrentTime = time; }
};

/*===========================================================
                     GameRun Class
===========================================================*/
class GameRunWindow;
class GameResultWindow;
class CGameRun : public CGameState
{

private:
	typedef struct
	{
		std::string name;
		DWORD pKey;
		int jobNum;
		int maxHP;
		int maxMP;
		float skill1Cool;
		float skill2Cool;
		float skill3Cool;
		Ogre::Vector3 dir;
		Ogre::Vector3 pos;	
	}stGAME_PLAYER_INSERT;

	typedef struct
	{
		GS_TYPE Type;
		bool isGround;
		DWORD dwTime;
		DWORD dwRecvTime;
		DWORD pKey;
		Ogre::Vector3 dir;
		Ogre::Vector3 pos;
		int HP;
		int MP;
		float Skill1NowCool;
		float Skill2NowCool;
		float Skill3NowCool;
	}stGAME_PLAYER_DATA;

	typedef struct
	{
		OBJECT_TYPE objType;
		DWORD dwPKey;
		DWORD dwTime;
		float dx;
		float px;
		float py;
	}stGAME_OBJECT;

	typedef struct
	{
		ROOM_TYPE roomType;
		float fGameTime;
		DWORD dwWinnerPKey;
	}stGAME_ROOM_DATA;

	//잡 넘버
	enum Job{
		JOB_WALKER,
		JOB_GUNNER,
		JOB_PSYKER
	};


	typedef std::hash_map<DWORD, CCharacter*> CHARACTER_MAP;
	typedef std::hash_map<DWORD, CCharacter*>::iterator CHARACTER_ITER;

	typedef std::hash_map<DWORD, CObject*>	OBJECT_MAP;
	typedef std::hash_map<DWORD, CObject*>::iterator OBJECT_ITER;

	typedef std::queue<stGAME_ROOM_DATA> GAME_QUEUE_ROOM_DATA;
	typedef std::queue<stGAME_PLAYER_INSERT> GAME_QUEUE_PLAYER_INSERT;
	typedef std::queue<stGAME_PLAYER_DATA> GAME_QUEUE_PLAYER_DATA;
	typedef std::queue<stGAME_OBJECT> GAME_QUEUE_OBJECT;

	GAME_QUEUE_ROOM_DATA		m_GameQueue_Room_Data;
	GAME_QUEUE_PLAYER_INSERT	m_GameQueue_Player_Insert;
	GAME_QUEUE_PLAYER_DATA		m_GameQueue_Player_Data;
	GAME_QUEUE_OBJECT			m_GameQueue_Object;

	bool	mSkillActive;		//스킬이 활성화 되었는지

public :
	virtual bool	CheckData();
	void			PushData						//게임 방 데이터
		(ROOM_TYPE roomType, float fGameTime, DWORD dwWinnerPKey);
	void			PushData						//데이터 처음 입력할때. 즉 캐릭터 생성
		(std::string szName, DWORD pKey, int jobNum, float dX, float dY, float pX, float pY, int maxHP, int maxMP, float Skill1Cool, float Skill2Cool, float Skill3Cool);		
	void			PushData						//끝날때까지 씀. 캐릭터 데이터.
		(GS_TYPE attackType, DWORD pKey, DWORD dwTime, bool isGround, float dX, float dY, float pX, float pY, int HP, int MP, float Skill1NowCool, float Skill2NowCool, float Skill3NowCool);
	void			PushData						//오브젝트 생성 및 파괴 할 때 쓰임.
		(OBJECT_TYPE objType, DWORD pKey, DWORD dwTime, float dx, float px, float py);

//	bool			Menu_Button( const CEGUI::EventArgs	&arg);

protected:
	DWORD					   m_Winner;
private:
	//values
	CMap						*map;			//맵 클래스
	CCamera						*camera;		//카메라 클래스
	CSkill						*mSkillMgr;		//스킬 매니저
	GameRunWindow				*runwindow;
	GameResultWindow			*resultwindow;

	Ogre::SceneManager			*mSceneMgr;		//씬매니저
	Ogre::Light					*light;
	Ogre::Viewport				*vp;					//카메라 뷰포트


	CHARACTER_MAP				m_CharacterMap;
	bool						m_bIsMoveLeft;
	bool						m_bIsMoveRight;
	bool						m_bIsReceiveData;

	int							mActiveSkill;	//발동할 스킬이 어떤것인지
	
	OBJECT_MAP					m_ObjectMap;

public:
	CGameRun(Game* pParent);
	~CGameRun();


	void				setWinner(DWORD pKey)    { m_Winner = pKey;}
	void				Run(const Ogre::FrameEvent &evt);									//실제 게임이 구동되는 부분
	bool				Result();															//게임 결과 화면
	bool				initialize(Ogre::SceneManager *sceneMgr, Ogre::Camera *cam);		//초기화
	void				AddCharacter(Ogre::String szEntityName, DWORD pKey, int nJobNum,	//캐릭터 추가
						Ogre::Vector3 dir, Ogre::Vector3 pos,
						int nMaxHP, int nMaxMP, float Skill1Cool, float Skill2Cool, float Skill3Cool);
	bool				DeleteCharacter(DWORD pKey);										//캐릭터의 삭제
	CCharacter*			GetCharacter(DWORD pKey);

	void				keyPressed(const OIS::KeyEvent &_arg);								//키가 눌리는 부분
	void				keyReleased(const OIS::KeyEvent &_arg);								//키가 떼이는 부분

	GameRunWindow		*GetRunWindow()			{return runwindow;}
	void				SetRunWindow(GameRunWindow *window)		{runwindow = window;}
	void				SetResultWindow(GameResultWindow *window)		{resultwindow = window;}

	//bool				GetIsGameRun()			{ return m_bIsGameRun; }
	//void				SetIsGameRun(bool b)	{ m_bIsGameRun = b; }

	//void				SkillRun(Ogre::SceneNode *TargetNode);								//스킬 구동

	void				AddObject(OBJECT_TYPE objType, DWORD pKey, DWORD dwTime, float dx, float px, float py);
	void				DestroyObject(DWORD pKey);
};