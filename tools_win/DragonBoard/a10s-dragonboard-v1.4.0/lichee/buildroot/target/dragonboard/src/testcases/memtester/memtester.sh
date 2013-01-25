#!/bin/sh

source send_cmd_pipe.sh
source script_parser.sh

test_size=`script_fetch "dram" "test_size"`
if [ $test_size -lt 0 ]; then
    test_size=8
fi

memtester $test_size"M" 1 > /dev/null

if [ $? -ne 0 ]; then
    SEND_CMD_PIPE_FAIL $3
else
    SEND_CMD_PIPE_OK $3
fi
