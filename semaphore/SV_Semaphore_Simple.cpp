#include "SV_Semaphore_Simple.h"
#include "include/os_sem.h"

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
  ACE_TRACE ("SV_Semaphore_Simple::control");
  if (this->internal_id_ == -1)
    return -1;
  else
    {
      semun semctl_arg;

      semctl_arg.val = value;
      return ACE_OS::semctl (this->internal_id_,
                             semnum,
                             cmd,
                             semctl_arg);
    }
}

// General ACE_SV_Semaphore operation. Increment or decrement by a
// specific amount (positive or negative; amount can`t be zero).

int
SV_Semaphore_Simple::op (short val, u_short n, short flags) const
{
  ACE_TRACE ("SV_Semaphore_Simple::op");
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
  ACE_TRACE ("SV_Semaphore_Simple::open");
  union semun ivalue;

#ifdef ACE_HAS_SYSV_IPC
  if (k == IPC_PRIVATE || k == static_cast<key_t> (ACE_INVALID_SEM_KEY))
    return -1;
#endif

  ivalue.val = initial_value;
  this->key_ = k;
  this->sem_number_ = n;

  this->internal_id_ = ACE_OS::semget (this->key_, n, perms | flags);

  if (this->internal_id_ == -1)
    return -1;

  if (ACE_BIT_ENABLED (flags, IPC_CREAT))
    for (int i = 0; i < n; i++)
      if (ACE_OS::semctl (this->internal_id_, i, SETVAL, ivalue) == -1)
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
  ACE_TRACE ("SV_Semaphore_Simple::SV_Semaphore_Simple");
  if (this->open (k, flags, initial_value, n, perms) == -1)
    ACELIB_ERROR ((LM_ERROR,  ACE_TEXT ("%p\n"),  ACE_TEXT ("ACE_SV_Semaphore::ACE_SV_Semaphore")));
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
  ACE_TRACE ("SV_Semaphore_Simple::name_2_key");

  if (name == 0)
    {
      errno = EINVAL;
#ifdef ACE_HAS_SYSV_IPC
      return static_cast<key_t> (ACE_INVALID_SEM_KEY);
#else
      key_t ret = ACE_DEFAULT_SEM_KEY;
      return ret;
#endif
    }

  // Basically "hash" the values in the <name>.  This won't
  // necessarily guarantee uniqueness of all keys.
  // But (IMHO) CRC32 is good enough for most purposes (Carlos)
#if defined (ACE_WIN32) && defined (_MSC_VER)
  // The cast below is legit...
#  pragma warning(push)
#  pragma warning(disable : 4312)
#endif /* defined (ACE_WIN32) && defined (_MSC_VER) */

#ifdef ACE_HAS_SYSV_IPC
  return (key_t) ACE::crc32 (name);
#else
  key_t ret = ACE_DEFAULT_SEM_KEY;
  return ret;
#endif

#if defined (ACE_WIN32) && defined (_MSC_VER)
#  pragma warning(pop)
#endif /* defined (ACE_WIN32) && defined (_MSC_VER) */
}

// Open or create a ACE_SV_Semaphore.  We return 1 if all is OK, else
// 0.

int
SV_Semaphore_Simple::open (const char *name,
                               short flags,
                               int initial_value,
                               u_short n,
                               mode_t perms)
{
  ACE_TRACE ("SV_Semaphore_Simple::open");

  key_t key = ACE_DEFAULT_SEM_KEY;

#ifdef ACE_HAS_SYSV_IPC
  if (name != 0)
    key = this->name_2_key (name);
#else
  ACE_UNUSED_ARG (name);
#endif

  return this->open (key, flags, initial_value, n, perms);
}

SV_Semaphore_Simple::SV_Semaphore_Simple (const char *name,
                                                  short flags,
                                                  int initial_value,
                                                  u_short n,
                                                  mode_t perms)
{
  ACE_TRACE ("SV_Semaphore_Simple::SV_Semaphore_Simple");
  if (this->open (name,
                  flags,
                  initial_value,
                  n,
                  perms) == -1)
    ACELIB_ERROR ((LM_ERROR,
                ACE_TEXT ("%p\n"),
                ACE_TEXT ("SV_Semaphore_Simple::SV_Semaphore_Simple")));
}

#if defined (ACE_HAS_WCHAR)
SV_Semaphore_Simple::SV_Semaphore_Simple (const wchar_t *name,
                                                  short flags,
                                                  int initial_value,
                                                  u_short nsems,
                                                  mode_t perms)
{
  ACE_TRACE ("SV_Semaphore_Simple::SV_Semaphore_Simple(wchar_t)");
  if (this->open (ACE_Wide_To_Ascii (name).char_rep (),
                  flags,
                  initial_value,
                  nsems,
                  perms) == -1)
    ACELIB_ERROR ((LM_ERROR,
                ACE_TEXT ("%p\n"),
                ACE_TEXT ("SV_Semaphore_Simple::SV_Semaphore_Simple")));
}
#endif /* ACE_HAS_WCHAR */

SV_Semaphore_Simple::~SV_Semaphore_Simple (void)
{
  ACE_TRACE ("SV_Semaphore_Simple::~SV_Semaphore_Simple");
  this->close ();
}

SV_Semaphore_Simple::SV_Semaphore_Simple (void) :
  sem_number_ (0)
{
  ACE_TRACE ("SV_Semaphore_Simple::SV_Semaphore_Simple");
#ifdef ACE_HAS_SYSV_IPC
  this->init ();
#endif
}

// Remove all SV_Semaphores associated with a particular key.  This
// call is intended to be called from a server, for example, when it
// is being shut down, as we do an IPC_RMID on the ACE_SV_Semaphore,
// regardless of whether other processes may be using it or not.  Most
// other processes should use close() below.

int
SV_Semaphore_Simple::remove (void) const
{
  ACE_TRACE ("SV_Semaphore_Simple::remove");
  int const result = this->control (IPC_RMID);
#ifdef ACE_HAS_SYSV_IPC
  ((SV_Semaphore_Simple *) this)->init ();
#endif
  return result;
}
