#include <Timer.h>
#include "NovelHop.h"

configuration NovelHopAppC {
}
implementation {
  components MainC;
  components LedsC;
  components NovelHopC as App;
  components new TimerMilliC() as Timer0;
  components ActiveMessageC;
  components new AMSenderC(AM_NOVELHOP);
  components new AMReceiverC(AM_NOVELHOP);

  App.Boot -> MainC;
  App.Leds -> LedsC;
  App.Timer0 -> Timer0;
  App.Packet -> AMSenderC;
  App.AMPacket -> AMSenderC;
  App.AMControl -> ActiveMessageC;
  App.AMSend -> AMSenderC;
  App.Receive -> AMReceiverC;
}
