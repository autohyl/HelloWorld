#include "SV_Message_Queue.h"

void
SV_Message_Queue::dump (void) const
{
  cout << "SV_Message_Queue::dump" << endl;
}

SV_Message_Queue::SV_Message_Queue (void) :
  internal_id_ (-1)
{
  cout << "SV_Message_Queue::SV_Message_Queue" << endl;
}

SV_Message_Queue::~SV_Message_Queue (void)
{
  cout << "SV_Message_Queue::~SV_Message_Queue" << endl;
}

SV_Message_Queue::SV_Message_Queue (key_t external_id,
                                            int create,
                                            int perms)
{
  cout << "SV_Message_Queue::SV_Message_Queue" << endl;
  if (this->open (external_id, create, perms) == -1)
    cout << "error SV_Message_Queue::SV_Message_Queue" << endl;
}
