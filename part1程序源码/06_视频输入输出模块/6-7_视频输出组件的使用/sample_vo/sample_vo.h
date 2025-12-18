
#ifndef _SAMPLE_VO_H_
#define _SAMPLE_VO_H_

#include <plat_type.h>
#include <tsemaphore.h>
#include <mpi_clock.h>
#include <mm_comm_vo.h>
#include <mm_comm_sys.h>

#define MAX_FILE_PATH_SIZE (256)

typedef struct SampleVOCmdLineParam
{
    char mConfigFilePath[MAX_FILE_PATH_SIZE];
}SampleVOCmdLineParam;

typedef struct SampleVOConfig
{
    int mPicWidth; //图片的宽度
    char mYuvFilePath[MAX_FILE_PATH_SIZE]; // YUV 文件的路经
    int mPicHeight; //图片的高度
    int mDisplayWidth; //显示的宽度
    int mDisplayHeight; //显示的高度
    PIXEL_FORMAT_E mPicFormat; //MM_PIXEL_FORMAT_YUV_PLANAR_420 存储图片的像素格式
    int mFrameRate; //帧率
    VO_INTF_TYPE_E mDispType; //显示接口类型
    VO_INTF_SYNC_E mDispSync; //显示同步类型
}SampleVOConfig;

typedef struct SampleVOFrameNode
{
    VIDEO_FRAME_INFO_S mFrame;
    struct list_head mList;
}SampleVOFrameNode;

typedef struct SampleVOFrameManager
{
    struct list_head mIdleList; //SampleVOFrameNode
    struct list_head mUsingList;
    int mNodeCnt;//帧节点数量
    pthread_mutex_t mLock; //互斥锁，保护对结构体中其他成员的并发访问
    VIDEO_FRAME_INFO_S* (*PrefetchFirstIdleFrame)(void *pThiz); //函数指针，从空闲列表中预取第一个空闲帧的操作
    int (*UseFrame)(void *pThiz, VIDEO_FRAME_INFO_S *pFrame);//用于将指定的帧 pFrame 加入到正在使用的列表中，并返回操作结果的整数值。
    int (*ReleaseFrame)(void *pThiz, unsigned int nFrameId);//释放指定 nFrameId 的帧资源
}SampleVOFrameManager;
int initSampleVOFrameManager();
int destroySampleVOFrameManager();

typedef struct SampleVOContext
{
    SampleVOCmdLineParam mCmdLinePara; //命令行参数相关的配置
    SampleVOConfig mConfigPara; //视频输出的配置参数，包括文件路径、尺寸、格式、帧率等信息

    FILE *mFpYuvFile; //YUV 文件
    SampleVOFrameManager mFrameManager; //视频帧管理器，用于管理视频帧资源
    pthread_mutex_t mWaitFrameLock; //保护等待帧的操
    int mbWaitFrameFlag; //等待帧的标志
    cdx_sem_t mSemFrameCome; //帧到达的信号量
    int mUILayer; // UI 层级

    MPP_SYS_CONF_S mSysConf; //存储系统配置信息
    VO_DEV mVoDev; //VO 设备
    VO_LAYER mVoLayer; //VO 图层
    VO_VIDEO_LAYER_ATTR_S mLayerAttr; // VO 视频图层属性
    VO_CHN mVOChn; //VO 通道
    CLOCK_CHN mClockChn; //时钟通道
    CLOCK_CHN_ATTR_S mClockChnAttr; //时钟通道属性
}SampleVOContext;
int initSampleVOContext();
int destroySampleVOContext();

#endif  /* _SAMPLE_VO_H_ */

