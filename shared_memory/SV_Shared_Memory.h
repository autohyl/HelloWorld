// -*- C++ -*-

//==========================================================================
/**
 *  @file    SV_Shared_Memory.h
 *
 *  @author Douglas C. Schmidt <schmidt@cs.wustl.edu>
 */
//==========================================================================

#ifndef SV_Shared_Memory_H
#define SV_Shared_Memory_H

#include "include/os_shm.h"

/**
 * @class SV_Shared_Memory
 *
 * @brief This is a wrapper for System V shared memory.
 */
class SV_Shared_Memory
{
public:
  enum
  {
    CREATE = IPC_CREAT,
    OPEN   = 0
  };

  // = Initialization and termination methods.
  SV_Shared_Memory (void);
  SV_Shared_Memory (key_t external_id,
                        size_t size,
                        int create,
                        int perms = DEFAULT_FILE_PERMS,
                        void *virtual_addr = 0,
                        int flags = 0);

  SV_Shared_Memory (int internal_id,
                        int flags = 0);

  int  open (key_t external_id,
             size_t size,
             int create = SV_Shared_Memory::OPEN,
             int perms = DEFAULT_FILE_PERMS);

  int  open_and_attach (key_t external_id,
                        size_t size,
                        int create = SV_Shared_Memory::OPEN,
                        int perms = DEFAULT_FILE_PERMS,
                        void *virtual_addr = 0,
                        int flags = 0);

  /// Attach this shared memory segment.
  int  attach (void *virtual_addr = 0, int flags = 0);

  /// Detach this shared memory segment.
  int  detach (void);

  /// Remove this shared memory segment.
  int  remove (void);

  /// Forward to underlying System V <shmctl>.
  int  control (int cmd, void *buf);

  // = Segment-related info.
  void *get_segment_ptr (void) const;
  size_t get_segment_size (void) const;

  /// Return the ID of the shared memory segment (i.e., an ACE_HANDLE).
  int get_id (void) const;

  /// Dump the state of an object.
  void dump (void) const;

protected:
  enum
  {
    /// Most restrictive alignment.
    ALIGN_WORDB = 8
  };

  /// Internal identifier.
  int internal_id_;

  /// Size of the mapped segment.
  size_t size_;

  /// Pointer to the beginning of the segment.
  void *segment_ptr_;

  /// Round up to an appropriate page size.
  size_t round_up (size_t len);
};

#include "SV_Shared_Memory.inl"


#endif /* SV_Shared_Memory_H */
