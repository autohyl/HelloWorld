#include "../shared_memory/Shared_Memory_SV.h"
#include "SV_Semaphore_Simple.h"
#include "SV_Semaphore_Complex.h"
#include <unistd.h>

#if defined (HAS_SYSV_IPC)

#define SHM_KEY (key_t) 5678
#define SHMSZ 27

static Shared_Memory_SV alloc(SHM_KEY,
                                   SHMSZ,
                                   Shared_Memory_SV::CREATE);

const int SEM_KEY = DEFAULT_SEM_KEY + 1;

static int
parent (char *shm)
{
  char *s = shm;

  SV_Semaphore_Complex sem (SEM_KEY, SV_Semaphore_Complex::CREATE, 0, 2);

  for (char c = 'a'; c <= 'z'; c++)
    *s++ = c;

  *s = '\0';

  if (sem.release (0) == -1)
    cout << "error parent sem.release(0)" << endl;
  else if (sem.acquire (1) == -1)
    cout << "error parent sem.acquire(1)" << endl;

  if (alloc.remove () == -1)
    cout << "error alloc.remove" << endl;
  if (sem.remove () == -1)
    cout << "error sem.remove" << endl;
  return 0;
}

static int
child (char *shm)
{
  SV_Semaphore_Complex sem (SEM_KEY, SV_Semaphore_Complex::CREATE, 0, 2);

#ifndef TEST_SEM
  while (sem.tryacquire (0) == -1) {
#else
  if (sem.tryacquire (0) == -1) {
#endif
    if (errno == EAGAIN)
      cout << "debug spinning in client!" << endl;
    else
      cout << "error client mutex.tryacquire(0)" << endl;
  }
#ifdef TEST_SEM
  else {
    cout << "error sem.tryacquire (0) == -1" << endl;
  }
#endif

  for (char *s = (char *) shm; *s != '\0'; s++)
    cout << "debug " << *s << " ";

  cout << endl;

  if (sem.release (1) < 0)
    cout << "error client sem.release(1)" << endl;
  return 0;
}

int
main (int, char *[])
{
  char *shm = (char *) alloc.malloc (27);

  switch (::fork ())
    {
    case -1:
      cout << "error fork failed" << endl;
      return 0;
    case 0:
      return child (shm);
    default:
      return parent (shm);
    }
}
#else
int main (int, char *[])
{
  cout << "error SYSV IPC, or SYSV SHMEM is not supported on this platform" << endl;
  return 0;
}
#endif /* ACE_HAS_SYSV_IPC */

