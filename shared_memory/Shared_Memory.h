// -*- C++ -*-

//==========================================================================
/**
 *  @file    Shared_Memory.h
 *
 *  @author Doug Schmidt
 */
//==========================================================================


#ifndef IPC_SHARED_MEMORY_H
#define IPC_SHARED_MEMORY_H

#include <stdlib.h>

class Shared_Memory
{
public:
  virtual ~Shared_Memory (void);

  // = Note that all the following methods are pure virtual.
  virtual int close (void) = 0;
  virtual int remove (void) = 0;
  virtual void *malloc (size_t = 0) = 0;
  virtual int free (void *p) = 0;
  virtual size_t get_segment_size (void) const = 0;
  virtual int get_id (void) const = 0;
};

#endif /* IPC_SHARED_MEMORY_H */
