// -*- C++ -*-

//=============================================================================
/**
 *  @file    os_shm.h
 *
 *  XSI shared memory facility
 *
 *  @author Don Hinton <dhinton@dresystems.com>
 *  @author This code was originally in various places including ace/OS.h.
 */
//=============================================================================

#ifndef IPC_OS_INCLUDE_SYS_OS_SHM_H
#define IPC_OS_INCLUDE_SYS_OS_SHM_H

#include <sys/shm.h>
#include <iostream>
using namespace std;

#define DEFAULT_FILE_PERMS 0600

// Place all additions (especially function declarations) within extern "C" {}
#ifdef __cplusplus
extern "C"
{
#endif /* __cplusplus */

  // Declare opaque type.  Needed for ACE_OS wrappers on systems
  // without SysV IPC.
  struct shmid_ds;

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* IPC_OS_INCLUDE_SYS_OS_SHM_H */
