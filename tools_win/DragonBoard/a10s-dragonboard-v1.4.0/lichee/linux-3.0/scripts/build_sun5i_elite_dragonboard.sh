#!/bin/bash
set -e
#########################################################################
#
#          Simple build scripts to build krenel(with rootfs)  -- by Benn
#
#########################################################################


#Setup common variables
export ARCH=arm
export CROSS_COMPILE=arm-none-linux-gnueabi-
export AS=${CROSS_COMPILE}as
export LD=${CROSS_COMPILE}ld
export CC=${CROSS_COMPILE}gcc
export AR=${CROSS_COMPILE}ar
export NM=${CROSS_COMPILE}nm
export STRIP=${CROSS_COMPILE}strip
export OBJCOPY=${CROSS_COMPILE}objcopy
export OBJDUMP=${CROSS_COMPILE}objdump

KERNEL_VERSION="3.0"
LICHEE_KDIR=`pwd`
LICHEE_MOD_DIR==${LICHEE_KDIR}/output/lib/modules/${KERNEL_VERSION}

CONFIG_CHIP_ID=1125

update_kern_ver()
{
	if [ -r include/generated/utsrelease.h ]; then
		KERNEL_VERSION=`cat include/generated/utsrelease.h |awk -F\" '{print $2}'`
	fi
	LICHEE_MOD_DIR=${LICHEE_KDIR}/output/lib/modules/${KERNEL_VERSION}
}

show_help()
{
	printf "
Build script for Lichee platform

Invalid Options:

	help         - show this help
	kernel       - build kernel
	modules      - build kernel module in modules dir
	clean        - clean kernel and modules

"
}

build_standby()
{
	echo "build standby"
	make ARCH=${ARCH} CROSS_COMPILE=${CROSS_COMPILE} KDIR=${LICHEE_KDIR} \
		-C ${LICHEE_KDIR}/arch/arm/mach-sun5i/pm/standby all
}

CEDAR_ROOT=${LICHEE_KDIR}/modules/cedarx

build_cedarx_lib()
{
	echo "build cedarx library ${CEDAR_ROOT}/lib"
	if [ -d ${CEDAR_ROOT}/lib ]; then
		echo "build cedarx library"		
	make -C modules/cedarx/lib LICHEE_MOD_DIR=${LICHEE_MOD_DIR} LICHEE_KDIR=${LICHEE_KDIR} \
		CONFIG_CHIP_ID=${CONFIG_CHIP_ID} install
	else
		echo "build cedarx library"
	fi
}

NAND_ROOT=${LICHEE_KDIR}/modules/nand

build_nand_lib()
{
	echo "build nand library ${NAND_ROOT}/lib"
	if [ -d ${NAND_ROOT}/lib ]; then
		echo "build nand library"		
	make -C modules/nand/lib LICHEE_MOD_DIR=${LICHEE_MOD_DIR} LICHEE_KDIR=${LICHEE_KDIR} \
		CONFIG_CHIP_ID=${CONFIG_CHIP_ID} install
	else
		echo "build nand library"
	fi
}

copy_nand_mod()
{
    cd $LICHEE_KDIR/rootfs

    if [ -x "./build.sh" ]; then
        ./build.sh e sun5i_rootfs.cpio.gz
    else
        echo "No such file: $LICHEE_KDIR/rootfs/build.sh"
        exit 1
    fi

    if [ ! -d "./skel/lib/modules/$KERNEL_VERSION" ]; then
        mkdir -p ./skel/lib/modules/$KERNEL_VERSION
    fi
    cp $LICHEE_MOD_DIR/nand.ko ./skel/lib/modules/$KERNEL_VERSION
    if [ $? -ne 0 ]; then
        echo "copy nand module error: $?"
        exit 1
    fi
    if [ -f "./sun5i_rootfs.cpio.gz" ]; then
        rm sun5i_rootfs.cpio.gz
    fi
    ./build.sh c sun5i_rootfs.cpio.gz
    rm -rf skel

    cd $LICHEE_KDIR
}

build_kernel()
{
	if [ ! -e .config ]; then
		echo -e "\n\t\tUsing default config... ...!\n"
		cp arch/arm/configs/sun5i_elite_defconfig .config
	fi

	build_standby
	make ARCH=${ARCH} CROSS_COMPILE=${CROSS_COMPILE} -j8 uImage modules

	update_kern_ver

	if [ -d output ]; then
		rm -rf output
	fi
	mkdir -p $LICHEE_MOD_DIR

	${OBJCOPY} -R .note.gnu.build-id -S -O binary vmlinux output/bImage
	cp -vf arch/arm/boot/[zu]Image output/
	cp .config output/
	cp rootfs/sun5i_rootfs.cpio.gz output/

	mkbootimg --kernel output/bImage \
			--ramdisk output/sun5i_rootfs.cpio.gz \
			--board 'sun5i' \
			--base 0x40000000 \
			-o output/boot.img

	mkbootimg --kernel output/bImage \
			--ramdisk output/sun5i_rootfs.cpio.gz \
			--board 'sun5i' \
			--base 0x40000000 \
			-o output/boot.img

	for file in $(find drivers sound crypto block fs security net -name "*.ko"); do
		cp $file ${LICHEE_MOD_DIR}
	done
	cp -f Module.symvers ${LICHEE_MOD_DIR}

	#copy bcm4330 firmware and nvram.txt
	cp drivers/net/wireless/bcm4330/firmware/bcm4330.bin ${LICHEE_MOD_DIR}
	cp drivers/net/wireless/bcm4330/firmware/bcm4330.hcd ${LICHEE_MOD_DIR}
	cp drivers/net/wireless/bcm4330/firmware/nvram.txt ${LICHEE_MOD_DIR}/bcm4330_nvram.txt
	cp drivers/net/wireless/rtxx7x/RT2870STA.dat ${LICHEE_MOD_DIR}
	cp drivers/net/wireless/rtxx7x/RT2870STACard.dat ${LICHEE_MOD_DIR}
}

build_modules()
{
	echo "Building modules"

	if [ ! -f include/generated/utsrelease.h ]; then
		printf "Please build kernel first!\n"
		exit 1
	fi

	update_kern_ver

	make -C modules/example LICHEE_MOD_DIR=${LICHEE_MOD_DIR} LICHEE_KDIR=${LICHEE_KDIR} \
		CONFIG_CHIP_ID=${CONFIG_CHIP_ID} install

	(
	export LANG=en_US.UTF-8
	unset LANGUAGE
	make -C modules/mali LICHEE_MOD_DIR=${LICHEE_MOD_DIR} LICHEE_KDIR=${LICHEE_KDIR} \
		CONFIG_CHIP_ID=${CONFIG_CHIP_ID} install
	)

	#build usi-bmc4329 sdio wifi module
	make -C modules/wifi/usi-bcm4329/v4.218.248.15/open-src/src/dhd/linux \
			CROSS_COMPILE=${CROSS_COMPILE} ARCH=arm LINUXVER=${KERNEL_VERSION} \
			LICHEE_MOD_DIR=${LICHEE_MOD_DIR} LINUXDIR=${LICHEE_KDIR} CONFIG_CHIP_ID=${CONFIG_CHIP_ID} \
			INSTALL_DIR=${LICHEE_MOD_DIR} dhd-cdc-sdmmc-gpl
	#build cedar driver
	#echo "build_cedarx_lib"
	#build_cedarx_lib
	
	#make -C modules/cedarx LICHEE_MOD_DIR=${LICHEE_MOD_DIR} LICHEE_KDIR=${LICHEE_KDIR} \
	#	CONFIG_CHIP_ID=${CONFIG_CHIP_ID} install
	
	#build nand driver
	echo "build_nand_lib"
	build_nand_lib
	
	make -C modules/nand LICHEE_MOD_DIR=${LICHEE_MOD_DIR} LICHEE_KDIR=${LICHEE_KDIR} \
		CONFIG_CHIP_ID=${CONFIG_CHIP_ID} install
	echo "build module end"

    echo "copy nand module"
    copy_nand_mod
}

build_ramfs()
{
    echo "build ramfs"
	cp rootfs/sun5i_rootfs.cpio.gz output/

	mkbootimg --kernel output/bImage \
			--ramdisk output/sun5i_rootfs.cpio.gz \
			--board 'sun5i' \
			--base 0x40000000 \
			-o output/boot.img
}

clean_kernel()
{
	make clean
	rm -rf output/*

        (
	export LANG=en_US.UTF-8
	unset LANGUAGE
	make -C modules/mali LICHEE_MOD_DIR=${LICHEE_MOD_DIR} LICHEE_KDIR=${LICHEE_KDIR} clean
	)

	#build usi-bmc4329 sdio wifi module
	make -C modules/wifi/usi-bcm4329/v4.218.248.15/open-src/src/dhd/linux \
			CROSS_COMPILE=${CROSS_COMPILE} ARCH=arm LINUXVER=${KERNEL_VERSION} \
			LICHEE_MOD_DIR=${LICHEE_MOD_DIR} LINUXDIR=${LICHEE_KDIR} CONFIG_CHIP_ID=${CONFIG_CHIP_ID} \
			INSTALL_DIR=${LICHEE_MOD_DIR} clean
}

clean_modules()
{
	echo "Cleaning modules"
	make -C modules/example LICHEE_MOD_DIR=${LICHEE_MOD_DIR} LICHEE_KDIR=${LICHEE_KDIR} clean
}

#####################################################################
#
#                      Main Runtine
#
#####################################################################

LICHEE_ROOT=`(cd ${LICHEE_KDIR}/..; pwd)`
export PATH=${LICHEE_ROOT}/buildroot/output/external-toolchain/bin:${LICHEE_ROOT}/tools/pack/pctools/linux/android:$PATH

case "$1" in
kernel)
	build_kernel
	;;
modules)
	build_modules
	;;
clean)
	clean_kernel
	clean_modules
	;;
all)
	build_kernel
	build_modules
        build_ramfs
	;;
*)
	show_help
	;;
esac

