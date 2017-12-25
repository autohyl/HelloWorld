#include "SV_Semaphore_Complex.h"

void
SV_Semaphore_Complex::dump (void) const
{
  cout << "SV_Semaphore_Complex::dump" << endl;
}

// initial value of process
const int SV_Semaphore_Complex::BIGCOUNT_ = 10000;

// Define the ACE_SV_Semaphore operation arrays for the semop() calls.
sembuf SV_Semaphore_Complex::op_lock_[2] =
{
  {0, 0, 0},                    // Wait for [0] (lock) to equal 0
  {0, 1, SEM_UNDO},             // then increment [0] to 1 - this locks it.
                                // UNDO to release the lock if processes exit
                                // before explicitly unlocking.
};

sembuf SV_Semaphore_Complex::op_endcreate_[2] =
{
  {1, -1, SEM_UNDO},            // Decrement [1] (proc counter) with undo on
                                // exit, UNDO to adjust proc counter if
                                // process exits before explicitly calling close()
  {0, -1, SEM_UNDO},            // the decrement [0] (lock) back to 0
};

sembuf SV_Semaphore_Complex::op_open_[1] =
{
  {1, -1, SEM_UNDO},            // Decrement [1] (proc counter) with undo on
                                // exit.
};

sembuf SV_Semaphore_Complex::op_close_[3] =
{
  {0, 0, 0},                    // Wait for [0] (lock) to equal 0
  {0, 1, SEM_UNDO},             // then increment [0] to 1 - this lock it
  {1, 1, SEM_UNDO},             // then increment [1] (proc counter)
};

sembuf SV_Semaphore_Complex::op_unlock_[1] =
{
  {0, -1, SEM_UNDO},            // Decrement [0] (lock) back to 0
};

// Open or create an array of SV_Semaphores.  We return 0 if all is OK, else -1.

int
SV_Semaphore_Complex::open (key_t k,
                                short create,
                                int initial_value,
                                u_short nsems,
                                mode_t perms)
{
  cout << "SV_Semaphore_Complex::open" << endl;
#ifdef HAS_SYSV_IPC
  if (k == IPC_PRIVATE)
    return -1;
#endif

  this->key_ = k;

  // Must include a count for the 2 additional semaphores we use
  // internally.
  this->sem_number_ = nsems + 2;

  if (create == SV_Semaphore_Complex::CREATE)
    {
      int result;

      do
        {
          this->internal_id_ = ::semget
            (this->key_,
             (u_short) 2 + nsems,
             perms | SV_Semaphore_Complex::CREATE);

          if (this->internal_id_ == -1)
            return -1; // permission problem or tables full

          // When the <SV_Semaphore_Complex> is created, we know
          // that the value of all 3 members is 0.  Get a lock on the
          // <SV_Semaphore_Complex> by waiting for [0] to equal 0,
          // then increment it.

          // There is a race condition here. There is the possibility
          // that between the <semget> above and the <semop> below,
          // another process can call out <close> function which can
          // remove the <ACE_SV_Semaphore> if that process is the last
          // one using it.  Therefor we handle the error condition of
          // an invalid <ACE_SV_Semaphore> ID specifically below, and
          // if it does happen, we just go back and create it again.
          result = ::semop (this->internal_id_,
                                  &SV_Semaphore_Complex::op_lock_[0],
                                  2);
        }
      while (result == -1 && (errno == EINVAL || errno == EIDRM));

      if (result == -1)
        return -1;

      // Get the value of the process counter. If it equals 0, then no
      // one has initialized the ACE_SV_Semaphore yet.

      int semval = SV_Semaphore_Simple::control (GETVAL, 0, 1);

      if (semval == -1)
#ifdef HAS_SYSV_IPC
        return this->init ();
#else
        return -1;
#endif

      else if (semval == 0)
        {
          // We should initialize by doing a SETALL, but that would
          // clear the adjust value that we set when we locked the
          // ACE_SV_Semaphore above. Instead we do system calls to
          // initialize [1], as well as all the nsems SV_Semaphores.

          if (SV_Semaphore_Simple::control (SETVAL,
                                                SV_Semaphore_Complex::BIGCOUNT_,
                                                1) == -1)
            return -1;
          else
            for (u_short i = 0; i < nsems; i++)
              if (this->control (SETVAL, initial_value, i) == -1)
                return -1;
        }

      // Decrement the process counter and then release the lock.
      return ::semop (this->internal_id_,
                            &SV_Semaphore_Complex::op_endcreate_[0],
                            2);
    }
  else
    {
      this->internal_id_ = ::semget (this->key_, 2 + nsems, 0);
      if (this->internal_id_ == -1)
        return -1;                      // doesn't exist or tables full

      // Decrement the process counter. We don't need a lock to do this.
      if (::semop (this->internal_id_,
                         &SV_Semaphore_Complex::op_open_[0], 1) < 0)
#ifdef HAS_SYSV_IPC
        return this->init ();
#else
        return -1;
#endif

      return 0;
    }
}

int
SV_Semaphore_Complex::open (const char *name,
                                short flags,
                                int initial_value,
                                u_short nsems,
                                mode_t perms)
{
  cout << "SV_Semaphore_Complex::open" << endl;
  return this->open (SV_Semaphore_Simple::name_2_key (name),
                     flags, initial_value, nsems, perms);
}

// Close a ACE_SV_Semaphore.  Unlike the remove above, this function
// is for a process to call before it exits, when it is done with the
// ACE_SV_Semaphore.  We "decrement" the counter of processes using
// the ACE_SV_Semaphore, and if this was the last one, we can remove
// the ACE_SV_Semaphore.

int
SV_Semaphore_Complex::close (void)
{
  cout << "SV_Semaphore_Complex::close" << endl;
  int semval;
#ifdef HAS_SYSV_IPC
  if (this->key_ == (key_t) - 1 || this->internal_id_ == -1)
    return -1;
#endif

  // The following semop() first gets a lock on the ACE_SV_Semaphore,
  // then increments [1] - the process number.

  if (::semop (this->internal_id_,
                     &SV_Semaphore_Complex::op_close_[0],
                     3) == -1)
    return -1;

  // Now that we have a lock, read the value of the process counter to
  // see if this is the last reference to the ACE_SV_Semaphore. There
  // is a race condition here - see the comments in create ().

  if ((semval = SV_Semaphore_Simple::control (GETVAL, 0, 1)) == -1)
    return -1;

  if (semval > SV_Semaphore_Complex::BIGCOUNT_)
    return -1;
  else if (semval == SV_Semaphore_Complex::BIGCOUNT_)
    return this->remove ();
  else
    {
      int result = ::semop (this->internal_id_,
                                  &SV_Semaphore_Complex::op_unlock_[0], 1);
#ifdef HAS_SYSV_IPC
      this->init ();
#endif
      return result;
    }
}

SV_Semaphore_Complex::SV_Semaphore_Complex (key_t k,
                                                    short flags,
                                                    int initial_value,
                                                    u_short nsems,
                                                    mode_t perms)
{
  cout << "SV_Semaphore_Complex::SV_Semaphore_Complex" << endl;
  if (this->open (k, flags, initial_value, nsems, perms) == -1)
    cout << "error SV_Semaphore_Complex" << endl;
}

SV_Semaphore_Complex::SV_Semaphore_Complex (const char *name,
                                                    short flags,
                                                    int initial_value,
                                                    u_short nsems,
                                                    mode_t perms)
{
  cout << "SV_Semaphore_Complex::SV_Semaphore_Complex" << endl;
  key_t key = DEFAULT_SEM_KEY;
#ifdef HAS_SYSV_IPC
  if (name != 0)
    key = this->name_2_key (name);
#else
  UNUSED_ARG (name);
#endif

  if (this->open (key, flags, initial_value, nsems, perms) == -1)
    cout << "error SV_Semaphore_Complex" << endl;
}

SV_Semaphore_Complex::~SV_Semaphore_Complex (void)
{
  cout << "SV_Semaphore_Complex::~SV_Semaphore_Complex" << endl;
  if (this->internal_id_ >= 0)
    this->close ();
}

SV_Semaphore_Complex::SV_Semaphore_Complex (void)
{
  cout << "SV_Semaphore_Complex::SV_Semaphore_Complex" << endl;
#ifdef HAS_SYSV_IPC
  this->init ();
#endif
}
