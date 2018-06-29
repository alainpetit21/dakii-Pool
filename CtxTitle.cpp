#include "lib.h"
#include "libMacros.h"
#include "libTypes.h"
#include "libMngInput.h"
#include "libMngSound.h"
#include "dfcApp.h"
#include "CtxTitle.h"
#include "AppPool.h"


CCtxTitle::CCtxTitle(const char* p_pRes):
CContext(p_pRes)
{
	m_pMusicTitle= 0;
};

CCtxTitle::~CCtxTitle()
{
}

void
CCtxTitle::Init(void)
{
}

CPhysObj*
CCtxTitle::UserInstall(char *p_dataSource, char* p_id)
{
CPhysObj *pRet= 0;

	return pRet;
}


void
CCtxTitle::Activate(void)
{
	CContext::Activate();

	if(g_TheAppPool->m_hasAudio){
		m_pMusicTitle= new CMusic();
		m_pMusicTitle->LoadInstance("MusicTitle.sound");
		m_pMusicTitle->Play();

		AddChild(m_pMusicTitle, false);
	}
}

void
CCtxTitle::Deactivate(void)
{
	CContext::Deactivate();
}

void
CCtxTitle::OnCollideObj2Line(THit *p_hitStruct, CPhysObj *p_pPhysObj)
{
	CContext::OnCollideObj2Line(p_hitStruct, p_pPhysObj);
}

void
CCtxTitle::OnCollideObj2Obj(THit *p_hitStruct, CPhysObj *p_pPhysObj1, CPhysObj *p_pPhysObj2)
{
	CContext::OnCollideObj2Obj(p_hitStruct, p_pPhysObj1, p_pPhysObj2);
}

void
CCtxTitle::Manage(unsigned long p_nTimeTick)
{
CMngInput *mngInput= GetMngInput();

	if(mngInput->IsMouseLogicKeyDown(0))
		g_TheApp->SetCurContext(CTX_OPTION);

	if(mngInput->IsLogicKeyDown(IK_ESCAPE))
		System_Quit();

//	Sleep(1000);
	CContext::Manage(p_nTimeTick);
}

void
CCtxTitle::Update(void)
{
CMngGraphic *pMngGraphic= GetMngGraphic();

	pMngGraphic->FillBitmap(pMngGraphic->GetBB(), 0);

	CContext::Update();
}
