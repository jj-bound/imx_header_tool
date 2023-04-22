#ifndef _COMMON_H_
#define _COMMON_H_

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>
#include <sys/stat.h>
#include <getopt.h>
#include <errno.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <zlib.h>

#ifndef O_BINARY
#define O_BINARY 0 
#endif


#define uswap_16(x) \
	((((x) & 0xff00) >> 8) | \
	 (((x) & 0x00ff) << 8))
#define uswap_32(x) \
	((((x) & 0xff000000) >> 24) | \
	 (((x) & 0x00ff0000) >>  8) | \
	 (((x) & 0x0000ff00) <<  8) | \
	 (((x) & 0x000000ff) << 24))
#define _uswap_64(x, sfx) \
	((((x) & 0xff00000000000000##sfx) >> 56) | \
	 (((x) & 0x00ff000000000000##sfx) >> 40) | \
	 (((x) & 0x0000ff0000000000##sfx) >> 24) | \
	 (((x) & 0x000000ff00000000##sfx) >>  8) | \
	 (((x) & 0x00000000ff000000##sfx) <<  8) | \
	 (((x) & 0x0000000000ff0000##sfx) << 24) | \
	 (((x) & 0x000000000000ff00##sfx) << 40) | \
	 (((x) & 0x00000000000000ff##sfx) << 56))

#if defined(__GNUC__)
# define uswap_64(x) _uswap_64(x, ull)
#else
#error
# define uswap_64(x) _uswap_64(x, )
#endif

#if __BYTE_ORDER == __LITTLE_ENDIAN
#define cpu_to_le16(x)		(x)
#define cpu_to_le32(x)		(x)
#define cpu_to_le64(x)		(x)
#define le16_to_cpu(x)		(x)
#define le32_to_cpu(x)		(x)
#define le64_to_cpu(x)		(x)
#define cpu_to_be16(x)		uswap_16(x)
#define cpu_to_be32(x)		uswap_32(x)
#define cpu_to_be64(x)		uswap_64(x)
#define be16_to_cpu(x)		uswap_16(x)
#define be32_to_cpu(x)		uswap_32(x)
#define be64_to_cpu(x)		uswap_64(x)
#else
#error
#define cpu_to_le16(x)		uswap_16(x)
#define cpu_to_le32(x)		uswap_32(x)
#define cpu_to_le64(x)		uswap_64(x)
#define le16_to_cpu(x)		uswap_16(x)
#define le32_to_cpu(x)		uswap_32(x)
#define le64_to_cpu(x)		uswap_64(x)
#define cpu_to_be16(x)		(x)
#define cpu_to_be32(x)		(x)
#define cpu_to_be64(x)		(x)
#define be16_to_cpu(x)		(x)
#define be32_to_cpu(x)		(x)
#define be64_to_cpu(x)		(x)
#endif

#if 0
#define cpu_to_le16(x)		(x)
#define cpu_to_le32(x)		(x)
#define cpu_to_le64(x)		(x)
#define le16_to_cpu(x)		(x)
#define le32_to_cpu(x)		(x)
#define le64_to_cpu(x)		(x)
#define cpu_to_be16(x)		uswap_16(x)
#define cpu_to_be32(x)		uswap_32(x)
#define cpu_to_be64(x)		uswap_64(x)
#define be16_to_cpu(x)		uswap_16(x)
#define be32_to_cpu(x)		uswap_32(x)
#define be64_to_cpu(x)		uswap_64(x)
#endif

#define ALIGN(x,a)		__ALIGN_MASK((x),(__typeof__(x))(a)-1, a)
#define __ALIGN_MASK(x,mask,mask2)	(((x)+(mask))/(mask2)*(mask2))

#undef offsetof
#define offsetof(TYPE, MEMBER) ((size_t) &((TYPE *)0)->MEMBER)

typedef char INT8;
typedef unsigned char UINT8;
typedef short int INT16;
typedef unsigned short int UINT16;
typedef int INT32;
typedef unsigned int UINT32;
typedef long long INT64;
typedef unsigned long long UINT64;


#endif
