#include "Shared_Memory_SV.h"

void
Shared_Memory_SV::dump (void) const
{
  cout << "Shared_Memory_SV::dump" << endl;
}

Shared_Memory_SV::Shared_Memory_SV (key_t id,
                                            size_t length,
                                            int create,
                                            int perms,
                                            void *addr,
                                            int flags)
  : shared_memory_ (id, length, create, perms, addr, flags)
{
  cout << "Shared_Memory_SV::Shared_Memory_SV" << endl;
}

// The overall size of the segment.

size_t
Shared_Memory_SV::get_segment_size (void) const
{
  cout << "Shared_Memory_SV::get_segment_size" << endl;
  // This cast is ok since the 'open' method for this class allows only
  // an 'int' size. Therefore, this case should not lose information.
  return this->shared_memory_.get_segment_size ();
}

// Removes the shared memory segment.

int
Shared_Memory_SV::remove (void)
{
  cout << "Shared_Memory_SV::remove" << endl;
  return shared_memory_.remove ();
}

// Closes (detaches) the shared memory segment.

int
Shared_Memory_SV::close (void)
{
  cout << "Shared_Memory_SV::close" << endl;
  return shared_memory_.detach ();
}

void *
Shared_Memory_SV::malloc (size_t)
{
  cout << "Shared_Memory_SV::malloc" << endl;
  return this->shared_memory_.get_segment_ptr ();
}

int
Shared_Memory_SV::get_id (void) const
{
  cout << "Shared_Memory_SV::get_id" << endl;
  return this->shared_memory_.get_id ();
}

int
Shared_Memory_SV::free (void *p)
{
  cout << "Shared_Memory_SV::free" << endl;
  return p != 0;
}
