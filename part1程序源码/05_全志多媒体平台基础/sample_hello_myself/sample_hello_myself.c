#include <mpi_sys.h>
#include <plat_log.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <wordexp.h>
#include <ion_memmanager.h>
#include "sample_hello_myself.h"

/*
 * 参考sample_hello示例程序，编写一个最简单的MPP程序，调用MPP的系统控制API，增加以下功能：
 * 1. 获取系统配置信息。
 * 2. 获取虚拟地址所使用的物理地址。
 * 3. 使用glog打印获取的信息。
*/

int main(int argc, char *argv[])
{
    int result = 0;
    GLogConfig stGLogConfig = {
      .FLAGS_logtostderr = 0,
      .FLAGS_colorlogtostderr = 1,
      .FLAGS_stderrthreshold = _GLOG_INFO,
      .FLAGS_minloglevel = _GLOG_INFO,
      .FLAGS_logbuflevel = -1,
      .FLAGS_logbufsecs = 0,
      .FLAGS_max_log_size = 1,
      .FLAGS_stop_logging_if_full_disk = 1,
    };

    strcpy(stGLogConfig.LogDir, "/tmp/log");
    strcpy(stGLogConfig.InfoLogFileNameBase, "LOG-");
    strcpy(stGLogConfig.LogFileNameExtension, "");
    log_init(argv[0], &stGLogConfig);

    MPP_SYS_CONF_S stSysConf;
    memset(&stSysConf, 0, sizeof(MPP_SYS_CONF_S));
    stSysConf.nAlignWidth = 32;
    AW_MPI_SYS_SetConf(&stSysConf);
    AW_MPI_SYS_Init();

    /* 获取系统配置信息: 图像对齐宽度*/
    MPP_SYS_CONF_S pstSysConf;
    AW_MPI_SYS_GetConf(&pstSysConf);
    alogd("set mpp sys conf sussess! Image alignment width:%d", pstSysConf.nAlignWidth);

    unsigned int nPhyAddr = 0;
    void *pVitAddr = NULL;
    ERRORTYPE ret = SUCCESS;
    ret = AW_MPI_SYS_MmzAlloc_Cached(&nPhyAddr, &pVitAddr, 128 * 1024);

    /* 获取虚拟地址所使用的物理地址 */
    SYS_VIRMEM_INFO_S pstMemInfo;
    AW_MPI_SYS_GetVirMemInfo(pVitAddr, &pstMemInfo);
    alogd("pVirtAddr used mPhyAddr: %d", pstMemInfo.mPhyAddr);

    if (ret != SUCCESS) {
        aloge("fatal error! malloc fail!");
    } else {
        alogd("phyAddr[0x%x], virtAddr[%p]", nPhyAddr, pVitAddr);
    }

    int i = 0;
    for (i = 0; i < 30; i++) {
        memset(pVitAddr, i, 64 * 1024);
        alogd("memset [%d]", i);
        alogd("result: pVirtAddr:0x%x,%x,%x", ((char *)pVitAddr)[0], ((char *)pVitAddr)[1], ((char *)pVitAddr)[2]);
        sleep(1);
    }

    AW_MPI_SYS_MmzFree(0, pVitAddr);
    AW_MPI_SYS_Exit();
    log_quit();
    alogd("%s test result: %s", argv[0], ((0 == result) ? "success" : "fail"));
    return result;
}