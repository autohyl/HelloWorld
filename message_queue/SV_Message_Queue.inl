// -*- C++ -*-

inline int
SV_Message_Queue::open (key_t external_id, int create, int perms)
{
  cout << "SV_Message_Queue::open" << endl;
  return this->internal_id_ = ::msgget (external_id, create | perms);
}

// What does it mean to close a message queue?!

inline int
SV_Message_Queue::close (void)
{
  cout << "SV_Message_Queue::close" << endl;
  this->internal_id_ = -1;
  return 1;
}

inline int
SV_Message_Queue::control (int option, void *arg)
{
  cout << "SV_Message_Queue::control" << endl;
  return ::msgctl (this->internal_id_, option,
                         (struct msqid_ds *) arg);
}

inline int
SV_Message_Queue::remove (void)
{
  cout << "SV_Message_Queue::remove" << endl;
  int const result = this->control (IPC_RMID);
  this->internal_id_ = -1;
  return result;
}

inline int
SV_Message_Queue::get_id (void) const
{
  cout << "SV_Message_Queue::get_id" << endl;
  return this->internal_id_;
}

inline void
SV_Message_Queue::set_id (int id)
{
  cout << "SV_Message_Queue::set_id" << endl;
  this->internal_id_ = id;
}

inline ssize_t
SV_Message_Queue::recv (SV_Message &mb,
                            int length,
                            long type,
                            int mflags)
{
  cout << "SV_Message_Queue::recv" << endl;
  return msgrcv (this->internal_id_, (void *) &mb,
                         length, type, mflags);
}

inline int
SV_Message_Queue::send (const SV_Message &mb,
                            int length,
                            int mflags)
{
  cout << "SV_Message_Queue::send" << endl;
  return msgsnd (this->internal_id_, (void *) &mb,
                         length, mflags);
}
