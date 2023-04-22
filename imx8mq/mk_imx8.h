#ifndef IMX8MQ_STRUCT_H_
#define IMX8MQ_STRUCT_H_

#include "common.h"

#define MK_IMX8MQ_MAJOR_VERSION 1
#define MK_IMX8MQ_MINOR_VERSION 0

#define IVT_HEADER_TAG 0xD1
#define IVT_HEADER_VERSION 0x41

#define EMMC_IVT_OFFSET 0x400
#if 0
#define BIN_OFFSET (0x100000)
#define LPDDR4_1D_DMEM_OFFSET (0x8000)//(BIN_OFFSET+0x8000)
#define LPDDR4_2D_IMEM_OFFSET (0x800)//(LPDDR4_1D_DMEM_OFFSET + 0x800)
#define LPDDR4_2D_DMEM_OFFSET (0x8000)//(LPDDR4_2D_IMEM_OFFSET + 0x8000)
#else
#define BIN_SIZE (0xc800)//(0x100000)
#define BIN_OFFEST (sizeof(imx8mq_header_t))

#define LPDDR4_1D_IMEM_SIZE (0x8000)
#define LPDDR4_1D_IMEM_OFFEST (BIN_SIZE + BIN_OFFEST)

#define LPDDR4_1D_DMEM_SIZE (0x4000)
#define LPDDR4_1D_DMEM_OFFSET (LPDDR4_1D_IMEM_OFFEST + LPDDR4_1D_IMEM_SIZE)

#define LPDDR4_2D_IMEM_SIZE (0x8000)
#define LPDDR4_2D_IMEM_OFFEST (LPDDR4_1D_DMEM_OFFSET+LPDDR4_1D_DMEM_SIZE)

#define LPDDR4_2D_DMEM_SIZE (0x4000)
#define LPDDR4_2D_DMEM_OFFEST (LPDDR4_2D_IMEM_OFFEST+LPDDR4_2D_IMEM_SIZE)

#define BIN_TOTAL_SIZE (LPDDR4_2D_DMEM_OFFEST+LPDDR4_2D_DMEM_SIZE)
#endif

#define LPDDR4_FILE_PATH "/home/jsf/imx8mq/code/imx_bin"
#define LPDDR4_1D_IMEM LPDDR4_FILE_PATH"/lpddr4_pmu_train_1d_imem.bin"
#define LPDDR4_1D_DMEM LPDDR4_FILE_PATH"/lpddr4_pmu_train_1d_dmem.bin"
#define LPDDR4_2D_IMEM LPDDR4_FILE_PATH"/lpddr4_pmu_train_2d_imem.bin"
#define LPDDR4_2D_DMEM LPDDR4_FILE_PATH"/lpddr4_pmu_train_2d_dmem.bin"

#define BUF_SIZE 0x2000

typedef struct {
	UINT8 tag;
	UINT16 length;
	UINT8 version;
} __attribute__((packed)) ivt_header_t;

typedef struct {
	UINT32 start;
	UINT32 size;
	UINT32 plugin;
	UINT32 padding[1];
} boot_data_t;

typedef struct {
	ivt_header_t header;
	UINT32 entry;
    UINT32 reserved1;
	UINT32 dcd_ptr;
	UINT32 boot_data_ptr;
	UINT32 self;
	UINT32 csf;
	UINT32 reserved2;
} flash_header_t;

typedef struct {
	flash_header_t fhdr;
	boot_data_t boot_data;
	UINT32 alignment[4];
} imx8mq_header_t;

struct list_head
{
	void *next;
	void *prev;
};

typedef struct _entry_file_info
{
	struct list_head tList;
	UINT32 uiEntryAddr;
	UINT8 ucFileMatch;
	char acBinName[50];
	struct stat tStBuf;
} T_ENTRY_FILE_INFO;

#define INIT_LIST_HEAD(x) \ 
(	\
	x.next = x.prev = NULL \
)

extern INT32 imx8mq_mkimage(int argc , char **argv);

#endif
