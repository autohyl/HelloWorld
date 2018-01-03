#include "SV_Message_Queue.h"
#include "include/Msg_Data.h"
#include <stdlib.h>
#include <signal.h>

// Must be global for signal Message...
static SV_Message_Queue sv_message_queue (SRV_KEY,
                                                  SV_Message_Queue::CREATE);

extern "C" void
handler (int)
{
  if (sv_message_queue.remove () < 0) {
    cout << "error sv_message_queue.close" << endl;
    ::exit (1);
  }
  ::exit (0);
}

int
main (int, char *[])
{
  long pid = long (::getpid ());
  Message_Block recv_msg (SRV_ID);
  Message_Block send_msg (0,
                          pid,
                          cuserid (static_cast<char *> (0)),
                          "I received your message.");

  // Register a signal handler.
  signal(SIGINT, handler);

  for (;;)
    {
      if (sv_message_queue.recv (recv_msg,
                                     sizeof (Message_Data),
                                     recv_msg.type ()) == -1) 
      {
         cout << "error sv_message_queue.recv" << endl;
         ::exit (1);
      }

      cout << "a msg of length "
           << recv_msg.length ()
           << " sent from client "
           << recv_msg.pid ()
           << " (user "
           << recv_msg.user () << "): "
           << recv_msg.text () << "\n";

      cout.flush ();

      send_msg.type (recv_msg.pid ());

      if (sv_message_queue.send (send_msg,
                                     send_msg.length ()) == -1)
      {
        cout << "error sv_message_queue.send" << endl;
        ::exit (1);
      }
    }

  return 0;
}
