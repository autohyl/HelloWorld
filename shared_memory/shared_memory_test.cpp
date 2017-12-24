#include "Shared_Memory_SV.h"
#include <stdio.h>
#include <unistd.h>

#define SHMSZ 27
#define SHM_KEY (key_t) 5678

static void
client (void)
{
  Shared_Memory_SV shm_client (SHM_KEY, SHMSZ);
  char *shm = (char *) shm_client.malloc ();

  for (char *s = shm; *s != '\0'; s++)
    putchar (*s);

  putchar ('\n');
  *shm = '*';
}

static void
server (void)
{
  Shared_Memory_SV shm_server (SHM_KEY,
                                   SHMSZ,
                                   Shared_Memory_SV::CREATE);
  char *shm = (char *) shm_server.malloc ();
  char *s   = shm;

  for (char c = 'a'; c <= 'z'; c++)
    *s++ = c;

  *s = '\0';

  while (*shm != '*')
    ::sleep (1);

  if (shm_server.remove () < 0)
    cout << "remove" << endl;
}

int
main (int, char *[])
{
  switch (::fork ())
    {
    case -1:
      cout << "fork error" << endl;
      return 1;
    case 0:
      // Make sure the server starts up first.
      ::sleep (1);
      client ();
      break;
    default:
      server ();
      break;
    }

  return 0;
}

