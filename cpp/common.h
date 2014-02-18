//
//  common.h
//  NetworkingPoC
//
//  Created by George Burgess IV on 2/2/13.
//  Copyright (c) 2013 George Burgess IV. All rights reserved.
//

#ifndef NetworkingPoC_common_h
#define NetworkingPoC_common_h

//#define NET_USE_EXCEPTIONS

#ifdef NET_USE_EXCEPTIONS
#define THROW(__n) throw __n
#else
#define THROW(__n)
#endif

// Determining byte order.
#if defined(__BYTE_ORDER__) && __BYTE_ORDER__ == __ORDER_BIG_ENDIAN__ 
#define NET_USE_BIG_ENDIAN 1
#else
#define NET_USE_LITTLE_ENDIAN 1
#endif

#endif
