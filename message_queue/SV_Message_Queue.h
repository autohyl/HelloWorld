// -*- C++ -*-

//==========================================================================
/**
 *  @file    SV_Message_Queue.h
 *
 *  @author Douglas C. Schmidt <schmidt@uci.edu>
 */
//===========================================================================

#ifndef SV_MESSAGE_QUEUE_H
#define SV_MESSAGE_QUEUE_H

#include "SV_Message.h"

/**
 * @class SV_Message_Queue
 *
 * @brief Defines the header file for the C++ wrapper for System V IPC
 * message queues.
 */
class SV_Message_Queue
{
public:
  // = Useful symbolic constants.
  enum
  {
    CREATE = IPC_CREAT,
    OPEN   = 0,
    NOWAIT = IPC_NOWAIT
  };

  // = Initialization and termination methods.
  /// Open a message queue using the <external_id>.
  SV_Message_Queue (void);
  SV_Message_Queue (key_t external_id,
                        int create = SV_Message_Queue::OPEN,
                        int perms = DEFAULT_FILE_PERMS);
  int open (key_t external_id,
            int create = SV_Message_Queue::OPEN,
            int perms = DEFAULT_FILE_PERMS);

  ~SV_Message_Queue (void);

  /// Close down this instance of the message queue without removing it
  /// from the system.
  int close (void);

  /// Close down and remove the message queue from the system.
  int remove (void);

  // = Message transfer methods.
  ssize_t recv (SV_Message &mb,
                int length,
                long mtype = 0,
                int mflags = 0);

  int send (const SV_Message &mb,
            int length,
            int mflags = 0);

  /// Access the underlying control operations.
  int control (int option, void *arg = 0);

  /// Get the underly internal id.
  int get_id (void) const;

  /// Set the underly internal id.
  void set_id (int);

  /// Dump the state of an object.
  void dump (void) const;

protected:
  /// Returned from the underlying <msgget> system call.
  int internal_id_;
};

#include "SV_Message_Queue.inl"

#endif /* SV_MESSAGE_QUEUE_H */
