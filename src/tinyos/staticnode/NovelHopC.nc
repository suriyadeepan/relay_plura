#include <Timer.h>
#include "NovelHop.h"

module NovelHopC {
  uses interface Boot;
  uses interface Leds;
  uses interface Timer<TMilli> as Timer0;
  uses interface Packet;
  uses interface AMPacket;
  uses interface AMSend;
  uses interface Receive;
  uses interface SplitControl as AMControl;
}
implementation {

  uint16_t counter;
  message_t pkt;
  bool busy = FALSE;

  void setLeds(uint16_t val) {
    if (val & 0x01)
      call Leds.led0On();
    else 
      call Leds.led0Off();
    if (val & 0x02)
      call Leds.led1On();
    else
      call Leds.led1Off();
    if (val & 0x04)
      call Leds.led2On();
    else
      call Leds.led2Off();
  }

  event void Boot.booted() {
    call AMControl.start();
  }

  event void AMControl.startDone(error_t err) {
    if (err == SUCCESS) {
      call Timer0.startPeriodic(TIMER_PERIOD_MILLI);
    }
    else {
      call AMControl.start();
    }
  }

  event void AMControl.stopDone(error_t err) {
  }

  event void Timer0.fired() {
    counter++;
    if (!busy && TOS_NODE_ID == 1) {
      NovelHopMsg* btrpkt = 
	(NovelHopMsg*)(call Packet.getPayload(&pkt, sizeof(NovelHopMsg)));
      if (btrpkt == NULL) {
	return;
      }
      btrpkt->src_id = TOS_NODE_ID;
      btrpkt->dst_id = 3;
      btrpkt->counter = counter;
      if (call AMSend.send(AM_BROADCAST_ADDR, 
          &pkt, sizeof(NovelHopMsg)) == SUCCESS) {
        busy = TRUE;
      }
    }
  }

  event void AMSend.sendDone(message_t* msg, error_t err) {
    if (&pkt == msg) {
      busy = FALSE;
    }
  }

  event message_t* Receive.receive(message_t* msg, void* payload, uint8_t len){
    if (len == sizeof(NovelHopMsg)) {
      NovelHopMsg* btrpkt = (NovelHopMsg*)payload;

      if(btrpkt->dst_id == TOS_NODE_ID)
        setLeds(btrpkt->counter);

      //else if(btrpkt->src_id != TOS_NODE_ID){
      else{

      /*
        Forward the packet 

        */

         NovelHopMsg* btrpkt_new = 
  (NovelHopMsg*)(call Packet.getPayload(&pkt, sizeof(NovelHopMsg)));

          btrpkt_new->src_id = btrpkt->src_id;
          btrpkt_new->dst_id = btrpkt->dst_id;
          btrpkt_new->counter = btrpkt->counter;


        if (call AMSend.send(AM_BROADCAST_ADDR, 
          &pkt, sizeof(NovelHopMsg)) == SUCCESS) {
        busy = TRUE;
       }


      }


    }
    return msg;
  }
}
