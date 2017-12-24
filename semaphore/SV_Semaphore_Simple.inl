// -*- C++ -*-

inline int
SV_Semaphore_Simple::control (int cmd,
                                  semun arg,
                                  u_short n) const
{
  cout << "SV_Semaphore_Simple::control" << endl;
  return this->internal_id_ == -1 ?
    -1 : ACE_OS::semctl (this->internal_id_, n, cmd, arg);
}

// Close a SV_Semaphore, marking it as invalid for subsequent
// operations...

inline int
SV_Semaphore_Simple::close (void)
{
  cout << "SV_Semaphore_Simple::close" << endl;
  return -1;
}

// General SV_Semaphore operation on an array of SV_Semaphores.

inline int
SV_Semaphore_Simple::op (sembuf op_vec[], u_short n) const
{
  cout << "SV_Semaphore_Simple::op" << endl;
  return this->internal_id_ == -1
    ? -1 : ::semop (this->internal_id_, op_vec, n);
}

// Wait until a SV_Semaphore's value is greater than 0, the
// decrement it by 1 and return. Dijkstra's P operation, Tannenbaums
// DOWN operation.

inline int
SV_Semaphore_Simple::acquire (u_short n, short flags) const
{
  cout << "SV_Semaphore_Simple::acquire" << endl;
  return this->op (-1, n, flags);
}

inline int
SV_Semaphore_Simple::acquire_read (u_short n, short flags) const
{
  cout << "SV_Semaphore_Simple::acquire_read" << endl;
  return this->acquire (n, flags);
}

inline int
SV_Semaphore_Simple::acquire_write (u_short n, short flags) const
{
  cout << "SV_Semaphore_Simple::acquire_write" << endl;
  return this->acquire (n, flags);
}

// Non-blocking version of acquire().

inline int
SV_Semaphore_Simple::tryacquire (u_short n, short flags) const
{
  cout << "SV_Semaphore_Simple::tryacquire" << endl;
  return this->op (-1, n, flags | IPC_NOWAIT);
}

// Non-blocking version of acquire().

inline int
SV_Semaphore_Simple::tryacquire_read (u_short n, short flags) const
{
  cout << "SV_Semaphore_Simple::tryacquire_read" << endl;
  return this->tryacquire (n, flags);
}

// Non-blocking version of acquire().

inline int
SV_Semaphore_Simple::tryacquire_write (u_short n, short flags) const
{
  cout << "SV_Semaphore_Simple::tryacquire_write" << endl;
  return this->tryacquire (n, flags);
}

// Increment SV_Semaphore by one. Dijkstra's V operation,
// Tannenbaums UP operation.

inline int
SV_Semaphore_Simple::release (u_short n, short flags) const
{
  cout << "SV_Semaphore_Simple::release" << endl;
  return this->op (1, n, flags);
}

inline int
SV_Semaphore_Simple::get_id (void) const
{
  cout << "SV_Semaphore_Simple::get_id" << endl;
  return this->internal_id_;
}
