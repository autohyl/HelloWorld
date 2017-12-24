// -*- C++ -*-
#include "include/os_shm.h"

inline size_t
SV_Shared_Memory::round_up (size_t len)
{
  cout << "SV_Shared_Memory::round_up" << endl;
  return (len + SV_Shared_Memory::ALIGN_WORDB - 1) & ~(SV_Shared_Memory::ALIGN_WORDB - 1);
}

// Creates a shared memory segment of SIZE bytes. Does *not* attach
// this memory segment...

inline int
SV_Shared_Memory::open (key_t external_id, size_t sz, int create, int perms)
{
  cout << "SV_Shared_Memory::open" << endl;
  this->segment_ptr_ = 0;
  this->size_ = sz;

  this->internal_id_ = ::shmget (external_id, sz, create | perms);

  return this->internal_id_ == -1 ? -1 : 0;
}

// Attachs to the shared memory segment.

inline int
SV_Shared_Memory::attach (void *virtual_addr, int flags)
{
  cout << "SV_Shared_Memory::attach" << endl;
  this->segment_ptr_ = ::shmat (this->internal_id_, virtual_addr, flags);
  return this->segment_ptr_ == (void *) -1 ? -1 : 0;
}

// Interface to the underlying shared memory control function.

inline int
SV_Shared_Memory::control (int cmd, void *buf)
{
  cout << "SV_Shared_Memory::control" << endl;
  return ::shmctl (this->internal_id_, cmd, (struct shmid_ds *) buf);
}

// The overall size of the segment.

inline size_t
SV_Shared_Memory::get_segment_size (void) const
{
  cout << "SV_Shared_Memory::get_segment_size" << endl;
  return this->size_;
}

// Removes the shared memory segment.

inline int
SV_Shared_Memory::remove (void)
{
  cout << "SV_Shared_Memory::remove" << endl;
  return ::shmctl (this->internal_id_, IPC_RMID, 0);
}

// Detach the current binding between this->segment_ptr and the shared
// memory segment.

inline int
SV_Shared_Memory::detach (void)
{
  cout << "SV_Shared_Memory::detach" << endl;
  return ::shmdt (this->segment_ptr_);
}

inline void *
SV_Shared_Memory::get_segment_ptr (void) const
{
  cout << "SV_Shared_Memory::get_segment_ptr" << endl;
  return this->segment_ptr_;
}

inline int
SV_Shared_Memory::get_id (void) const
{
  cout << "SV_Shared_Memory::get_id" << endl;
  return this->internal_id_;
}
