/**
 * vim: set ts=4 :
 * =============================================================================
 * SourceMod
 * Copyright (C) 2004-2008 AlliedModders LLC.  All rights reserved.
 * =============================================================================
 *
 * This program is free software; you can redistribute it and/or modify it under
 * the terms of the GNU General Public License, version 3.0, as published by the
 * Free Software Foundation.
 * 
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 * FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more
 * details.
 *
 * You should have received a copy of the GNU General Public License along with
 * this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 * As a special exception, AlliedModders LLC gives you permission to link the
 * code of this program (as well as its derivative works) to "Half-Life 2," the
 * "Source Engine," the "SourcePawn JIT," and any Game MODs that run on software
 * by the Valve Corporation.  You must obey the GNU General Public License in
 * all respects for all other code used.  Additionally, AlliedModders LLC grants
 * this exception to all derivative works.  AlliedModders LLC defines further
 * exceptions, found in LICENSE.txt (as of this writing, version JULY-31-2007),
 * or <http://www.sourcemod.net/license.php>.
 *
 * Version: $Id$
 */

#ifndef _INCLUDE_WINTHREADS_H_
#define _INCLUDE_WINTHREADS_H_

#include <am-thread-utils.h>
#include <am-utility.h>
#include <windows.h>
#include "IThreader.h"

using namespace SourceMod;

class WinThreader : public IThreader
{
public:
	class ThreadHandle : public IThreadHandle
	{
		friend class WinThreader;
		friend DWORD WINAPI Win32_ThreadGate(LPVOID param);
	public:
		ThreadHandle(IThreader *parent, IThread *run, const ThreadParams *params);
		virtual ~ThreadHandle();
	public:
		virtual bool WaitForThread();
		virtual void DestroyThis();
		virtual IThreadCreator *Parent();
		virtual void GetParams(ThreadParams *ptparams);
		virtual ThreadPriority GetPriority();
		virtual bool SetPriority(ThreadPriority prio);
		virtual ThreadState GetState();
		virtual bool Unpause();
		virtual void Run();
	protected:
		IThreader *m_parent;		//Parent handle
		ke::AutoPtr<ke::Thread> m_thread;
		ThreadParams m_params;		//Current Parameters
		IThread *m_run;				//Runnable context
		ThreadState m_state;		//internal state
		ke::ConditionVariable suspend_;
	};

public:
	IMutex *MakeMutex();
	void MakeThread(IThread *pThread);
	IThreadHandle *MakeThread(IThread *pThread, ThreadFlags flags);
	IThreadHandle *MakeThread(IThread *pThread, const ThreadParams *params);
	void GetPriorityBounds(ThreadPriority &max, ThreadPriority &min);
	void ThreadSleep(unsigned int ms);
	IEventSignal *MakeEventSignal();
	IThreadWorker *MakeWorker(IThreadWorkerCallbacks *hooks, bool threaded);
	void DestroyWorker(IThreadWorker *pWorker);
};

#if defined SM_DEFAULT_THREADER && !defined SM_MAIN_THREADER
#define SM_MAIN_THREADER WinThreader;
typedef class WinThreader MainThreader;
#endif

#endif //_INCLUDE_WINTHREADS_H_
