#ifndef _SAMPLE_VIRVI_H_
#define _SAMPLE_VIRVI_H_

#include <plat_type.h>   // 平台相关类型定义
#include <tsemaphore.h>  // 信号量相关接口
#include "media/mm_comm_vi.h"

#define MAX_FILE_PATH_SIZE (256)  // 文件路径最大长度

// 命令行参数结构体：保存配置文件路径
typedef struct SampleViResetCmdLineParam
{
    char mConfigFilePath[MAX_FILE_PATH_SIZE];  // 配置文件完整路径
}SampleViResetCmdLineParam;

// 测试配置结构体：定义一次完整测试的所有参数
typedef struct SampleViResetConfig
{
    int  mTestCount;         // 总共需要重复测试的次数
    int  mFrameCountStep1;   // 第一阶段捕获的帧数；达到该帧数后执行VI复位
    bool mbRunIsp;           // 是否同时跑ISP（图像信号处理）线程
    ISP_DEV mIspDev;         // 使用的ISP设备号
    VI_DEV  mVippStart;      // 起始VIPPI（Video Input PPI）设备号，取值范围 0~3
    VI_DEV  mVippEnd;        // 结束VIPPI设备号，取值范围 0~3
    int  mPicWidth;          // 主通道图像宽度
    int  mPicHeight;         // 主通道图像高度
    int  mSubPicWidth;       // 子通道图像宽度
    int  mSubPicHeight;      // 子通道图像高度
    int  mFrameRate;         // 帧率，单位 fps
    PIXEL_FORMAT_E mPicFormat; // 像素格式，例如 MM_PIXEL_FORMAT_YUV_PLANAR_420
}SampleViResetConfig;

// 虚拟VI通道信息：每个线程/通道的运行时数据
typedef struct VirViChnInfo
{
    pthread_t mThid;              // 工作线程 ID
    VI_DEV    mVipp;              // 物理 VIPP 设备号
    VI_CHN    mVirChn;            // 虚拟通道号
    AW_S32    mMilliSec;          // 获取图像的超时时间（毫秒）
    VIDEO_FRAME_INFO_S mFrameInfo; // 当前帧信息结构体
    int       mCaptureFrameCount;  // 本通道已捕获的帧数
} VirViChnInfo;

// 全局上下文：保存整个测试过程的状态与数据
typedef struct SampleViResetContext
{
    SampleViResetCmdLineParam mCmdLinePara;  // 命令行参数
    SampleViResetConfig       mConfigPara;   // 配置参数

    VI_ATTR_S   mViAttr;   // VI 属性配置
    VI_CHN      mVirChn;   // 当前使用的虚拟通道号

    // 二维数组：保存每个 VIPP 下每个虚拟通道的运行时信息
    VirViChnInfo mVirViChnArray[MAX_VIPP_DEV_NUM][MAX_VIR_CHN_NUM];

    int  mCaptureNum;        // 当前已捕获的总帧数（调试用）
    bool mFirstVippRunFlag;  // 标记与 ISP 连接的 VIPP 是否已启动
    bool mbIspRunningFlag;   // 标记 ISP 是否在运行
    int  mTestNum;           // 当前已执行的测试轮次
    // VIDEO_FRAME_INFO_S mFrameInfo; // 已注释：原打算全局缓存一帧，后未使用
}SampleViResetContext;

#endif  /* _SAMPLE_VIRVI_H_ */