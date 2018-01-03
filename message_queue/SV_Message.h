// -*- C++ -*-

//==========================================================================
/**
 *  @file    SV_Message.h
 *
 *  @author Doug Schmidt
 */
//==========================================================================


#ifndef SV_MESSAGE_H
#define SV_MESSAGE_H

/**
 * @class SV_Message
 *
 * @brief Defines the header file for the C++ wrapper for message queues.
 */
class SV_Message
{
public:
  /// Constructor
  SV_Message (long type = 0);

  /// Destructor
  ~SV_Message (void);

  /// Get the message type.
  long type (void) const;

  /// Set the message type.
  void type (long);

  /// Dump the state of an object.
  void dump (void) const;

protected:
  /// Type of the message.
  long type_;
};

#include "SV_Message.inl"

#endif /* SV_MESSAGE_H */
