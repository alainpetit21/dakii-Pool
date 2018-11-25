//#define _DEBUG_UNIT
//#define _UNIT_TEST

#include "lib.h"
#include "libMacros.h"
#include "libTypes.h"
#include "libMath.h"
#include "libMngInput.h"
#include "libMngSound.h"
#include "dfcApp.h"
#include "CtxPool.h"
#include "AppPool.h"

const char* pWorld[]=
{
	"CtxPool00.context",
	"CtxPool01.context",
	"CtxPool02.context",
	"CtxPool03.context",
	"CtxPool04.context",
	"CtxPool05.context",
	"CtxPool06.context",
	"CtxPool07.context",
	"CtxPool08.context",
	"CtxPool09.context",
};

#define DELAY_TEST 32000

CBall::CBall(void):
CEntity()
{
	m_incZoom= 0.00f;
	m_isCollidable	= true;
	m_radius	= 11;
	m_zoom		= 0.0f;
//	Hide();
}

void
CBall::Appear(void)
{
	Show();
	m_zoom= 0.0f;
	m_incZoom= 0.05f;
}

void
CBall::Disappear(void)
{
	m_incZoom= -0.05f;
}

void
CBall::Manage(u32 p_timeTick)
{
	m_zoom+= (m_incZoom/16.667f)*p_timeTick;

	if(m_zoom >= 1.0f){
		m_incZoom= 0.0f;
		m_zoom= 1.0f;
	}else if(m_zoom <= 0.0001f){
		m_incZoom= 0.0f;
		m_zoom= 0.0f;
		Hide();
	}

	CEntity::Manage(p_timeTick);
}

CHole::CHole(void):
CEntity()
{
	m_isCollidable	= true;
	m_radius	= 7;
	m_zoom		= 1.0f;
}

CCtxPool::CCtxPool(const char* p_pRes):
CContext(p_pRes),
m_isBallMoving(0)
{
	m_pMusicIntro		= 0;
	m_pMusicGame		= 0;
	m_pSoundCollideBalls= 0;
	m_pSoundCollideWalls= 0;
	m_pSoundWhiteFail	= 0;
	m_pSoundBallsScore	= 0;
}

CCtxPool::~CCtxPool(void)
{
}

void
CCtxPool::Init(void)
{
	SetNbChilds(64);

#ifdef _UNIT_TEST
	m_velStart	= 1.01f;
	m_angThrow	= 0.00f;
	m_delay		= DELAY_TEST;
#endif
}

CPhysObj*
CCtxPool::UserInstall(const char *p_dataSource, const char* p_id)
{
CPhysObj *pRet= 0;

	if(strcmp(p_id, "CLSID_CBall") == 0)
		pRet= new CBall;
	else if(strcmp(p_id, "CLSID_CHole") == 0)
		pRet= new CHole;

	return pRet;
}

void
CCtxPool::Activate(void)
{
CMngSound *mngSound= GetMngSound();

	LoadInstance(pWorld[g_TheAppPool->m_curLevel]);

	SetMouse((CEntity*)FindByID("Baton"));

	((CLabel*)FindByID("LblGameStageValue"))->Format("%02d", g_TheAppPool->m_curLevel+1);
	((CLabel*)FindByID("LblGameBallValue"))->Format("%02d", g_TheAppPool->m_nbBallLeft);

	if(g_TheAppPool->m_hasAudio){
		m_pSoundCollideBalls= new CSound();
		m_pSoundCollideWalls= new CSound();
		m_pSoundWhiteFail	= new CSound();
		m_pSoundBallsScore	= new CSound();
		m_pSoundCollideWalls->LoadInstance("CollisionWalls.sound");
		m_pSoundCollideBalls->LoadInstance("CollisionBalls.sound");
		m_pSoundWhiteFail->LoadInstance("BallWhiteFail.sound");
		m_pSoundBallsScore->LoadInstance("BallScore.sound");

		m_pMusicIntro= new CMusic();
		m_pMusicIntro->LoadInstance("MusicIntro.sound");
		m_pMusicIntro->Play();

		m_pMusicGame= new CMusic();
		m_pMusicGame->LoadInstance("MusicGame.sound");
		m_pMusicGame->SetLoop(true);

		AddChild(m_pMusicIntro, false);
		AddChild(m_pMusicGame, false);
	}

	m_posSaved		= new s32[2*GetNbBallsChilds()];
	m_stateSaved	= new bool[GetNbBallsChilds()];
	m_idxBallSuccess= new u32[GetNbBallsChilds()];
	m_nbBallSuccess	= 0;

	m_hasPendingReload= false;

	((CBall*)FindByID("BallWhite"))->m_idxBall= 0;
	for(u32 i= 0; i < GetNbBallsChilds() - 1; ++i){
		char textID[32];
		sprintf(textID, "Ball%d", int(i + 1));
		((CBall*)FindByID(textID))->m_idxBall= i + 1;
	}
	SaveBallsPositions();

	//Test Unit
#ifdef _UNIT_TEST
	((CPhysObj*)FindByID("BallWhite"))->SetVelX(cos(m_angThrow)*m_velStart);
	((CPhysObj*)FindByID("BallWhite"))->SetVelY(sin(m_angThrow)*m_velStart);
#endif

	//Debug
#ifdef _DEBUG_UNIT
	((CPhysObj*)FindByID("BallWhite"))->SetVelX(cos(7.25f)*1.01f);
	((CPhysObj*)FindByID("BallWhite"))->SetVelY(sin(7.25f)*1.01f);
#endif

	if(!m_pMusicIntro || !mngSound->IsOn())
	{
		for(u32 j=0; j < GetNbChilds(); ++j)
		{
			CObj *pObj= GetChild(j);

			if(pObj->IsKindOf((DFC_CLASSID)CLSID_CBall))
			{
				CBall *pBall= (CBall *)pObj;
				pBall->Appear();
			}
		}
	}


}

void
CCtxPool::Deactivate(void)
{
	delete [] m_posSaved;
	delete [] m_stateSaved;
	delete [] m_idxBallSuccess;

	delete m_pSoundCollideWalls;
	delete m_pSoundCollideBalls;
	delete m_pSoundWhiteFail;
	delete m_pSoundBallsScore;


#ifdef _UNIT_TEST
	char temp[256];

	sprintf(temp, "\n%.2f,%.2f,%d", m_velStart, m_angThrow, m_delay);
	OutputDebugStr(temp);

	m_angThrow	+= 0.01f;

	if(m_angThrow >= 8.0f)
	{
		m_velStart+= 0.01f;
		m_angThrow=	0.0f;
	}
	m_delay		= DELAY_TEST;
#endif

	CContext::Deactivate();
}

void
CCtxPool::OnCollideObj2Line(THit *p_hitStruct, CPhysObj *p_pPhysObj)
{
	CContext::OnCollideObj2Line(p_hitStruct, p_pPhysObj);

	if(m_pSoundCollideWalls)
		m_pSoundCollideWalls->Play();
}

void
CCtxPool::OnCollideObj2Obj(THit *p_hitStruct, CPhysObj *p_pPhysObj1, CPhysObj *p_pPhysObj2)
{
	CContext::OnCollideObj2Obj(p_hitStruct, p_pPhysObj1, p_pPhysObj2);

	if((p_pPhysObj1->Rtti() == (DFC_CLASSID)CLSID_CHole) && (p_pPhysObj2->Rtti() == (DFC_CLASSID)CLSID_CBall)){
		CBall *pBall= (CBall *)p_pPhysObj2;

		p_pPhysObj1->SetVel(0, 0, 0);
		p_pPhysObj2->SetPos(p_pPhysObj1->GetPosX(), p_pPhysObj1->GetPosY(), p_pPhysObj1->GetPosZ());

		pBall->SetVel(0, 0, 0);
		pBall->Disappear();
		pBall->m_isCollidable= false;

		if(strcmp(p_pPhysObj2->GetTextID(), "BallWhite") == 0){
			if(m_pSoundWhiteFail)
				m_pSoundWhiteFail->Play();
			g_TheAppPool->m_nbBallLeft--;
		}else
			if(m_pSoundBallsScore)
				m_pSoundBallsScore->Play();
	}else if((p_pPhysObj2->Rtti() == (DFC_CLASSID)CLSID_CHole) && (p_pPhysObj1->Rtti() == (DFC_CLASSID)CLSID_CBall)){
		CBall *pBall= (CBall *)p_pPhysObj1;

		p_pPhysObj2->SetVel(0, 0, 0);
		p_pPhysObj1->SetPos(p_pPhysObj1->GetPosX(), p_pPhysObj1->GetPosY(), p_pPhysObj1->GetPosZ());

		pBall->SetVel(0, 0, 0);
		pBall->Disappear();
		pBall->m_isCollidable= false;

		if(strcmp(p_pPhysObj1->GetTextID(), "BallWhite") == 0){
			if(m_pSoundWhiteFail)
				m_pSoundWhiteFail->Play();
			g_TheAppPool->m_nbBallLeft--;
		}else
			if(m_pSoundBallsScore)
				m_pSoundBallsScore->Play();
	}else
		if(m_pSoundCollideBalls)
			m_pSoundCollideBalls->Play();
}

void
CCtxPool::KeyManage(unsigned long p_nTimeTick)
{
CMngInput *mngInput= GetMngInput();

	//Keyboard Logic
	if(mngInput->IsLogicKeyDown(IK_SPACE))
		g_TheApp->SetCurContext(g_TheApp->GetCurContextID());

	if(mngInput->IsLogicKeyDown(IK_ESCAPE))
		g_TheApp->SetCurContext(CTX_TITLE);

	if(mngInput->IsLogicKeyDown(IK_ADD)){
		if(++g_TheAppPool->m_curLevel > 9)
			g_TheAppPool->m_curLevel= 9;

		g_TheApp->SetCurContext(g_TheApp->GetCurContextID());
	}
	if(mngInput->IsLogicKeyDown(IK_SUBTRACT)){
		if(--g_TheAppPool->m_curLevel < 0)
			g_TheAppPool->m_curLevel= 0;

		g_TheApp->SetCurContext(g_TheApp->GetCurContextID());
	}

	//Mouse Logic
	if(mngInput->IsMouseKeyDown(1)){	//This is mostly for debug to place object in place
		CObj* pFound;

		if((pFound= FindAt(m_pMouse->GetIntPosX(), m_pMouse->GetIntPosY(), m_pMouse->GetIntPosZ())))
			if(m_pMouse->GetNbChilds() == 0)
				m_pMouse->AddChild(RemoveChild(pFound), true);
	}else if(mngInput->IsMouseLogicKeyDown(0) && !m_isBallMoving){
		if(!GetMngSound()->IsOn() || m_pMusicIntro->IsFinished()){
			float vector[3]= {(m_pMouse->GetPosX() - ((CPhysObj*)FindByID("BallWhite"))->GetPosX()) / 15,
							   (m_pMouse->GetPosY() - ((CPhysObj*)FindByID("BallWhite"))->GetPosY()) / 15, 0};
			float len= CMngMath::VectorGetLenght(vector);

			if(len > 15)
				CMngMath::VectorScale(vector, vector, 15/len);

			((CPhysObj*)FindByID("BallWhite"))->SetVelX(vector[0]);
			((CPhysObj*)FindByID("BallWhite"))->SetVelY(vector[1]);
		}
	}else{	//This is mostly for debug to place object in place
		if(m_pMouse && m_pMouse->m_arChilds && m_pMouse->m_arChilds[0])
			AddChild(m_pMouse->RemoveChild(m_pMouse->m_arChilds[0]), false);
	}
}

void
CCtxPool::Manage(unsigned long p_nTimeTick)
{
	if(g_TheAppPool->m_nbBallLeft == 0){
		g_TheApp->SetCurContext(CTX_GAMEOVER);
		return;
	}

	//Intro Of the round
	if(m_pMusicIntro && m_pMusicIntro->IsPlaying()){
		if(m_pMusicIntro->IsFinished()){
			m_pMusicIntro->Stop();
			m_pMusicGame->Play();
		}
	}

	//Apply Friction && Detect Movment
	m_isBallMoving= false;

	for(u32 i=0; i < GetNbChilds(); ++i){
		CObj *pObj= GetChild(i);

		if(pObj->IsKindOf((DFC_CLASSID)CLSID_CBall)){
			CBall *pBall= (CBall *)pObj;

			float vec[3]= {pBall->GetVelX(), pBall->GetVelY(), 0};
			float len= CMngMath::VectorGetLenght(vec);

			if(len > 0){
				vec[0]/= len;
				vec[1]/= len;
				if((len-= 0.125f) <= 0.125f)
					len= 0;

				pBall->SetVelX(vec[0]*len);
				pBall->SetVelY(vec[1]*len);

			}
			if((len > 0) || (pBall->m_zoom >= 0.0001f && pBall->m_zoom < 1.0f))
				m_isBallMoving= true;
		}
	}

	//No balls moving ... we calculate score and reload balls if fallen
	if(!m_isBallMoving){
		if(m_hasPendingReload  && (!m_pMusicIntro || !m_pMusicIntro->IsPlaying()))
			ReloadLastBallsPosition();

		if(!((CPhysObj*)FindByID("BallWhite"))->m_isShown)
			m_hasPendingReload= true;

		if(!m_hasPendingReload)
			SaveBallsPositions();

		//If All the balls are in ->CHange level
		bool	levelFinished= true;
		for(u32 i=0; i < GetNbChilds(); ++i){
			CPhysObj *pObj= (CPhysObj *)GetChild(i);

			if(pObj->IsKindOf((DFC_CLASSID)CLSID_CBall) && (pObj != FindByID("BallWhite"))){
				if(pObj->m_isShown)
					levelFinished= false;
			}

		}

		if(levelFinished && (!m_pMusicIntro || !m_pMusicIntro->IsPlaying())){
			CPhysObj *pObj= (CPhysObj *)FindByID("BallWhite");

			if(pObj->m_isShown){
				if(++g_TheAppPool->m_curLevel > 9)
					g_TheAppPool->m_curLevel= 9;

				g_TheApp->SetCurContext(g_TheApp->GetCurContextID());
			}
		}
	}

	//White has Fallen Make other disappear
	if(m_hasPendingReload){
		for(u32 i=0; i < GetNbChilds(); ++i){
			CObj *pObj= GetChild(i);

			if(pObj->IsKindOf((DFC_CLASSID)CLSID_CBall)){
				CBall *pBall= (CBall*)pObj;

				pBall->Disappear();
			}
		}
	}

	//Debug Unit
#ifdef _DEBUG_UNIT

	if(--m_delay == 8141){
		int toto=34;
		float posX= ((CPhysObj*)FindByID("BallWhite"))->GetPosX();
		float posY= ((CPhysObj*)FindByID("BallWhite"))->GetPosY();

		float velX= ((CPhysObj*)FindByID("BallWhite"))->GetVelX();
		float velY= ((CPhysObj*)FindByID("BallWhite"))->GetVelY();
		float velZ= ((CPhysObj*)FindByID("BallWhite"))->GetVelZ();
	}
#endif

	//Test Unit
#ifdef _UNIT_TEST
	if(--m_delay == 0)
		g_TheApp->SetCurContext(g_TheApp->GetCurContextID());
	else{
		float posX= ((CPhysObj*)FindByID("BallWhite"))->GetPosX();
		float posY= ((CPhysObj*)FindByID("BallWhite"))->GetPosY();

		if((posX < -320) || (posX > 320) || (posY < -240) || (posY > 240))
			g_TheApp->SetCurContext(g_TheApp->GetCurContextID());
	}
#endif


	CContext::Manage(p_nTimeTick);
}

u32
CCtxPool::GetNbBallsChilds(void)
{
u32 nbBalls= 0;

	for(u32 i= 0; i < GetNbChilds(); ++i){
		if(GetChild(i)->IsKindOf((DFC_CLASSID)CLSID_CBall)){
			++nbBalls;
		}
	}

	return nbBalls;
}

void
CCtxPool::SaveBallsPositions(void)
{
	for(u32 i=0; i < GetNbChilds(); ++i){
		CObj *pObj= GetChild(i);

		if(pObj->IsKindOf((DFC_CLASSID)CLSID_CBall)){
			CBall *pBall= (CBall *)pObj;

			m_posSaved[pBall->m_idxBall*2+0]= pBall->GetIntPosX();
			m_posSaved[pBall->m_idxBall*2+1]= pBall->GetIntPosY();
			m_stateSaved[pBall->m_idxBall]= pBall->m_isShown;
		}
	}
}

void
CCtxPool::ReloadLastBallsPosition(void)
{
	((CLabel*)FindByID("LblGameBallValue"))->Format("%02d", g_TheAppPool->m_nbBallLeft);

	for(u32 i=0; i < GetNbChilds(); ++i){
		CObj *pObj= GetChild(i);

		if(pObj->IsKindOf((DFC_CLASSID)CLSID_CBall)){
			CBall *pBall= (CBall *)pObj;

			pBall->SetPosX(m_posSaved[pBall->m_idxBall*2+0]);
			pBall->SetPosY(m_posSaved[pBall->m_idxBall*2+1]);

			if(m_stateSaved[pBall->m_idxBall]){
				pBall->Appear();
				pBall->m_isCollidable= true;
			}
		}
	}

	m_hasPendingReload= false;
}
