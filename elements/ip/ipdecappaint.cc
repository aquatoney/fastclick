/*
 * ipdecappaint.{cc,hh} -- element strips ip encapsulation header from front of packet
 * Alexander Yip
 *
 * Copyright (c) 2001 Massachusetts Institute of Technology
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, subject to the following
 * conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * Further elaboration of this license, including a DISCLAIMER OF ANY
 * WARRANTY, EXPRESS OR IMPLIED, is provided in the LICENSE file, which is
 * also accessible at http://www.pdos.lcs.mit.edu/click/license.html
 */

#include <click/config.h>
#include <click/package.hh>
#include "ipdecappaint.hh"
#include <click/confparse.hh>
#include <click/error.hh>
#include <click/glue.hh>
#include <click/click_ip.h>
#include <click/packet_anno.hh>


IPDecapPaint::IPDecapPaint()
  : Element(1, 1)
{
  MOD_INC_USE_COUNT;
}

IPDecapPaint::~IPDecapPaint()
{
  MOD_DEC_USE_COUNT;
}

Packet *
IPDecapPaint::simple_action(Packet *p)
{

  const click_ip *ip = p->ip_header();
  assert(ip);

  if (ip->ip_p != IP_PROTO_IPIP) {
    p->kill();
  }

  // set annotation using the first byte of the packet
  SET_PAINT_ANNO(p, p->data()[ sizeof(click_ip)] );


  // pull off first header and annotation byte
  p->pull( (int)p->ip_header_offset() + p->ip_header_length() + 1);
  
  return p;
}

EXPORT_ELEMENT(IPDecapPaint)
ELEMENT_MT_SAFE(IPDecapPaint)
