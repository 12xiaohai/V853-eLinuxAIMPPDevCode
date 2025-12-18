#!/bin/sh

#author by wuguanling

#get sdk_dir dir
cd `dirname $0`
sdk_dir=`pwd`
cd -

APP_BIN_NAME=sample

#这里不同硬件产品的型号，传递给makefile
PRODUCT_MODEL=perf1

toolchain_dir=${sdk_dir}/toolchain/toolchain-sunxi-musl/toolchain/bin
toolchain_name=arm-openwrt-linux-muslgnueabi-

make -C ${sdk_dir} APP_BIN_NAME=$APP_BIN_NAME PRODUCT_MODEL=$PRODUCT_MODEL COMPILE_PREX=${toolchain_dir}/${toolchain_name} build_app

if [ $? -eq 0 ]; then
	echo "===compile success, next will cp sample to rootfs and make AW image==="
	sleep 2
        ${sdk_dir}/mk_firmware/build_part.sh ${APP_BIN_NAME}
        if [ $? != 0 ]; then
    	echo "make partition erro"
    	return 2
        fi
fi


