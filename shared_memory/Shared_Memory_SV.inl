// -*- C++ -*-

inline int
Shared_Memory_SV::open (key_t id,
                            size_t length,
                            int create,
                            int perms,
                            void *addr,
                            int flags)
{
  cout << "Shared_Memory_SV::open" << endl;;
  return shared_memory_.open_and_attach (id, length, create,
                                         perms, addr, flags);
}

// The "do-nothing" constructor.

inline
Shared_Memory_SV::Shared_Memory_SV (void)
{
  cout << "Shared_Memory_SV::Shared_Memory_SV" << endl;;
}

