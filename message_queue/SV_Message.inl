// -*- C++ -*-

inline
SV_Message::SV_Message (long t)
  : type_ (t)
{
  cout << "SV_Message::SV_Message" << endl;
}

inline
SV_Message::~SV_Message (void)
{
  cout << "SV_Message::~SV_Message" << endl;
}

inline long
SV_Message::type (void) const
{
  cout << "SV_Message::type" << endl;
  return this->type_;
}

inline void
SV_Message::type (long t)
{
  cout << "SV_Message::type" << endl;
  this->type_ = t;
}
