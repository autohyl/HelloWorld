#include "SV_Message_Queue.h"
#include "include/Msg_Data.h"
#include <stdlib.h>

int
main (int, char *[])
{
  long pid = long (::getpid ());
  SV_Message_Queue msgque (SRV_KEY);
  Message_Block send_msg (SRV_ID,
                          pid,
                          cuserid (static_cast<char *> (0)),
                          "did you get this?");
  Message_Block recv_msg (pid);

  if (msgque.send (send_msg, send_msg.length ()) < 0) {
    cout << "error msgque.send" << endl;
    ::exit (1);
  }

  if (msgque.recv (recv_msg, sizeof (Message_Data), recv_msg.type ()) < 0) {
    cout << "msgrcv" << endl;
    ::exit (1);
  }

  cout << "a message of length "
       << recv_msg.length ()
       << " received from server "
       << recv_msg.pid ()
       << " (user "
       << recv_msg.user () << "): "
       << recv_msg.text () << "\n";

  return 0;
}

