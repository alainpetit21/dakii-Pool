#include "lib.h"
#include "libTypes.h"
#include "libMacros.h"
#include "AppPool.h"

#include "CtxPool.h"
#include "CtxTitle.h"
#include "CtxOption.h"
#include "CtxGameOver.h"

CAppPool	*g_TheAppPool;

CAppPool::CAppPool():
CApp("AppPool.application"),
m_curLevel(0),
m_nbBallLeft(3)
{
}

void
CAppPool::OnPostCreate(void)
{
	Init(MAX_CTX);

	RegisterContext(CTX_TITLE, new CCtxTitle("CtxTitle.context"));
	RegisterContext(CTX_OPTION, new CCtxOption("CtxOption.context"));
	RegisterContext(CTX_GAMEOVER, new CCtxGameOver("CtxGameOver.context"));
	RegisterContext(CTX_POOL, new CCtxPool(0));

	SetCurContext(CTX_TITLE);

	CApp::OnPostCreate();
}

CApp*
newGameApp(void)
{
	return g_TheAppPool= new CAppPool();
}


ENTRY_POINT("Pool");
