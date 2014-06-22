// $Id: BlinkToRadio.h,v 1.4 2006-12-12 18:22:52 vlahan Exp $

#ifndef NOVELHOP_H
#define NOVELHOP_H

enum {
  AM_NOVELHOP = 6,
  TIMER_PERIOD_MILLI = 250
};

typedef nx_struct NovelHopMsg {
  nx_uint16_t counter;
  nx_uint16_t src_id;
  nx_uint16_t dst_id;
} NovelHopMsg;

#endif
