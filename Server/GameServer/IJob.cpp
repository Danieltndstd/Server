#include "stdafx.h"
#include "IJob.h"

VideoJob::VideoJob()
{
	ZeroMemory(&m_dbr, sizeof m_dbr);
}

bool VideoJob::DoJob(CAdoAdapter* pAdoApater)
{
	CStringA cmdText; 

	cmdText.Format("INSERT into QPGameWebDB.dbo.GameVideo(ServerName, FileName, Content) VALUES('%s', '%s','%s')", 
		m_dbr.szRoomName, m_dbr.szFileName, m_dbr.cbStreamContent);

	pAdoApater->ClearParemeter();
	
	return pAdoApater->ExecuteProcess(cmdText.GetBuffer(), false);
}
