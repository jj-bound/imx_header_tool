#include "mk_imx8.h"

static struct option s_atCmdOption[] = {
    {"ivt_addr",required_argument,NULL,'i'},
    {"entry_addr",required_argument,NULL,'e'},
    {"bin_name",required_argument, NULL,'b'},
    {"output_name",required_argument, NULL, 'o'},
    {"boot_device", required_argument, NULL, 'd'},
    {"version", no_argument, NULL, 'v'},
    {NULL, 0, NULL, 0},
};

#if 0
static INT32 mk_imx8mq_image(imx8mq_header_t *ptImx8mqHead, INT8 *pcBinName, INT8 *pcOutputName, UINT32 uiBinOffest, UINT8 ucIsNeedHead);
#endif

static INT32 mk_imx8mq_image(INT32 iFd, void *pucBuf, UINT32 uiSize);

static void list_insert_tail(struct list_head *ptListHead, T_ENTRY_FILE_INFO *ptEntryFileInfo)
{
	
}

INT32 imx8mq_mkimage(int argc , char **argv)
{
    int c = 0;
    int iOptIndex=0;
    unsigned int uiIvtAddr = 0;
    unsigned int uiEntryAddr = 0;
    UINT32 uiDcdSize = 0;
    UINT32 uiPluginFlag = 0;
    UINT32 uiIvtOffset = 0;
    INT32 iBinFd = 0;
    INT32 iRet = 0;
    char acBinName[50];
    char acOutputName[50];
    imx8mq_header_t tImx8mqHead;
    ivt_header_t tIvtHead;
    boot_data_t tBootData;
    flash_header_t tFlashHead;
    struct stat tSbuf;
	//struct list_head tEntryFileListHead;
	T_ENTRY_FILE_INFO atEntryFileInfo[10];
	INT32 iOutFd = 0;
	//INT32 iBinFd = 0;
	void *pucBuf = NULL;
	UINT32 uiFileSize = 0;

    memset(acBinName, 0x00, sizeof(acBinName));
    memset(acOutputName, 0x00, sizeof(acOutputName));
    memset(&tImx8mqHead, 0x00, sizeof(tImx8mqHead));
    memset(&tIvtHead, 0x00, sizeof(tIvtHead));
    memset(&tBootData, 0x00, sizeof(tBootData));
    memset(&tFlashHead, 0x00, sizeof(tFlashHead));
    memset(&tSbuf, 0x00, sizeof(tSbuf));
	memset(atEntryFileInfo, 0x00, sizeof(atEntryFileInfo));
	//INIT_LIST_HEAD(tEntryFileListHead);
	
    while(1)
    {
       c = getopt_long_only(argc, argv, "i:e:b:o:d:v",s_atCmdOption,&iOptIndex);
       if(c == -1)
       {
           break;
       }

       switch(c)
       {
           case 'i':
           {
               uiIvtAddr = strtoll(optarg,NULL,0);
               printf("ivt addr:%#x\n", uiIvtAddr);
               break;
           }

           case 'e':
           {
#if 0
			   atEntryFileInfo[iEntryNum].uiEntryAddr = strtoll(optarg,NULL,0);
			   atEntryFileInfo[iEntryNum].ucFileMatch = 0;
			   iEntryNum++;
			   //list_insert_tail(&tEntryFileListHead, ptEntryFileInfo);
#else			   
               uiEntryAddr = strtoll(optarg,NULL,0);
			   printf("entry addr:%#x\n", uiEntryAddr);
#endif
               break;
           }

           case 'b':
           {
               memcpy(acBinName, optarg, sizeof(acBinName));
               printf("bin name:%s\n", acBinName);
               break;
           }

           case 'o':
           {
               memcpy(acOutputName, optarg, sizeof(acOutputName));
               printf("outpur name:%s\n", acOutputName);
               break;
           }

           case 'd':
           {
               uiIvtOffset = EMMC_IVT_OFFSET;
               break;
           }

           case 'v':
           {
               printf("current version:V%d.%d\n",MK_IMX8MQ_MAJOR_VERSION,MK_IMX8MQ_MINOR_VERSION);
               break;
           }

           default:
           {
               
               break;
           }
       }
    }

    tIvtHead.tag = IVT_HEADER_TAG;
    tIvtHead.length = cpu_to_be16(sizeof(flash_header_t));
    tIvtHead.version = IVT_HEADER_VERSION;
    memcpy(&(tFlashHead.header), &tIvtHead, sizeof(ivt_header_t));

    tFlashHead.entry = uiEntryAddr;
    tFlashHead.self = uiEntryAddr - sizeof(imx8mq_header_t) - ALIGN(uiDcdSize, 64);
    tFlashHead.boot_data_ptr = tFlashHead.self + offsetof(imx8mq_header_t, boot_data);
    if (uiDcdSize)
    {
       tFlashHead.dcd_ptr = tFlashHead.self + sizeof(imx8mq_header_t);
    }
    else
    {
       tFlashHead.dcd_ptr = 0;
    }

    iBinFd = open(acBinName, O_RDONLY | O_BINARY);
    if (iBinFd < 0)
    {
       printf("open bin file fail,no such file\n");
       return -1;
    }

    iRet = fstat(iBinFd, &tSbuf);
    if (iRet < 0)
    {
       printf("bin file size error:%s\n", acBinName);
       close(iBinFd);
       return -2;
    }
    close(iBinFd);

    if(uiPluginFlag == 0)
    {
       tBootData.start = tFlashHead.self - uiIvtOffset;
       tBootData.size = ALIGN(uiIvtOffset + sizeof(imx8mq_header_t) + ALIGN(uiDcdSize,64) 
                        + BIN_TOTAL_SIZE, 0x200);//+ tSbuf.st_size, 0x200);
    }
    tBootData.plugin = uiPluginFlag;

    memcpy(&tImx8mqHead.boot_data, &tBootData, sizeof(tBootData));

    tFlashHead.csf = tBootData.start + tBootData.size;
    memcpy(&tImx8mqHead.fhdr, &tFlashHead, sizeof(tFlashHead));

	pucBuf = malloc(BIN_TOTAL_SIZE);
	if (NULL == pucBuf)
	{
		printf("pucBuf is NULL\n");
		return -3;
	}
	
	/* 1.放置头部结构体 */
	memcpy(pucBuf, &tImx8mqHead, sizeof(tImx8mqHead));
	
	/* 2.放置bin file */
	iBinFd = open(acBinName, O_RDONLY | O_BINARY);
	if (iBinFd < 0)
	{
		printf("open %s fail\n", acBinName);
		return -4;
	}
	fstat(iBinFd, &tSbuf);
	if (tSbuf.st_size > BIN_SIZE)
	{
		printf("bin exceed size:%#x\n", tSbuf.st_size);
		uiFileSize = BIN_SIZE;
	}
	else
	{
		uiFileSize = tSbuf.st_size;
	}
	read(iBinFd, pucBuf+BIN_OFFEST, uiFileSize); 
	close(iBinFd);
	
	/* 3.lpddr4 1d imem */
	iBinFd = open(LPDDR4_1D_IMEM, O_RDONLY | O_BINARY);
	if (iBinFd < 0)
	{
		printf("open %s fail\n", LPDDR4_1D_IMEM);
		return -4;
	}
	fstat(iBinFd, &tSbuf);
	read(iBinFd, pucBuf+LPDDR4_1D_IMEM_OFFEST, tSbuf.st_size);
	close(iBinFd);
	
	/* 4.lpddr4 1d dmem */
	iBinFd = open(LPDDR4_1D_DMEM, O_RDONLY | O_BINARY);
	if (iBinFd < 0)
	{
		printf("open %s fail\n", LPDDR4_1D_DMEM);
		return -4;
	}
	fstat(iBinFd, &tSbuf);
	read(iBinFd, pucBuf+LPDDR4_1D_DMEM_OFFSET, tSbuf.st_size);
	close(iBinFd);	
	
	/* 5.lpddr4 2d imem */
	iBinFd = open(LPDDR4_2D_IMEM, O_RDONLY | O_BINARY);
	if (iBinFd < 0)
	{
		printf("open %s fail\n", LPDDR4_2D_IMEM);
		return -4;
	}
	fstat(iBinFd, &tSbuf);
	read(iBinFd, pucBuf+LPDDR4_2D_IMEM_OFFEST, tSbuf.st_size);
	close(iBinFd);	
	
	/* 6.lpddr4 2d dmem */
	iBinFd = open(LPDDR4_2D_DMEM, O_RDONLY | O_BINARY);
	if (iBinFd < 0)
	{
		printf("open %s fail\n", LPDDR4_2D_DMEM);
		return -4;
	}
	fstat(iBinFd, &tSbuf);
	read(iBinFd, pucBuf+LPDDR4_2D_DMEM_OFFEST, tSbuf.st_size);
	close(iBinFd);	
	
	/* 生成文件 */
	iOutFd = open(acOutputName, O_RDWR|O_CREAT|O_TRUNC|O_BINARY, 0666); 
	if (iOutFd < 0)
	{
		printf("create %s file fail\n", acOutputName);
		return -5;
	}
	
	write(iOutFd, pucBuf, BIN_TOTAL_SIZE);
	close(iOutFd);
	
	free(pucBuf);
	pucBuf = NULL;
	return 0;
}

static INT32 mk_imx8mq_image(INT32 iFd, void *pucBuf, UINT32 uiSize)
{
	write(iFd, pucBuf, uiSize);
	return 0;
}

#if 0
static INT32 mk_imx8mq_image(imx8mq_header_t *ptImx8mqHead, INT8 *pcBinName, INT8 *pcOutputName, UINT32 uiBinOffest, UINT8 ucIsNeedHead)
{
    INT32 iOutFd = 0;
    INT32 iBinFd = 0;
    UINT32 uiBinSize = 0;
    INT32 iRet = 0;
    UINT32 uiCurSize = 0;
    UINT8 *pucBuf = NULL;
    UINT32 uiPerSize = 0;
    struct stat tSbuf;

    memset(&tSbuf, 0x00, sizeof(tSbuf));

    if (NULL == ptImx8mqHead)
    {
        printf("ptImx8mqHead is NULL\n");
        return -1;
    }


    iBinFd = open(pcBinName, O_RDONLY | O_BINARY);
    if (iBinFd < 0)
    {
        printf("open bin file fail,no such file\n");
        return -2;
    }
    
    iOutFd = open(pcOutputName, O_RDWR|O_CREAT|O_TRUNC|O_BINARY, 0666); 
    if (iOutFd < 0)
    {
       printf("create output file fail:%s\n", pcOutputName);
       close(iBinFd);
       return -3;
    }
#if 0
    fstat(iOutFd, &tSbuf);
	if(tSbuf.st_size <= uiBinOffest)
	{
		pucBuf = malloc(uiBinOffest-tSbuf.st_size);
		memset(pucBuf, 0x0, uiBinOffest-tSbuf.st_size);
		lseek
		write(iOutFd, pucBuf, uiBinOffest-tSbuf.st_size);
	}
#endif
	//fstat(iOutFd, &tSbuf);
	lseek(iOutFd, 0, SEEK_END);
#if 0	
	if(tSbuf.st_size <= uiBinOffest)
	{
		lseek(iOutFd, uiBinOffest-tSbuf.st_size, SEEK_CUR);
	}
#endif	
	if (1 == ucIsNeedHead)
	{
		write(iOutFd, ptImx8mqHead, sizeof(*ptImx8mqHead));
	}
	
    iRet = fstat(iBinFd, &tSbuf);
    if (iRet < 0)
    {
        printf("bin file size error\n");
        iRet = -4;
        goto exit_mkim8mq;
    }

    uiBinSize = tSbuf.st_size;
	printf("file size:%#x\n", uiBinSize);
    pucBuf = (UINT8 *)malloc(BUF_SIZE);
    if (NULL == pucBuf)
    {
        printf("malloc buf fail\n");
        iRet = -5;
        goto exit_mkim8mq;
    }
    
    while(uiCurSize < uiBinSize)
    {
        uiPerSize = ((uiBinSize - uiCurSize) > BUF_SIZE) ? BUF_SIZE : (uiBinSize - uiCurSize);
        read(iBinFd, pucBuf, uiPerSize);
        write(iOutFd, pucBuf, uiPerSize);
        uiCurSize += uiPerSize;
    }
	printf("test1\n");
	uiBinOffest -= uiCurSize;
	uiCurSize = 0;
	while(uiCurSize < uiBinOffest)
    {
        uiPerSize = ((uiBinOffest - uiCurSize) > BUF_SIZE) ? BUF_SIZE : (uiBinSize - uiCurSize);
         memset(pucBuf, 0x00, uiPerSize);
		//read(iBinFd, pucBuf, uiPerSize);
        write(iOutFd, pucBuf, uiPerSize);
        uiCurSize += uiPerSize;
    }
	printf("test2\n");
	free(pucBuf);
	close(iBinFd);
	close(iOutFd);

    return 0;

exit_mkim8mq:
    close(iBinFd);
    close(iOutFd);
    return iRet;
}
#endif
