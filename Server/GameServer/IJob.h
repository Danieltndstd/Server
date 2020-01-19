#pragma once

#include "CAdoAdapter.h"
#include "DataBasePacket.h"

class IJob
{
public:
	virtual ~IJob(){}
	virtual bool DoJob(CAdoAdapter* pAdoApater) = NULL;
};


class VideoJob : public IJob
{

public:
	VideoJob();
	
	virtual bool DoJob(CAdoAdapter* pAdoApater);

public:
	DBR_GR_WriteVideo	m_dbr;
};
