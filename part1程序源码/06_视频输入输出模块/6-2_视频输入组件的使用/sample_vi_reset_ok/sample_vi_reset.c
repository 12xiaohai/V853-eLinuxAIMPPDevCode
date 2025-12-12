#include <endian.h>
#include <errno.h>
#include <fcntl.h>
#include <getopt.h>
#include <pthread.h>
#include <signal.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "media/mm_comm_vi.h"
#include "media/mpi_isp.h"
#include "media/mpi_sys.h"
#include "media/mpi_vi.h"
#include <mpi_videoformat_conversion.h>
#include <utils/plat_log.h>
#include "sample_vi_reset.h"
#include "sample_vi_reset_config.h"
#include <confparser.h>

static int parseCmdLine(SampleViResetContext *pContext, int argc,
                        char *argv[]) {
  int ret = -1;

  alogd("argc=%d", argc);
  if (argc != 3) {
    printf("CmdLine param:\n"
           "\t-path ./sample_CodecParallel.conf\n");
    return -1;
  }

  while (*argv) {
    if (!strcmp(*argv, "-path")) {
      argv++;
      if (*argv) {
        ret = 0;
        if (strlen(*argv) >= MAX_FILE_PATH_SIZE) {
          aloge("fatal error! file path[%s] too long:!", *argv);
        }
        if (pContext) {
          strncpy(pContext->mCmdLinePara.mConfigFilePath, *argv,
                  MAX_FILE_PATH_SIZE - 1);
          pContext->mCmdLinePara.mConfigFilePath[MAX_FILE_PATH_SIZE - 1] = '\0';
        }
      }
    } else if (!strcmp(*argv, "-h")) {
      printf("CmdLine param:\n"
             "\t-path ./sample_CodecParallel.conf\n");
      break;
    } else if (*argv) {
      argv++;
    }
  }
  return ret;
}

static ERRORTYPE LoadSampleViResetConfig(SampleViResetConfig *pConfig,
                                         const char *conf_path) {
  if (NULL == pConfig) {
    aloge("pConfig is NULL!");
    return FAILURE;
  }

  if (NULL == conf_path) {
    aloge("user not set config file!");
    return FAILURE;
  }

  pConfig->mTestCount = 0;
  pConfig->mFrameCountStep1 = 300;
  pConfig->mbRunIsp = true;
  pConfig->mIspDev = 0;
  pConfig->mVippStart = 0;
  pConfig->mVippEnd = 0;
  pConfig->mPicWidth = 1920;
  pConfig->mPicHeight = 1080;
  pConfig->mSubPicWidth = 640;
  pConfig->mSubPicHeight = 360;
  pConfig->mFrameRate = 20;
  pConfig->mPicFormat = MM_PIXEL_FORMAT_YVU_SEMIPLANAR_420;

  CONFPARSER_S stConfParser;
  if (0 > createConfParser(conf_path, &stConfParser)) {
    aloge("load conf fail!");
    return FAILURE;
  }
  pConfig->mTestCount =
      GetConfParaInt(&stConfParser, SAMPLE_VI_RESET_TEST_COUNT, 0);
  alogd("mTestCount=%d", pConfig->mTestCount);

  pConfig->mVippStart =
      GetConfParaInt(&stConfParser, SAMPLE_VI_RESET_VIPP_ID_START, 0);
  pConfig->mVippEnd =
      GetConfParaInt(&stConfParser, SAMPLE_VI_RESET_VIPP_ID_END, 0);

  alogd("vi config: vipp scope=[%d,%d], captureSize:[%dx%d,%dx%d] "
        "frameRate:%d, pixelFormat:0x%x",
        pConfig->mVippStart, pConfig->mVippEnd, pConfig->mPicWidth,
        pConfig->mPicHeight, pConfig->mSubPicWidth, pConfig->mSubPicHeight,
        pConfig->mFrameRate, pConfig->mPicFormat);

  destroyConfParser(&stConfParser);

  return SUCCESS;
}

int main(int argc, char *argv[]) {
  int result = 0;

  /* 步骤 1: 启动MPP和Glog库 */
  GLogConfig stGLogConfig = {
      .FLAGS_logtostderr = 1,
      .FLAGS_colorlogtostderr = 1,
      .FLAGS_stderrthreshold = _GLOG_INFO,
      .FLAGS_minloglevel = _GLOG_INFO,
      .FLAGS_logbuflevel = -1,
      .FLAGS_logbufsecs = 0,
      .FLAGS_max_log_size = 1,
      .FLAGS_stop_logging_if_full_disk = 1,
  };
  log_init(argv[0], &stGLogConfig);
  alogd("app:[%s] begin! time=%s, %s", argv[0], __DATE__, __TIME__);

  MPP_SYS_CONF_S stSysConf;
  memset(&stSysConf, 0, sizeof(stSysConf));
  stSysConf.nAlignWidth = 32;
  AW_MPI_SYS_SetConf(&stSysConf);
  /* 步骤 3: 初始化ISP */
  ERRORTYPE ret = AW_MPI_SYS_Init();
  if (ret != SUCCESS) {
    aloge("MPP System Init failed!");
    log_quit();
    goto _exit;
  }

  /* 步骤 2: 获取配置文件信息并解析 */
  char *pConfigFilePath = NULL;
  SampleViResetContext *pContext =
      (SampleViResetContext *)malloc(sizeof(SampleViResetContext));
  if (NULL == pContext) {
    aloge("fatal error! malloc fail!");
    return -1;
  }
  memset(pContext, 0, sizeof(SampleViResetContext));
  if (parseCmdLine(pContext, argc, argv) != SUCCESS) {
    aloge("fatal error! parse cmd line fail");
    result = -1;
    goto _exit;
  }
  pConfigFilePath = pContext->mCmdLinePara.mConfigFilePath;

  if (LoadSampleViResetConfig(&pContext->mConfigPara, pConfigFilePath) !=
      SUCCESS) {
    aloge("fatal error! no config file or parse conf file fail");
    result = -1;
    goto _exit;
  }

  /* 步骤 7: 退出MPP  */
  AW_MPI_SYS_Exit();
  log_quit();
  alogd("Step 7: MPP System Exited. Program finished successfully.");

_exit:
  free(pContext);
  alogd("%s test result: %s", argv[0], (result == 0) ? "success" : "fail");
  return result;
}