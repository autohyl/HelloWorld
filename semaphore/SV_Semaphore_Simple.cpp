#include "SV_Semaphore_Simple.h"

void
SV_Semaphore_Simple::dump (void) const
{
  cout << "SV_Semaphore_Simple::dump" << endl;
}

int
SV_Semaphore_Simple::control (int cmd,
                                  int value,
                                  u_short semnum) const
{
  cout << "SV_Semaphore_Simple::control" << endl;
  if (this->internal_id_ == -1)
    return -1;
  else
    {
      semun semctl_arg;

      semctl_arg.val = value;
      return ::semctl (this->internal_id_,
                             semnum,
                             cmd,
                             semctl_arg);
    }
}

#ifdef HAS_SYSV_IPC
int
SV_Semaphore_Simple::init (key_t k, int i)
{
  cout << "SV_Semaphore_Simple::init" << endl;
  this->key_ = k;
  this->internal_id_ = i;
  return 0;
}
#endif

// General SV_Semaphore operation. Increment or decrement by a
// specific amount (positive or negative; amount can`t be zero).

int
SV_Semaphore_Simple::op (short val, u_short n, short flags) const
{
  cout << "SV_Semaphore_Simple::op" << endl;
  sembuf op_op;

  op_op.sem_num = n;
  op_op.sem_flg = flags;

  if (this->internal_id_ == -1)
    return -1;
  else if ((op_op.sem_op = val) == 0)
    return -1;
  else
    return semop (this->internal_id_, &op_op, 1);
}

// Open or create one or more SV_Semaphores.  We return 0 if all is
// OK, else -1.

int
SV_Semaphore_Simple::open (key_t k,
                               short flags,
                               int initial_value,
                               u_short n,
                               mode_t perms)
{
  cout << "SV_Semaphore_Simple::open" << endl;
  union semun ivalue;

#ifdef HAS_SYSV_IPC
  if (k == IPC_PRIVATE || k == static_cast<key_t> (INVALID_SEM_KEY))
    return -1;
#endif

  ivalue.val = initial_value;
  this->key_ = k;
  this->sem_number_ = n;

  this->internal_id_ = ::semget (this->key_, n, perms | flags);

  if (this->internal_id_ == -1)
    return -1;

  if (BIT_ENABLED (flags, IPC_CREAT))
    for (int i = 0; i < n; i++)
      if (::semctl (this->internal_id_, i, SETVAL, ivalue) == -1)
        return -1;

  return 0;
}

SV_Semaphore_Simple::SV_Semaphore_Simple (key_t k,
                                                  short flags,
                                                  int initial_value,
                                                  u_short n,
                                                  mode_t perms)
  : key_ (k)
{
  cout << "SV_Semaphore_Simple::SV_Semaphore_Simple" << endl;
  if (this->open (k, flags, initial_value, n, perms) == -1)
    cout << "error SV_Semaphore::SV_Semaphore" << endl;
}

// Convert name to key.  This function is used internally to create keys
// for the semaphores.
//
// The method for generating names is a 32 bit CRC, but still we
// measured close to collition ratio of nearly 0.1% for
// ACE::unique_name()-like strings.

key_t
SV_Semaphore_Simple::name_2_key (const char *name)
{
  cout << "SV_Semaphore_Simple::name_2_key" << endl;

  if (name == 0)
    {
#ifdef HAS_SYSV_IPC
      return static_cast<key_t> (INVALID_SEM_KEY);
#else
      key_t ret = DEFAULT_SEM_KEY;
      return ret;
#endif
    }

#ifdef HAS_SYSV_IPC
  return (key_t) crc32 (name);
#else
  key_t ret = DEFAULT_SEM_KEY;
  return ret;
#endif
}

// Open or create a SV_Semaphore.  We return 1 if all is OK, else
// 0.

int
SV_Semaphore_Simple::open (const char *name,
                               short flags,
                               int initial_value,
                               u_short n,
                               mode_t perms)
{
  cout << "SV_Semaphore_Simple::open" << endl;

  key_t key = DEFAULT_SEM_KEY;

#ifdef HAS_SYSV_IPC
  if (name != 0)
    key = this->name_2_key (name);
#else
  UNUSED_ARG (name);
#endif

  return this->open (key, flags, initial_value, n, perms);
}

SV_Semaphore_Simple::SV_Semaphore_Simple (const char *name,
                                                  short flags,
                                                  int initial_value,
                                                  u_short n,
                                                  mode_t perms)
{
  cout << "SV_Semaphore_Simple::SV_Semaphore_Simple" << endl;
  if (this->open (name,
                  flags,
                  initial_value,
                  n,
                  perms) == -1)
    cout << "error SV_Semaphore_Simple::SV_Semaphore_Simple" << endl;
}

SV_Semaphore_Simple::~SV_Semaphore_Simple (void)
{
  cout << "SV_Semaphore_Simple::~SV_Semaphore_Simple" << endl;
  this->close ();
}

SV_Semaphore_Simple::SV_Semaphore_Simple (void) :
  sem_number_ (0)
{
  cout << "SV_Semaphore_Simple::SV_Semaphore_Simple" << endl;
#ifdef HAS_SYSV_IPC
  this->init ();
#endif
}

// Remove all SV_Semaphores associated with a particular key.  This
// call is intended to be called from a server, for example, when it
// is being shut down, as we do an IPC_RMID on the SV_Semaphore,
// regardless of whether other processes may be using it or not.  Most
// other processes should use close() below.

int
SV_Semaphore_Simple::remove (void) const
{
  cout << "SV_Semaphore_Simple::remove" << endl;
  int const result = this->control (IPC_RMID);
#ifdef HAS_SYSV_IPC
  ((SV_Semaphore_Simple *) this)->init ();
#endif
  return result;
}
