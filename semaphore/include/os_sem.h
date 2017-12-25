// -*- C++ -*-

//=============================================================================
/**
 *  @file    os_sem.h
 *
 *  XSI semaphore facility
 *
 *  @author Don Hinton <dhinton@dresystems.com>
 *  @author This code was originally in various places including ace/OS.h.
 */
//=============================================================================

#ifndef IPC_OS_INCLUDE_SYS_OS_SEM_H
#define IPC_OS_INCLUDE_SYS_OS_SEM_H

#include <stdint.h>
#include <sys/sem.h>
#include <iostream>
#include <errno.h>
using namespace std;

#define SEM_DEFAULT_FILE_PERMS 0644

#define INVALID_SEM_KEY -1

#define BIT_ENABLED(WORD, BIT) (((WORD) & (BIT)) != 0)

#define NOTSUP_RETURN(FAILVALUE) do { errno = ENOTSUP ; return FAILVALUE; } while (0)

#define UNUSED_ARG(a) (a)

extern uint32_t crc32 (const char *string);

// Place all additions (especially function declarations) within extern "C" {}
#ifdef __cplusplus
extern "C"
{
#endif /* __cplusplus */

# if !defined (GETVAL)
#   define GETVAL 0
# endif /* GETVAL */

# if !defined (SETVAL)
#   define SETVAL 0
# endif /* SETVAL */

# if !defined (GETALL)
#   define GETALL 0
# endif /* GETALL */

# if !defined (SETALL)
#   define SETALL 0
# endif /* SETALL */

# if !defined (SEM_UNDO)
#   define SEM_UNDO 0
# endif /* SEM_UNDO */

/**
struct sembuf
{
    /// semaphore #
    unsigned short sem_num;

    /// semaphore operation
    short sem_op;

    /// operation flags
    short sem_flg;
};*/

union semun
{
    /// value for SETVAL
    int val;
    /// buffer for IPC_STAT & IPC_SET
    struct semid_ds *buf;
    /// array for GETALL & SETALL
    u_short *array;
};

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* ACE_OS_INCLUDE_SYS_OS_SEM_H */
