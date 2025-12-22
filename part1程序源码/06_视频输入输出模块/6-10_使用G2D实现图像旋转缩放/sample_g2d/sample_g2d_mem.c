//#define LOG_NDEBUG 0
#define LOG_TAG "SampleG2dMem"

#include <memoryAdapter.h>
#include "sc_interface.h"
#include "plat_log.h"
#include <ion_memmanager.h>

int g2d_MemOpen(void)//打开和初始化 ION 内存管理器
{
    aloge("zjx_g2d_mem_open");
    return ion_memOpen();
}

int g2d_MemClose(void)//关闭和清理与 ION 内存
{
    return ion_memClose();
}

unsigned char* g2d_allocMem(unsigned int size)//使用ION分配内存分配内存
{
    IonAllocAttr allocAttr;
    memset(&allocAttr, 0, sizeof(IonAllocAttr));
    allocAttr.mLen = size;
    allocAttr.mAlign = 0;
    allocAttr.mIonHeapType = IonHeapType_IOMMU;
    allocAttr.mbSupportCache = 0;
    return ion_allocMem_extend(&allocAttr);//内存分配，并返回分配得到的内存虚拟地址指针
}

int g2d_freeMem(void *vir_ptr)//接受一个指向要释放内存的虚拟地址的指针 vir_ptr，然后调用 ion_freeMem 函数来释放该内存
{
    return ion_freeMem(vir_ptr);//释放分配的内存并从内存管理器的列表中删除相应的内存节点
}

unsigned int g2d_getPhyAddrByVirAddr(void *vir_ptr)//获取与给定虚拟地址对应的物理地址
{
    return ion_getMemPhyAddr(vir_ptr);
}

int g2d_flushCache(void *vir_ptr, unsigned int size)
{
    return ion_flushCache(vir_ptr, size);//刷新特定虚拟地址范围对应的缓存
}
