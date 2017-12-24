#include "SV_Shared_Memory.h"

void
SV_Shared_Memory::dump (void) const
{
  cout << "SV_Shared_Memory::dump" << endl;
}

// Creates a shared memory segment of SIZE bytes and *does* attach to
// this segment.

int
SV_Shared_Memory::open_and_attach (key_t external_id,
                                       size_t sz,
                                       int create,
                                       int perms,
                                       void *virtual_addr,
                                       int flags)
{
  cout << "SV_Shared_Memory::open_and_attach" << endl;
  if (this->open (external_id, sz, create, perms) == -1)
    return -1;
  else if (this->attach (virtual_addr, flags) == -1)
    return -1;
  else
    return 0;
}

// Constructor interface to this->open_and_attach () member function.

SV_Shared_Memory::SV_Shared_Memory (key_t external_id,
                                            size_t sz,
                                            int create,
                                            int perms,
                                            void *virtual_addr,
                                            int flags)
{
  cout << "SV_Shared_Memory::SV_Shared_Memory" << endl;
  if (this->open_and_attach (external_id, sz, create,
                             perms, virtual_addr, flags) == -1)
   cout << "SV_Shared_Memory::SV_Shared_Memory" << endl;
}

// The "do nothing" constructor.

SV_Shared_Memory::SV_Shared_Memory (void)
  : internal_id_ (0),
    size_ (0),
    segment_ptr_ (0)
{
  cout << "SV_Shared_Memory::SV_Shared_Memory" << endl;
}

// Added this constructor to accept an internal id, the one generated
// when a server constructs with the key IPC_PRIVATE.  The client can
// be passed SV_Shared_Memory::internal_id via a socket and call
// this construtor to attach the existing segment.  This prevents
// having to hard-code a key in advance.  Courtesy of Marvin Wolfthal
// (maw@fsg.com).

SV_Shared_Memory::SV_Shared_Memory (int int_id,
                                            int flags)
  : internal_id_ (int_id),
    size_ (0)
{
  cout << "SV_Shared_Memory::SV_Shared_Memory" << endl;
  if (this->attach (0, flags) == -1)
    cout << "SV_Shared_Memory::SV_Shared_Memory" << endl;
}
