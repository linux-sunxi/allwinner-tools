#!/bin/sh

source send_cmd_pipe.sh
source script_parser.sh

mkfs.vfat /dev/nanda
if [ $? -ne 0 ]; then
    SEND_CMD_PIPE_FAIL $3
    exit 1
fi
echo "create vfat file system for /dev/nanda done"

if [ ! -d "/tmp/nanda" ]; then
    mkdir /tmp/nanda
fi

mount /dev/nanda /tmp/nanda
if [ $? -ne 0 ]; then
    SEND_CMD_PIPE_FAIL $3
    exit 1
fi
echo "mount /dev/nanda to /tmp/nanda OK"

capacity=`df -h | grep '/dev/nanda' | awk '{printf $2}'`
echo "nand capacity: $capacity"
SEND_CMD_PIPE_MSG $3 $capacity

test_size=`script_fetch "nand" "test_size"`
if [ -z "$test_size" ]; then
    test_size=64
fi

echo "nand test read and write"
if [ $test_size > 0 ]; then
    nandrw "/tmp/nanda/test.bin" "$test_size"
else
    nandrw "/tmp/nanda/test.bin" 64
fi

if [ $? -ne 0 ]; then
    SEND_CMD_PIPE_FAIL $3
else
    SEND_CMD_PIPE_OK_EX $3 $capacity
fi
