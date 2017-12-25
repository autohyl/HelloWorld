// -*- C++ -*-

//=============================================================================
/**
 *  @file   SV_Semaphore_Complex.h
 *
 *  @author Douglas C. Schmidt <schmidt@cs.wustl.edu>
 */
//=============================================================================

#ifndef SV_SEMAPHORE_COMPLEX_H
#define SV_SEMAPHORE_COMPLEX_H

#include "SV_Semaphore_Simple.h"
#include "include/os_sem.h"

/**
 * @class SV_Semaphore_Complex
 *
 * @brief This is a more complex semaphore wrapper that handles race
 * conditions for initialization correctly...
 *
 * This code is a port to C++, inspired by: W. Richard Stevens
 * from his book: UNIX Network Programming (Prentice Hall, ISBN
 * 0-13-949876-1 - 1990).  We provide a simpler and easier to
 * understand interface to the System V Semaphore system calls.
 * We create and use a 2 + n-member set for the requested
 * SV_Semaphore_Complex. The first member, [0], is a
 * counter used to know when all processes have finished with
 * the SV_Semaphore_Complex.  The counter is initialized
 * to a large number, decremented on every create or open and
 * incremented on every close. This way we can use the "adjust"
 * feature provided by System V so that any process that exit's
 * without calling <close> is accounted for. It doesn't help us
 * if the last process does this (as we have no way of getting
 * control to remove the SV_Semaphore_Complex) but it
 * will work if any process other than the last does an exit
 * (intentional or unintentional).
 * The second member, [1], of the semaphore is used as a lock
 * variable to avoid any race conditions in the create() and
 * close() functions.
 * The members beyond [1] are actual semaphore values in the
 * array of semaphores, which may be sized by the user in the
 * constructor.
 */
class SV_Semaphore_Complex : private SV_Semaphore_Simple
{
public:
  enum
  {
    CREATE = IPC_CREAT,
    OPEN   = 0
  };

  // = Initialization and termination methods.
  SV_Semaphore_Complex (void);
  SV_Semaphore_Complex (key_t key,
                            short create = SV_Semaphore_Complex::CREATE,
                            int initial_value = 1,
                            u_short nsems = 1,
                            mode_t perms = SEM_DEFAULT_FILE_PERMS);
  SV_Semaphore_Complex (const char *name,
                            short create = SV_Semaphore_Complex::CREATE,
                            int initial_value = 1,
                            u_short nsems = 1,
                            mode_t perms = SEM_DEFAULT_FILE_PERMS);
  ~SV_Semaphore_Complex (void);

  /// Open or create an array of SV_Semaphores.  We return 0 if all is
  /// OK, else -1.
  int open (const char *name,
            short flags = SV_Semaphore_Simple::CREATE,
            int initial_value = 1,
            u_short nsems = 1,
            mode_t perms = SEM_DEFAULT_FILE_PERMS);

  /// Open or create an array of SV_Semaphores.  We return 0 if all is
  /// OK, else -1.
  int open (key_t key,
            short flags = SV_Semaphore_Simple::CREATE,
            int initial_value = 1,
            u_short nsems = 1,
            mode_t perms = SEM_DEFAULT_FILE_PERMS);

  /**
   * Close an ACE_SV_Semaphore. Unlike the remove() method, this
   * method is for a process to call before it exits, when it is done
   * with the ACE_SV_Semaphore. We "decrement" the counter of
   * processes using the ACE_SV_Semaphore, and if this was the last
   * one, we can remove the ACE_SV_Semaphore.
   */
  int close (void);

  // = Semaphore acquire and release methods.

  /// Acquire the semaphore.
  int acquire (u_short n = 0, short flags = 0) const;

  /// Acquire a semaphore for reading.
  int acquire_read (u_short n = 0, short flags = 0) const;

  /// Acquire a semaphore for writing
  int acquire_write (u_short n = 0, short flags = 0) const;

  /// Try to acquire the semaphore.
  int tryacquire (u_short n = 0, short flags = 0) const;

  /// Try to acquire the semaphore for reading.
  int tryacquire_read (u_short n = 0, short flags = 0) const;

  /// Try to acquire the semaphore for writing.
  int tryacquire_write (u_short n = 0, short flags = 0) const;

  /// Release the semaphore.
  int release (u_short n = 0, short flags = 0) const;

  // = Semaphore operation methods.
  int op (short val, u_short n = 0, short flags = 0) const;
  int op (sembuf op_vec[], u_short n) const;

  // = Semaphore control methods.
  int control (int cmd, semun arg, u_short n = 0) const;
  int control (int cmd, int value = 0, u_short n = 0) const;

  // = Upgrade access control...
  using SV_Semaphore_Simple::get_id;
  using SV_Semaphore_Simple::remove;

  /// Dump the state of an object.
  void dump (void) const;

private:
  static const int BIGCOUNT_;
  static sembuf op_lock_[2];
  static sembuf op_endcreate_[2];
  static sembuf op_open_[1];
  static sembuf op_close_[3];
  static sembuf op_unlock_[1];
};

#include "SV_Semaphore_Complex.inl"

#endif /* SV_SEMAPHORE_COMPLEX_H */
