#include "lib.h"
#include "libMacros.h"
#include "libTypes.h"
#include "libMngInput.h"
#include "libMngSound.h"
#include "dfcApp.h"
#include "CtxGameOver.h"
#include "AppPool.h"

CCtxGameOver::CCtxGameOver(const char* p_pRes):
CContext(p_pRes)
{
};

CCtxGameOver::~CCtxGameOver()
{
}

void
CCtxGameOver::Init(void)
{
}

CPhysObj*
CCtxGameOver::UserInstall(char *p_dataSource, char* p_id)
{
CPhysObj *pRet= 0;

	return pRet;
}


void
CCtxGameOver::Activate(void)
{
	CContext::Activate();
}

void
CCtxGameOver::Deactivate(void)
{
	CContext::Deactivate();
}

void
CCtxGameOver::OnCollideObj2Line(THit *p_hitStruct, CPhysObj *p_pPhysObj)
{
	CContext::OnCollideObj2Line(p_hitStruct, p_pPhysObj);
}

void
CCtxGameOver::OnCollideObj2Obj(THit *p_hitStruct, CPhysObj *p_pPhysObj1, CPhysObj *p_pPhysObj2)
{
	CContext::OnCollideObj2Obj(p_hitStruct, p_pPhysObj1, p_pPhysObj2);
}

void
CCtxGameOver::Manage(unsigned long p_nTimeTick)
{
CMngInput *mngInput= GetMngInput();

	if(mngInput->IsMouseLogicKeyDown(0))
	{
		g_TheAppPool->m_nbBallLeft= 3;

		g_TheApp->SetCurContext(CTX_TITLE);

	}
	CContext::Manage(p_nTimeTick);
}

void
CCtxGameOver::Update(void)
{
CMngGraphic *pMngGraphic= GetMngGraphic();

	pMngGraphic->FillBitmap(pMngGraphic->GetBB(), 0);

	CContext::Update();
}
