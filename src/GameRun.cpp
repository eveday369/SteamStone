#include "stdafx.h"
#include "GameRun.h"
#include "..\include\Game.h"
#include "..\server\CServerMain.h"
#include "..\server\GameServer.h"


//-------------------------------------------------------------------------------------
CGameRun::CGameRun(Game* pParent) : CGameState(pParent)
{
	m_bIsMoveLeft			= false;
	m_bIsMoveRight			= false;
	map						= NULL;
	m_bIsReceiveData		= false;
	m_CharacterMap.clear();
	m_ObjectMap.clear();
}

//-------------------------------------------------------------------------------------
CGameRun::~CGameRun()
{
	//가장 마지막에 생성된 것부터 삭제한다
	//delete SoundMan::getSingletonPtr();
	//방 삭제시 라디얼블러를 해제
	Ogre::CompositorManager::getSingleton().setCompositorEnabled(vp, "Radial Blur", false);
	
	{
		OBJECT_ITER _iter;
		for(_iter = m_ObjectMap.begin(); _iter != m_ObjectMap.end(); ++_iter)
		{
			delete _iter->second;
		}
		m_ObjectMap.clear();
	}
	{
		CHARACTER_ITER _iter;
		for(_iter = m_CharacterMap.begin(); _iter != m_CharacterMap.end(); ++_iter)
		{
			delete _iter->second;
		}
		m_CharacterMap.clear();
	}
	if(map != NULL)
	{
		//std::cout<<"뻗니"<<std::endl;
		delete map;
		map = NULL;
	}
	delete camera;
	mSceneMgr->destroyLight(light);
	mSceneMgr->clearScene();
}

//----------------q---------------------------------------------------------------------
bool CGameRun::initialize(Ogre::SceneManager *sceneMgr, Ogre::Camera *cam)
{
	runwindow->ShowLoadingImage(true);
	mSceneMgr = sceneMgr;
	camera = new CCamera(sceneMgr, cam);
	camera->SetSightRevision(0.0f, 10.0f, -20.0f);
	if(map == NULL)
	{
		map = new CMap();
		map->Initialize(mSceneMgr);
	}
	mSkillMgr = new CSkill;

	//그림자
	mSceneMgr->setShadowTechnique(Ogre::SHADOWTYPE_STENCIL_MODULATIVE);
	mSceneMgr->setShadowColour(Ogre::ColourValue(0.5, 0.5, 0.5));	//그림자 색깔 조정
	mSceneMgr->setShadowTextureSize(512);
	mSceneMgr->setShadowTextureCount(100);

	//ambient light 세팅
    mSceneMgr->setAmbientLight(Ogre::ColourValue(1, 1, 1));
    //light 만들기
	light = mSceneMgr->createLight("GameRunMainLight");
	light->setType(Ogre::Light::LT_DIRECTIONAL);
	light->setPosition(0,200,200);
	
	Ogre::Vector3 dir;
	dir = -light->getPosition();
	dir.normalise();
	light->setDirection(dir);	//현재 빛의 방향과 반대의 방향을 가진다
	
	light->setSpecularColour(Ogre::ColourValue::White);
	light->setDiffuseColour(Ogre::ColourValue(0.9,0.9,0.9));
    light->setSpecularColour(Ogre::ColourValue(0.9,0.9,0.9));

	
	CGameServer* p = (CGameServer*)GetGame()->GetServerMain()->GetNowServer();
	p->CMD_Ready();
	printf("GameRun init\n");

	//컴포지터 등록
	vp = cam->getViewport();
	CompositorInstance* instance = Ogre::CompositorManager::getSingleton().getCompositorChain(vp)->getCompositor("Radial Blur");
	Ogre::CompositorManager::getSingleton().addCompositor(vp, "Radial Blur", 0);
	Ogre::CompositorManager::getSingleton().setCompositorEnabled(vp, "Radial Blur", false);

	return true;
}

void CGameRun::AddCharacter(Ogre::String szEntityName, DWORD pKey, int nJobNum, Ogre::Vector3 dir, Ogre::Vector3 pos, int nMaxHP, int nMaxMP, float Skill1Cool, float Skill2Cool, float Skill3Cool)
{
	CCharacter *pCharacter = new CCharacter(szEntityName, mSceneMgr, nJobNum);

	pCharacter->SetJob(nJobNum);
	pCharacter->initialize();
	pCharacter->SetPKey(pKey);
	pCharacter->setDirection(dir);
	pCharacter->setPosition(pos);
	pCharacter->setCurrentPosition(pos);
	pCharacter->setMaxHP(nMaxHP);
	pCharacter->setHP(nMaxHP);
	pCharacter->setMaxMP(nMaxMP);
	pCharacter->setMP(nMaxMP);
	pCharacter->SetSkill1Cool(Skill1Cool);
	pCharacter->SetSkill2Cool(Skill2Cool);
	pCharacter->SetSkill3Cool(Skill3Cool);
	
	if(pCharacter->GetPKey() == GetPKey())
	{
		camera->Init(pCharacter->getTargetNode());

		runwindow->Init(pCharacter->GetJob());
	}

	m_CharacterMap.insert(std::hash_map<DWORD, CCharacter*>::value_type(pKey, pCharacter));

	//printf("[생성]%d %s\n", pKey, szEntityName.c_str());
}

bool CGameRun::DeleteCharacter(DWORD pKey)
{
	CHARACTER_ITER _iter = m_CharacterMap.find(pKey);
	if(_iter == m_CharacterMap.end())
		return false;
	delete _iter->second;
	m_CharacterMap.erase(_iter);

	return true;
}

/*-------------------------------------------------------------------------------------
						Run : 실제 게임이 구동되는 부분
-------------------------------------------------------------------------------------*/
void CGameRun::Run(const Ogre::FrameEvent &evt)
{

	CCharacter* pCharacter = GetCharacter(GetPKey());
	if(pCharacter != NULL)
	{
		runwindow->CheckStatus(pCharacter->getHP(), pCharacter->getMaxHP(), pCharacter->getMP(), pCharacter->getMaxMP());
		runwindow->CheckSkill(GS_TYPE::GS_SKILL1, pCharacter->GetSkill1Cool(), pCharacter->GetSkill1NowCool());
		runwindow->CheckSkill(GS_TYPE::GS_SKILL2, pCharacter->GetSkill2Cool(), pCharacter->GetSkill2NowCool());
		runwindow->CheckSkill(GS_TYPE::GS_SKILL3, pCharacter->GetSkill3Cool(), pCharacter->GetSkill3NowCool());
		//자기 자신이 궁극기를 사용할 때
		if(pCharacter->GetSkill(2) == true)
			Ogre::CompositorManager::getSingleton().setCompositorEnabled(camera->GetViewport(), "Radial Blur", true);
		else
			Ogre::CompositorManager::getSingleton().setCompositorEnabled(camera->GetViewport(), "Radial Blur", false);

	}

	camera->moveCamera();
	//camera->moveCamera(evt.timeSinceLastFrame);

	CHARACTER_ITER _CharIter;
	for(_CharIter = m_CharacterMap.begin(); _CharIter != m_CharacterMap.end(); ++_CharIter)
	{
		_CharIter->second->update(evt);
		_CharIter->second->UpdateAnimation(evt);
	}

	OBJECT_ITER _objIter;
	for(_objIter = m_ObjectMap.begin(); _objIter != m_ObjectMap.end(); ++_objIter)
	{
		_objIter->second->update(evt);
	}
}

/*-------------------------------------------------------------------------------------
								서버를 사용하는 경우
-------------------------------------------------------------------------------------*/
void CGameRun::keyPressed(const OIS::KeyEvent &_arg)
{
	CGameServer* p = (CGameServer*)GetGame()->GetServerMain()->GetNowServer();

	CHARACTER_ITER _iter = m_CharacterMap.find(GetPKey());
	if(_iter == m_CharacterMap.end())
		return;

	if(_arg.key == OIS::KC_LEFT)		//공격중이 아닐때
	{
		m_bIsMoveLeft = true;
		p->CMD_Character(GS_TYPE::GS_MOVE_LEFT);
	}
	else if(_arg.key == OIS::KC_RIGHT)  //공격중이 아닐때
	{
		m_bIsMoveRight = true;
		p->CMD_Character(GS_TYPE::GS_MOVE_RIGHT);
	}
	if(_arg.key == OIS::KC_DOWN)
	{
		p->CMD_Character(GS_TYPE::GS_MOVE_DOWN);
	}
	//키를 누를때 && 상태가 IDLE, 또는 RUN일때 점프
	//현재 착지판정이 없으므로 상태를 IDLE로 바꿔주는 루틴이 필요하다
	if(_arg.key == OIS::KC_X) 
	{
		p->CMD_Character(GS_TYPE::GS_MOVE_UP);
	}
	//임시 공격
	if(_arg.key == OIS::KC_Z)
	{
		if(_iter->second->getState() != 2)
			p->CMD_Character(GS_TYPE::GS_ATTACK1);
	}
	//가드
	/*if(_arg.key == OIS::KC_C)
	{
		_iter->second->SetGuard(true);
	}*/
	//Skill
	if(_arg.key == OIS::KC_A)
	{
		if(_iter->second->GetSkill(1) == false && _iter->second->GetSkill(2) == false)
		{
			p->CMD_Character(GS_TYPE::GS_SKILL1);
			
			
		}
	}
	if(_arg.key == OIS::KC_S)
	{
		if(_iter->second->GetSkill(0) == false && _iter->second->GetSkill(2) == false)
			p->CMD_Character(GS_TYPE::GS_SKILL2);
	}
	if(_arg.key == OIS::KC_D)
	{
		if(_iter->second->GetSkill(0) == false && _iter->second->GetSkill(1) == false)
		{
			p->CMD_Character(GS_TYPE::GS_SKILL3);
		}
	}
	//임시 사망 플래그
	if(_arg.key == OIS::KC_Q)
	{
		_iter->second->setHP(0);
	}
	if(_arg.key == OIS::KC_W)
	{
		_iter->second->SetHit(true);
	}
	if(_arg.key == OIS::KC_ESCAPE)
	{
		if(runwindow->isMenu())
		{
			runwindow->ShowMenu(false);
		}
		else
		{
			runwindow->ShowMenu(true);
		}
	}
}

void CGameRun::keyReleased(const OIS::KeyEvent &_arg)
{
	CGameServer* p = (CGameServer*)GetGame()->GetServerMain()->GetNowServer();

	//임시 자기 캐릭터 찾는 용 아마 다른 클라에서 보면 순간이동할듯
	CHARACTER_ITER _iter = m_CharacterMap.find(GetPKey());
	if(_iter == m_CharacterMap.end())
		return;

	if(_arg.key == OIS::KC_LEFT)
	{
		if(m_bIsMoveLeft && m_bIsMoveRight)
		{
			m_bIsMoveLeft = false;
			return;
		}
		else
		{
			p->CMD_Character(GS_TYPE::GS_MOVE_RELEAESE);
			m_bIsMoveLeft = false;
		}
		
	}
	else if(_arg.key == OIS::KC_RIGHT) 
	{
		if(m_bIsMoveLeft && m_bIsMoveRight)
		{
			m_bIsMoveRight = false;
			return;
		}
		else
		{
			p->CMD_Character(GS_TYPE::GS_MOVE_RELEAESE);
			m_bIsMoveRight = false;
		}
		
	}
	//Skill
	if(_arg.key == OIS::KC_A)
	{
		//if(p->)
	}
	if(_arg.key == OIS::KC_S)
	{
	}
	if(_arg.key == OIS::KC_D)
	{
	}
	//가드
	if(_arg.key == OIS::KC_C)
	{
		_iter->second->SetGuard(false);
	}
}

CCharacter* CGameRun::GetCharacter(DWORD pKey)
{
	CHARACTER_ITER _iter = m_CharacterMap.find(pKey);
	if(_iter != m_CharacterMap.end())
		return _iter->second;

	return NULL;
}

void CGameRun::PushData(ROOM_TYPE roomType, float fGameTime, DWORD dwWinnerPKey)
{
	stGAME_ROOM_DATA stGame;
	stGame.roomType = roomType;
	stGame.fGameTime = fGameTime;
	stGame.dwWinnerPKey = dwWinnerPKey;

	m_GameQueue_Room_Data.push(stGame);
}

void CGameRun::PushData(std::string szName, DWORD pKey, int jobNum, float dX, float dY, float pX, float pY, int maxHP, int maxMP, float Skill1Cool, float Skill2Cool, float Skill3Cool)
{
	stGAME_PLAYER_INSERT stGame;
	stGame.name = szName;
	stGame.pKey = pKey;
	stGame.jobNum = jobNum;
	stGame.dir.x = dX;
	stGame.dir.y = dY;
	stGame.dir.z = 0.0f;
	stGame.pos.x = pX;
	stGame.pos.y = pY;
	stGame.pos.z = 0.0f;
	stGame.maxHP = maxHP;
	stGame.maxMP = maxMP;
	stGame.skill1Cool = Skill1Cool;
	stGame.skill2Cool = Skill2Cool;
	stGame.skill3Cool = Skill3Cool;
	
	m_GameQueue_Player_Insert.push(stGame);
}

void CGameRun::PushData(GS_TYPE attackType, DWORD pKey, DWORD dwTime,  bool isGround, float dX, float dY, float pX, float pY, int HP, int MP,  float Skill1NowCool, float Skill2NowCool, float Skill3NowCool)
{
	stGAME_PLAYER_DATA stGame;
	stGame.Type = attackType;
	stGame.pKey = pKey;
	stGame.dwTime = dwTime;
	stGame.isGround = isGround;
	stGame.dir.x = dX;
	stGame.dir.y = dY;
	stGame.dir.z = 0.0f;
	stGame.pos.x = pX;
	stGame.pos.y = pY;
	stGame.pos.z = 0.0f;
	stGame.HP = HP;
	stGame.MP = MP;
	stGame.Skill1NowCool = Skill1NowCool;
	stGame.Skill2NowCool = Skill2NowCool;
	stGame.Skill3NowCool = Skill3NowCool;

	m_GameQueue_Player_Data.push(stGame);
}

void CGameRun::PushData(OBJECT_TYPE objType, DWORD pKey, DWORD dwTime, float dx, float px, float py)
{
	stGAME_OBJECT stObj;
	stObj.objType = objType;
	stObj.dwPKey = pKey;
	stObj.dwTime = dwTime;
	stObj.dx = dx;
	stObj.px = px;
	stObj.py = py;

	m_GameQueue_Object.push(stObj);
}

bool CGameRun::CheckData()
{
	while(!m_GameQueue_Room_Data.empty())
	{
		stGAME_ROOM_DATA stGame = m_GameQueue_Room_Data.front();
		//방 정보 날라옴
		runwindow->TimeCheck(stGame.fGameTime);
		
		m_Winner = stGame.dwWinnerPKey;
		ROOM_TYPE roomType = stGame.roomType;

		switch(roomType)
		{
		case ROOM_TYPE::RT_START :
			{
				runwindow->ShowLoadingImage(false);
			}
			break;
		case ROOM_TYPE::RT_GAMING :
			{

			}
			break;
		case ROOM_TYPE::RT_END :
			{
				CEGUI::MouseCursor::getSingletonPtr()->setVisible(true);

				if(GetPKey() == m_Winner)
				{
					resultwindow->setVisible(true);
					resultwindow->setWinLose(true);
				}
				else
				{
					resultwindow->setVisible(true);
					resultwindow->setWinLose(false);
				}
			}
			break;
		default:
			break;
		}
		
		m_GameQueue_Room_Data.pop();
	}
	while(!m_GameQueue_Player_Insert.empty())
	{
		stGAME_PLAYER_INSERT stGame = m_GameQueue_Player_Insert.front();
	
		AddCharacter(stGame.name, stGame.pKey, stGame.jobNum, stGame.dir, stGame.pos, stGame.maxHP, stGame.maxMP, stGame.skill1Cool, stGame.skill2Cool, stGame.skill3Cool);
		m_GameQueue_Player_Insert.pop();
	}
	while(!m_GameQueue_Player_Data.empty())
	{
		stGAME_PLAYER_DATA stGame = m_GameQueue_Player_Data.front();
		
		CCharacter* pCharacter = GetCharacter(stGame.pKey);
		if(pCharacter != NULL)
		{
			switch(stGame.Type)
			{
			case GS_TYPE::GS_IDLE :
				{

				}
				break;
			case GS_TYPE::GS_ATTACK1 :
				{
					pCharacter->SetAttack(1);
					switch(pCharacter->GetJob())
					{
					case JOB_WALKER:
						SoundMan::getSingletonPtr()->playSound("walker0");
						break;

					case JOB_GUNNER:
						SoundMan::getSingletonPtr()->playSound("gunner0");
						break;

					case JOB_PSYKER:
						SoundMan::getSingletonPtr()->playSound("psyker0");
						break;

					}
				
				}
				break;
			case GS_TYPE::GS_ATTACK2 :
				{
					if(pCharacter->GetJob() == 0)
						pCharacter->SetAttack(2);
					else
						pCharacter->SetAttack(1);

					switch(pCharacter->GetJob())
					{
					case JOB_WALKER:
						SoundMan::getSingletonPtr()->playSound("walker0");
						break;

					case JOB_GUNNER:
						SoundMan::getSingletonPtr()->playSound("gunner");
						break;

					case JOB_PSYKER:
				//		SoundMan::getSingletonPtr()->playSound("psyker0");
						break;

					}


				}
				break;
			case GS_TYPE::GS_ATTACK3 :
				{
					if(pCharacter->GetJob() == 0)
						pCharacter->SetAttack(3);
					else
						pCharacter->SetAttack(1);

					
					switch(pCharacter->GetJob())
					{
					case JOB_WALKER:
						SoundMan::getSingletonPtr()->playSound("walker");
						break;

					case JOB_GUNNER:
						break;

					case JOB_PSYKER:
						break;

					}
				}
				break;
			case GS_TYPE::GS_SKILL1 :
				{
					pCharacter->SetSkill(0, true);
					switch(pCharacter->GetJob())
					{
					case JOB_WALKER:
						SoundMan::getSingletonPtr()->playSound("walker1");
						break;
					case JOB_GUNNER:
						SoundMan::getSingletonPtr()->playSound("gunner1");
						break;
					case JOB_PSYKER:
						SoundMan::getSingletonPtr()->playSound("psyker1");
						break;
					}
				}
				break;
			case GS_TYPE::GS_SKILL2 :
				{
					pCharacter->SetSkill(1, true);
					switch(pCharacter->GetJob())
					{
						case JOB_WALKER:
							SoundMan::getSingletonPtr()->playSound("walker2");
							break;
						case JOB_GUNNER:
							SoundMan::getSingletonPtr()->playSound("gunner2");
							break;
						case JOB_PSYKER:
							SoundMan::getSingletonPtr()->playSound("psyker2");
							break;
					}
				}
				break;
			case GS_TYPE::GS_SKILL3 :
				{
					pCharacter->SetSkill(2, true);
					switch(pCharacter->GetJob())
					{
					case JOB_WALKER:
				//		SoundMan::getSingletonPtr()->playSound("walker3");
						break;
					case JOB_GUNNER:
					//	SoundMan::getSingletonPtr()->playSound("gunner333");
						break;
					case JOB_PSYKER:
						SoundMan::getSingletonPtr()->playSound("psyker3");
						break;
					}
				}
				break;
			case GS_TYPE::GS_PLAYER_DELETE :
				{
					DeleteCharacter(stGame.pKey);
					return true;
				}
				break;
			default :
				{

				}
				break;
			}

			//점프체크
			//추후 이전 위치가 나중 위치보다 높을 경우 하강자세로 변경
			pCharacter->isGround(stGame.isGround);

			DWORD dwEnd = stGame.dwTime - pCharacter->getCurrentTime();
			float fCurrentTime = dwEnd / 1000.0f;
			Ogre::Vector3 currentSpeed = stGame.pos - pCharacter->getCurrentPosition();
			if(fCurrentTime != 0.0f)
			{
				currentSpeed = currentSpeed / fCurrentTime;
				//currentSpeed -= (currentSpeed/10.0f);
				/*if(stGame.dir.x == 0.0f)
				{
					currentSpeed.x = 0.0f;
				}*/
				//currentSpeed.y /= 2.0f;
				pCharacter->setCurrentSpeed(currentSpeed);
			}

			Ogre::Real Dist = pCharacter->getCurrentPosition().distance(pCharacter->getPosition());
			if(Dist >= 0.1f)
			{
				pCharacter->setPosition(pCharacter->getCurrentPosition());
			}

			pCharacter->setCurrentPosition(stGame.pos);
			pCharacter->setDirection(stGame.dir);
			pCharacter->setCurrentTime(stGame.dwTime);

			//피를 확인해서 이전피와 서버에서 반환해준 피와 다르면 피격판정
			if(pCharacter->getHP() > stGame.HP)
			{
				pCharacter->SetHit(true);
				SoundMan::getSingletonPtr()->playSound("attack");
			}
			//else
				//pCharacter->SetHit(false);


			if(pCharacter->getHP() <= 0 && stGame.HP > 0)
			{
				pCharacter->SetHit( false );
				pCharacter->ResetCharacter();
				pCharacter->ResetCharacter();
			}

			pCharacter->setHP(stGame.HP);
			pCharacter->setMP(stGame.MP);
			pCharacter->SetSkill1NowCool(stGame.Skill1NowCool);
			pCharacter->SetSkill2NowCool(stGame.Skill2NowCool);
			pCharacter->SetSkill3NowCool(stGame.Skill3NowCool);

			//게임 판정
			//모든 캐릭터의 상태를 판별해서 한명만 살아남았을 경우 그 키 값을 검사하여
			//Result로 넘겨서 Result때 검사하여 그 키 값이 자기 캐릭터가 아닐 경우 패, 맞을 경우 승을 띄운다.
		}
		m_GameQueue_Player_Data.pop();
	}
	while(!m_GameQueue_Object.empty())
	{
		stGAME_OBJECT stObj = m_GameQueue_Object.front();
		OBJECT_TYPE objType = stObj.objType;
		DWORD dwPKey = stObj.dwPKey;
		DWORD dwTime = stObj.dwTime;
		float dX = stObj.dx;
		float pX = stObj.px;
		float pY = stObj.py;

		switch(objType)
		{
		case OBJECT_TYPE::OT_HPUP:
		case OBJECT_TYPE::OT_MINE:
		case OBJECT_TYPE::OT_BULLET:
		case OBJECT_TYPE::OT_STRAY:
			{
				AddObject(objType, dwPKey, dwTime, dX, pX, pY);
			}
			break;
		case OBJECT_TYPE::OT_DESTROY:
			{
				DestroyObject(dwPKey);
			}
			break;
		default:
			break;
		}
		
		m_GameQueue_Object.pop();
	}
	return true;
}

bool CGameRun::Result()
{
	return true;
}

void CGameRun::AddObject(OBJECT_TYPE objType, DWORD pKey, DWORD dwTime, float dx, float px, float py)
{
	OBJECT_ITER _iter = m_ObjectMap.find(pKey);
	CObject *pObject = NULL;
	Ogre::Vector3 pos = Ogre::Vector3(px, py, 0.0f);
	if(_iter == m_ObjectMap.end())
	{
		pObject = new CObject(mSceneMgr, pos, dwTime, objType);
		pObject->Initialize(pKey, dx);
		m_ObjectMap.insert(OBJECT_MAP::value_type(pKey, pObject));
	}
	else
		pObject = _iter->second;


	DWORD dwEnd = dwTime - pObject->GetCurrentTime();
	float fCurrentTime = dwEnd / 1000.0f;
	Ogre::Vector3 currentSpeed = pos - pObject->GetCurrentPosition();
	if(fCurrentTime != 0.0f)
	{
		currentSpeed = currentSpeed / fCurrentTime;
		pObject->SetCurrentSpeed(currentSpeed);
	}

	Ogre::Real Dist = pObject->GetCurrentPosition().distance(pObject->GetPosition());
	if(Dist >= 0.1f)
	{
		pObject->SetPosition(pObject->GetCurrentPosition());
	}

	pObject->SetCurrentPosition(pos);
	pObject->SetCurrentTime(dwTime);
}

void CGameRun::DestroyObject(DWORD pKey)
{
	OBJECT_ITER _iter = m_ObjectMap.find(pKey);
	if(_iter != m_ObjectMap.end())
	{
		delete _iter->second;
		m_ObjectMap.erase(_iter);
	}
}

/*================================================================================
                               Objects
================================================================================*/
CObject::CObject(Ogre::SceneManager *sceneMgr, Ogre::Vector3 pos, DWORD dwTime, OBJECT_TYPE _ObjectType)
{
	mSceneMgr = sceneMgr;
	mChar = NULL;
	mTargetNode = NULL;
	mCurrentSpeed = Ogre::Vector3(0,0,0);
	mCurrentPosition = pos;
	ObjectType = _ObjectType;
	mCurrentTime = dwTime;
}

CObject::~CObject()
{
	Ogre::Entity *delEnt = static_cast<Ogre::Entity*>(mTargetNode->getAttachedObject(mEntityName));
	delEnt->setVisible(false);
	mSceneMgr->destroyEntity(mEntityName);
	mSceneMgr->destroySceneNode(mTargetNode);
}

void CObject::Initialize(DWORD _ObjectNum, float direction)
{
	switch(ObjectType)
	{
	case OT_HPUP:
		mEntityName = "HP_UP_" + Ogre::StringConverter::toString(_ObjectNum);
		mChar = mSceneMgr->createEntity(mEntityName, "mineSphere001.mesh");
		break;
	case OT_BULLET:
		mEntityName = "BULLET_" + Ogre::StringConverter::toString(_ObjectNum);
		mChar = mSceneMgr->createEntity(mEntityName, "bulletCylinder001.mesh");
		break;
	case OT_STRAY:
		mEntityName = "STRAY_" + Ogre::StringConverter::toString(_ObjectNum);
		mChar = mSceneMgr->createEntity(mEntityName, "straybulletCylinder001.mesh");
		break;
	case OT_MINE:
		mEntityName = "MINE_" + Ogre::StringConverter::toString(_ObjectNum);
		mChar = mSceneMgr->createEntity(mEntityName, "mineSphere001.mesh");
		break;
	default :
		return;
	}
	mTargetNode = mSceneMgr->getRootSceneNode()->createChildSceneNode();
	mTargetNode->attachObject(mChar);

	if( direction > 0)
	{
		mTargetNode->yaw(Ogre::Degree(360*direction));
	}
	if ( direction < 0)
	{
		mTargetNode->yaw(Ogre::Degree(180*direction));  
	}

	if(ObjectType == OT_BULLET)
	{
		mTargetNode->setScale(1.5, 1.5, 1.5);
	}

	if(ObjectType == OT_STRAY)
	{
		mTargetNode->setScale(5, 5, 5);
	}
	
}

void CObject::update(const Ogre::FrameEvent &evt)
{
	mTargetNode->translate(GetCurrentSpeed()*evt.timeSinceLastFrame);
}