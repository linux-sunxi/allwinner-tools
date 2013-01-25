#!/bin/bash
set -e

################################################################################
#
# Show how to build buildroot automatically -- Benn
#
################################################################################

echo -e '\033[0;31;1m#########################################\033[0m'
echo -e '\033[0;31;1m#  skip make buildroot for dragonboard  #\033[0m'
echo -e '\033[0;31;1m#########################################\033[0m'
CUR_DIR=`pwd`
export PATH=${CUR_DIR}/output/external-toolchain/bin:$PATH
echo $CUR_DIR
if [ ! -e output ]; then
mkdir  output
fi
if [ ! -e output/external-toolchain ];then
cd output
tar -jxf ../dl/arm-2010.09-50-arm-none-linux-gnueabi-i686-pc-linux-gnu.tar.bz2 
mv arm-2010.09 external-toolchain 
fi
