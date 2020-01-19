#include "StdAfx.h"
#include "DebugLog.h"
#include "..\公共组件\服务核心\WHDataQueue.h"


const wchar_t INFO_MARK[] = { 29, 29, 31, 29, 29, 0 };
const wchar_t WARN_MARK[] = { 29, 31, 29, 31, 29, 0 };
const wchar_t ERR_MARK [] = { 31, 29, 31, 29, 31, 0 };

const wchar_t* DebugLogLevel<LL_INFO>::MARK = INFO_MARK;
const wchar_t* DebugLogLevel<LL_INFO>::ICON = L"───";

const wchar_t* DebugLogLevel<LL_WARN>::MARK = WARN_MARK;
const wchar_t* DebugLogLevel<LL_WARN>::ICON = L"[警告]";

const wchar_t* DebugLogLevel<LL_ERR >::MARK = ERR_MARK;
const wchar_t* DebugLogLevel<LL_ERR >::ICON = L"[错误]";


