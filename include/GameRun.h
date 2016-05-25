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
	Ogre::SceneManager	*mSceneMgr;				//���Ŵ���
	Ogre::Entity		*mChar;					//
	Ogre::String		mEntityName; 
	Ogre::SceneNode		*mTargetNode;			//Ÿ�ٳ��
	Ogre::Vector3		mCurrentSpeed;			//�ֱ� �ӵ�
	Ogre::Vector3		mCurrentPosition;		//�ֱ� ��ġ
	DWORD				mCurrentTime;			//�ֱٿ� ������ �ð�.
	OBJECT_TYPE			ObjectType;				//������Ʈ�� Ÿ��
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

	//�� �ѹ�
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

	bool	mSkillActive;		//��ų�� Ȱ��ȭ �Ǿ�����

public :
	virtual bool	CheckData();
	void			PushData						//���� �� ������
		(ROOM_TYPE roomType, float fGameTime, DWORD dwWinnerPKey);
	void			PushData						//������ ó�� �Է��Ҷ�. �� ĳ���� ����
		(std::string szName, DWORD pKey, int jobNum, float dX, float dY, float pX, float pY, int maxHP, int maxMP, float Skill1Cool, float Skill2Cool, float Skill3Cool);		
	void			PushData						//���������� ��. ĳ���� ������.
		(GS_TYPE attackType, DWORD pKey, DWORD dwTime, bool isGround, float dX, float dY, float pX, float pY, int HP, int MP, float Skill1NowCool, float Skill2NowCool, float Skill3NowCool);
	void			PushData						//������Ʈ ���� �� �ı� �� �� ����.
		(OBJECT_TYPE objType, DWORD pKey, DWORD dwTime, float dx, float px, float py);

//	bool			Menu_Button( const CEGUI::EventArgs	&arg);

protected:
	DWORD					   m_Winner;
private:
	//values
	CMap						*map;			//�� Ŭ����
	CCamera						*camera;		//ī�޶� Ŭ����
	CSkill						*mSkillMgr;		//��ų �Ŵ���
	GameRunWindow				*runwindow;
	GameResultWindow			*resultwindow;

	Ogre::SceneManager			*mSceneMgr;		//���Ŵ���
	Ogre::Light					*light;
	Ogre::Viewport				*vp;					//ī�޶� ����Ʈ


	CHARACTER_MAP				m_CharacterMap;
	bool						m_bIsMoveLeft;
	bool						m_bIsMoveRight;
	bool						m_bIsReceiveData;

	int							mActiveSkill;	//�ߵ��� ��ų�� �������
	
	OBJECT_MAP					m_ObjectMap;

public:
	CGameRun(Game* pParent);
	~CGameRun();


	void				setWinner(DWORD pKey)    { m_Winner = pKey;}
	void				Run(const Ogre::FrameEvent &evt);									//���� ������ �����Ǵ� �κ�
	bool				Result();															//���� ��� ȭ��
	bool				initialize(Ogre::SceneManager *sceneMgr, Ogre::Camera *cam);		//�ʱ�ȭ
	void				AddCharacter(Ogre::String szEntityName, DWORD pKey, int nJobNum,	//ĳ���� �߰�
						Ogre::Vector3 dir, Ogre::Vector3 pos,
						int nMaxHP, int nMaxMP, float Skill1Cool, float Skill2Cool, float Skill3Cool);
	bool				DeleteCharacter(DWORD pKey);										//ĳ������ ����
	CCharacter*			GetCharacter(DWORD pKey);

	void				keyPressed(const OIS::KeyEvent &_arg);								//Ű�� ������ �κ�
	void				keyReleased(const OIS::KeyEvent &_arg);								//Ű�� ���̴� �κ�

	GameRunWindow		*GetRunWindow()			{return runwindow;}
	void				SetRunWindow(GameRunWindow *window)		{runwindow = window;}
	void				SetResultWindow(GameResultWindow *window)		{resultwindow = window;}

	//bool				GetIsGameRun()			{ return m_bIsGameRun; }
	//void				SetIsGameRun(bool b)	{ m_bIsGameRun = b; }

	//void				SkillRun(Ogre::SceneNode *TargetNode);								//��ų ����

	void				AddObject(OBJECT_TYPE objType, DWORD pKey, DWORD dwTime, float dx, float px, float py);
	void				DestroyObject(DWORD pKey);
};