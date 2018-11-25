#include "lib.h"
#include "libMacros.h"
#include "libTypes.h"
#include "libMngInput.h"
#include "libMngSound.h"
#include "dfcApp.h"
#include "CtxOption.h"
#include "CtxPool.h"
#include "AppPool.h"


CCtxOption::CCtxOption(const char* p_pRes):
CContext(p_pRes)
{
	m_pMusicOption= 0;
}

CCtxOption::~CCtxOption()
{
}

void
CCtxOption::Init(void)
{
}

void
CCtxOption::Activate(void)
{
	CContext::Activate();

	if(g_TheAppPool->m_hasAudio){
		m_pMusicOption= new CMusic();
		m_pMusicOption->LoadInstance("MusicOption.sound");
		m_pMusicOption->SetLoop(true);
		m_pMusicOption->Play();

		AddChild(m_pMusicOption, false);
	}
}

void
CCtxOption::Deactivate(void)
{
	CContext::Deactivate();
}

void
CCtxOption::Manage(unsigned long p_nTimeTick)
{
CMngInput *mngInput= GetMngInput();

	if(mngInput->IsMouseLogicKeyDown(0))
	{
		g_TheAppPool->m_nbBallLeft= 3;

		g_TheApp->SetCurContext(CTX_POOL);

	}
	CContext::Manage(p_nTimeTick);
}

void
CCtxOption::Update(void)
{
	CContext::Update();
}
