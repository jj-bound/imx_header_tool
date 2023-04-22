#include "mk_imx8.h"

int main(int argc, char **argv)
{
    INT32 iRet = 0;
    
#ifdef IMX8_MQ
    iRet = imx8mq_mkimage(argc, argv);
    if (iRet < 0)
    {
        printf("mkimage error:%d\n", iRet);
    }
    return iRet;
#else
    printf("unknown mkimage bin,please config SOC_CHIP\n");
    return -1;
#endif
}
