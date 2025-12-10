#使用gcc编译.O文件 COMPILE_PREX = /home/sdk/prebuilt/gcc/linux-x86/arm/toolchain-sunxi-musl/toolchain/bin/arm-openwrt-linux-muslgnueabi-
#使用g++链接.O文件
CXX = $(COMPILE_PREX)
AR = $(COMPILE_PREX)ar
CC = $(COMPILE_PREX)gcc
XX = $(CXX)g++
NM = $(COMPILE_PREX)nm
CPP = $(COMPILE_PREX)g++
STRIP = $(COMPILE_PREX)strip
OBJCOPY = $(COMPILE_PREX)objcopy
OBJDUMP = $(COMPILE_PREX)objdump

#全志芯片头文件选项
AWCHIP_CFLAGS := -DAWCHIP=0x1886#AW_V853

#编译所用变量
ROOT_DIR = $(abspath .)
OUTPUT_DIR = $(ROOT_DIR)/output/
OUTPUT_DIR_OBJS = $(OUTPUT_DIR)/objs

#sammple目录
USER_SRC_BASE_DIR 	=  $(ROOT_DIR)/sample

#多媒体静态库目录
mpp_lib_dir=$(ROOT_DIR)/aw_pack_src/lib_aw/lib/eyesee-mpp

#需要使用的动态库目录
rootfs_share_lib_dir=$(ROOT_DIR)/share_lib

#包含多媒体库位置
LINKFLAGS = \
	-lpthread -lrt -ldl -lm -Wl,--gc-sections \
	-L$(mpp_lib_dir) \
	-L$(rootfs_share_lib_dir)

#自动化链接库文件夹里面所有的.a库
#遍历所有的.a库文件
lib_files := $(foreach dir,$(mpp_lib_dir),$(wildcard $(dir)/*.a))
#将文件去掉目录，只保留文件名
lib_nordir :=  $(notdir $(lib_files))
#将库的名字去除头lib 和 尾的.a
lib_name := $(patsubst lib%,%,$(patsubst %.a,%,$(lib_nordir)))
#给所有库添加-l参数
ld_mpp_libs := $(addprefix -l, $(lib_name))

mpp_lib := \
  -Wl,-Bstatic -Wl,--whole-archive -Wl,--start-group \
  $(ld_mpp_libs) \
  -Wl,--end-group \
  -Wl,--no-whole-archive \
  -Wl,-Bdynamic  \
  -lglog \
  -lasound

#手动多媒体包含的库，根据自己的选择增减
#mpp_lib := \
#  -Wl,-Bstatic -Wl,--whole-archive -Wl,--start-group \
#  -llog \
#  -lion  \
#  -law_mpp \
#  -lmedia_utils \
#  -lMemAdapter  \
#  -lVE  \
#  -lcdc_base  \
#  -lcedarxstream  \
#  -lsample_confparser  \
#  -lcdx_common  \
#  -lcdx_base  \
#  -lResample  \
#  -lISP  \
#  -lisp_dev  \
#  -lisp_ini  \
#  -liniparser  \
#  -lisp_ae  \
#  -lisp_af  \
#  -lisp_afs  \
#  -lisp_awb  \
#  -lisp_base  \
#  -lisp_gtm  \
#  -lisp_iso  \
#  -lisp_math  \
#  -lisp_md  \
#  -lisp_pltm  \
#  -lisp_rolloff \
#  -lvencoder  \
#  -lvenc_codec  \
#  -lvenc_base  \
#  -lcedarx_aencoder  \
#  -lAec \
#  -lDrc  \
#  -lAgc  \
#  -lAns  \
#  -lVIPlite \
#  -lVIPuser \
#  -lawnn_det \
#  -Wl,--end-group \
#  -Wl,--no-whole-archive \
#  -Wl,-Bdynamic  \
#  -lglog \
#  -lasound


CCFLAGS = \
	-g

#加一些编译参数。其中-Wl,-z,stack-size=1048576 表示设置线程堆栈为1M,避免不够用导致崩溃
DEFINES = \
	-Os -pipe -march=armv7-a -mtune=cortex-a7 -mfpu=neon -g3 -fno-caller-saves -Wno-unused-result -mfloat-abi=hard  -Wformat  -Werror=format-security -fstack-protector -Wl,-z,now -Wl,-z,relro -Wl,-z,stack-size=1048576 

#全志头文件路径
USER_INC_BASE_DIR  += $(ROOT_DIR)/aw_pack_src/lib_aw/include/eyesee-mpp/viplite
USER_INC_BASE_DIR  += $(ROOT_DIR)/aw_pack_src/lib_aw/include/eyesee-mpp/middleware/include/utils
USER_INC_BASE_DIR  += $(ROOT_DIR)/aw_pack_src/lib_aw/include/eyesee-mpp/middleware/include/media
USER_INC_BASE_DIR  += $(ROOT_DIR)/aw_pack_src/lib_aw/include/eyesee-mpp/middleware/include
USER_INC_BASE_DIR  += $(ROOT_DIR)/aw_pack_src/lib_aw/include/eyesee-mpp/middleware/media/include
USER_INC_BASE_DIR  += $(ROOT_DIR)/aw_pack_src/lib_aw/include/eyesee-mpp/middleware/media/include/component
USER_INC_BASE_DIR  += $(ROOT_DIR)/aw_pack_src/lib_aw/include/eyesee-mpp/middleware/media/LIBRARY/libisp/include
USER_INC_BASE_DIR  += $(ROOT_DIR)/aw_pack_src/lib_aw/include/eyesee-mpp/middleware/media/LIBRARY/libisp/isp_dev
USER_INC_BASE_DIR  += $(ROOT_DIR)/aw_pack_src/lib_aw/include/eyesee-mpp/middleware/media/LIBRARY/libisp/include/V4l2Camera
USER_INC_BASE_DIR  += $(ROOT_DIR)/aw_pack_src/lib_aw/include/eyesee-mpp/middleware/media/LIBRARY/libisp/isp_tuning
USER_INC_BASE_DIR  += $(ROOT_DIR)/aw_pack_src/lib_aw/include/eyesee-mpp/middleware/media/LIBRARY/include_stream
USER_INC_BASE_DIR  += $(ROOT_DIR)/aw_pack_src/lib_aw/include/eyesee-mpp/middleware/media/LIBRARY/include_FsWriter
USER_INC_BASE_DIR  += $(ROOT_DIR)/aw_pack_src/lib_aw/include/eyesee-mpp/middleware/media/LIBRARY/libcedarc/include
USER_INC_BASE_DIR  += $(ROOT_DIR)/aw_pack_src/lib_aw/include/eyesee-mpp/middleware/media/LIBRARY/include_muxer
USER_INC_BASE_DIR  += $(ROOT_DIR)/aw_pack_src/lib_aw/include/eyesee-mpp/middleware/media/LIBRARY/libResample/include
USER_INC_BASE_DIR  += $(ROOT_DIR)/aw_pack_src/lib_aw/include/eyesee-mpp/middleware/media/LIBRARY/libcedarx/libcore/common/iniparser
USER_INC_BASE_DIR  += $(ROOT_DIR)/aw_pack_src/lib_aw/include/eyesee-mpp/middleware/sample/configfileparser/
USER_INC_BASE_DIR  += $(ROOT_DIR)/aw_pack_src/lib_aw/include/eyesee-mpp/middleware/media/LIBRARY/agc_lib/include
USER_INC_BASE_DIR  += $(ROOT_DIR)/aw_pack_src/lib_aw/include/eyesee-mpp/system/public/include


#user app 编译文件
USER_SRC_DIRS = $(shell find $(USER_SRC_BASE_DIR) -type d)
USER_SRCS += $(foreach dir, $(USER_SRC_DIRS), $(wildcard $(dir)/*.c)) 
USER_SRCS += $(foreach dir, $(USER_SRC_DIRS), $(wildcard $(dir)/*.cpp)) 
USER_SRCS += $(foreach dir, $(USER_SRC_DIRS), $(wildcard $(dir)/*.s)) 
USER_SRCS += $(foreach dir, $(USER_SRC_DIRS), $(wildcard $(dir)/*.S)) 
#user头文件
USER_INCS = $(addprefix -I ,  $(shell find $(USER_INC_BASE_DIR) -type d) )
#user的obj命令
USER_OBJS = $(addsuffix .o, $(basename  $(USER_SRCS) ) )
#user的实际obj地址
USER_OBJS_OUT =  $(subst $(ROOT_DIR),$(OUTPUT_DIR_OBJS), $(USER_OBJS))

CFLAGS = $(CCFLAGS) $(DEFINES) $(SDK_INCS) $(USER_INCS) $(AWCHIP_CFLAGS)

build_app: $(USER_OBJS)
	@mkdir -p $(OUTPUT_DIR)
	$(XX)  $(USER_OBJS_OUT) $(CFLAGS)  $(LINKFLAGS) $(mpp_lib) -o $(OUTPUT_DIR)/$(APP_BIN_NAME) 
	@echo "Build APP Finish"
	@cp $(OUTPUT_DIR)/$(APP_BIN_NAME) $(OUTPUT_DIR)/$(APP_BIN_NAME)_strip
	$(STRIP) $(OUTPUT_DIR)/$(APP_BIN_NAME)_strip

%.o: %.c
	@mkdir -p $(dir $(subst $(ROOT_DIR),$(OUTPUT_DIR_OBJS), $@)); 
	$(CC) $(CFLAGS) -o  $(subst $(ROOT_DIR),$(OUTPUT_DIR_OBJS), $@)   -c $<

%.o: %.cpp
	@mkdir -p $(dir $(subst $(ROOT_DIR),$(OUTPUT_DIR_OBJS), $@)); 
	$(XX) $(CFLAGS) -o $(subst $(ROOT_DIR),$(OUTPUT_DIR_OBJS), $@)   -c $<

%.o: %.s
	@mkdir -p $(dir $(subst $(ROOT_DIR),$(OUTPUT_DIR_OBJS), $@)); 
	$(CC) $(CFLAGS) -o $(subst $(ROOT_DIR),$(OUTPUT_DIR_OBJS), $@)  -c $<

%.o: %.S
	@mkdir -p $(dir $(subst $(ROOT_DIR),$(OUTPUT_DIR_OBJS), $@)); 
	$(CC) $(CFLAGS) -D__ASSEMBLER__ -o $(subst $(ROOT_DIR),$(OUTPUT_DIR_OBJS), $@)  -c $<


.PHONY:clean SHOWARGS
clean:
	rm -rf $(OUTPUT_DIR)

