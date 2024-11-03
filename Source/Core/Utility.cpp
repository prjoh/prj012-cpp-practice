/*
 * Utility.cpp - collection of core utility functionality
 * Copyright (c) 2024 Johannes Przybilla. All Rights Reserved.
 */

#include <Core/PlatformContext.h>
#include <Core/Utility.h>

#if OS_WINDOWS
#include <windows.h>
#endif

bool zv::is_debugger_present()
{
#if OS_WINDOWS
  return IsDebuggerPresent();
#else
  int mib[4];
  struct kinfo_proc info;
  size_t size;

  info.kp_proc.p_flag = 0;
  mib[0] = CTL_KERN;
  mib[1] = KERN_PROC;
  mib[2] = KERN_PROC_PID;
  mib[3] = getpid();

  size = sizeof(info);
  sysctl(mib, sizeof(mib) / sizeof(*mib), &info, &size, NULL, 0);

  return ((info.kp_proc.p_flag & P_TRACED) != 0);
#endif
}