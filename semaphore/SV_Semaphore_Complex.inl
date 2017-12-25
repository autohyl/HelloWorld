// -*- C++ -*-

inline int
SV_Semaphore_Complex::acquire (u_short n, short flags) const
{
  cout <<"SV_Semaphore_Complex::acquire" << endl;
  return SV_Semaphore_Simple::acquire ((u_short) n + 2, flags);
}

inline int
SV_Semaphore_Complex::acquire_read (u_short n, short flags) const
{
  cout <<"SV_Semaphore_Complex::acquire_read" << endl;
  return this->acquire (n, flags);
}

inline int
SV_Semaphore_Complex::acquire_write (u_short n, short flags) const
{
  cout <<"SV_Semaphore_Complex::acquire_write" << endl;
  return this->acquire (n, flags);
}

inline int
SV_Semaphore_Complex::tryacquire (u_short n, short flags) const
{
  cout <<"SV_Semaphore_Complex::tryacquire" << endl;
  return SV_Semaphore_Simple::tryacquire ((u_short) n + 2, flags);
}

inline int
SV_Semaphore_Complex::tryacquire_read (u_short n, short flags) const
{
  cout <<"SV_Semaphore_Complex::tryacquire_read" << endl;
  return this->tryacquire (n, flags);
}

inline int
SV_Semaphore_Complex::tryacquire_write (u_short n, short flags) const
{
  cout <<"SV_Semaphore_Complex::tryacquire_write" << endl;
  return this->tryacquire (n, flags);
}

inline int
SV_Semaphore_Complex::release (u_short n, short flags) const
{
  cout <<"SV_Semaphore_Complex::release" << endl;
  return SV_Semaphore_Simple::release ((u_short) n + 2, flags);
}

inline int
SV_Semaphore_Complex::op (short val, u_short n, short flags) const
{
  cout <<"SV_Semaphore_Complex::op" << endl;
  return SV_Semaphore_Simple::op (val, (u_short) n + 2, flags);
}

inline int
SV_Semaphore_Complex::op (sembuf op_vec[], u_short n) const
{
  cout <<"SV_Semaphore_Complex::op" << endl;
  return SV_Semaphore_Simple::op (op_vec, (u_short) n + 2);
}

inline int
SV_Semaphore_Complex::control (int cmd, semun arg, u_short n) const
{
  cout <<"SV_Semaphore_Complex::control" << endl;
  return SV_Semaphore_Simple::control (cmd, arg, (u_short) n + 2);
}

inline int
SV_Semaphore_Complex::control (int cmd, int value, u_short n) const
{
  cout <<"SV_Semaphore_Complex::control" << endl;
  return SV_Semaphore_Simple::control (cmd, value, (u_short) n + 2);
}
