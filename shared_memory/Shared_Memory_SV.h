// -*- C++ -*-

//=============================================================================
/**
 *  @file    Shared_Memory_SV.h
 *
 *  @author Douglas C. Schmidt <schmidt@cs.wustl.edu>
 */
//=============================================================================


#ifndef IPC_SHARED_MALLOC_SV_H
#define IPC_SHARED_MALLOC_SV_H

#include "Shared_Memory.h"

#include "SV_Shared_Memory.h"

class Shared_Memory_SV : public Shared_Memory
{
public:
  enum
    {
      CREATE = IPC_CREAT,
      OPEN = 0
    };

  // = Initialization and termination methods.
  Shared_Memory_SV (void);
  Shared_Memory_SV (key_t id,
                        size_t length,
                        int create = Shared_Memory_SV::OPEN,
                        int perms = DEFAULT_FILE_PERMS,
                        void *addr = 0,
                        int flags = 0);

  int open (key_t id,
            size_t length,
            int create = Shared_Memory_SV::OPEN,
            int perms = DEFAULT_FILE_PERMS,
            void *addr = 0,
            int flags = 0);

  /// Close down the shared memory segment.
  virtual int close (void);

  /// Remove the underlying shared memory segment.
  virtual int remove (void);

  // = Allocation and deallocation methods.
  /// Create a new chuck of memory containing @a size bytes.
  virtual void *malloc (size_t = 0);

  /// Free a chuck of memory allocated by <Shared_Memory_SV::malloc>.
  virtual int free (void *p);

  /// Return the size of the shared memory segment.
  virtual size_t get_segment_size (void) const;

  /// Return the ID of the shared memory segment (i.e., a System V
  /// shared memory internal id).
  virtual int get_id (void) const;

  /// Dump the state of an object.
  void dump (void) const;

private:
   /// This version is implemented with System V shared memory
   /// segments.
   SV_Shared_Memory shared_memory_;
};

#include "Shared_Memory_SV.inl"

#endif /* IPC_SHARED_MALLOC_SV_H */
