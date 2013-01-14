#!/bin/sh

source send_cmd_pipe.sh
source script_parser.sh

module_path=`script_fetch "wifi" "module_path"`
module_args=`script_fetch "wifi" "module_args"`

if [ -z "$module_path" ]; then
    SEND_CMD_PIPE_FAIL $3
    exit 1
fi
flag="######"

echo "insmod $module_path $module_args"
insmod "$module_path" "$module_args"
if [ $? -ne 0 ]; then
    SEND_CMD_PIPE_FAIL $3
    exit 1
fi

for try in `seq 5`; do
    if ifconfig -a | grep wlan0; then
        # enable wlan0
        for i in `seq 3`; do
            ifconfig wlan0 up
            if [ $? -ne 0 -a $i -eq 3 ]; then
                echo "ifconfig wlan0 up failed, no more try"
                SEND_CMD_PIPE_FAIL $3
                exit 1
            fi
            if [ $? -ne 0 ]; then
                echo "ifconfig wlan0 up failed, try again 1s later"
                sleep 1
            else
                echo "ifconfig wlan0 up done"
                break
            fi
        done

        # find hot point
        while true; do
            for item in $(iwlist wlan0 scan | grep SSID); do
                item=`echo $item | awk -F: '{print $2}'`
                if [ -n "$item" ]; then
                    SEND_CMD_PIPE_OK_EX $3 $item
                fi
            done
	    SEND_CMD_PIPE_MSG $3 $flag 

            # update in 5s
            sleep 5
        done

        # disable wlan0
        ifconfig wlan0 down
        if [ $? -ne 0 ]; then
            SEND_CMD_PIPE_FAIL $3
            exit 1
        fi

        # test done
        SEND_CMD_PIPE_OK $3
        exit 0
    else
        echo "wlan0 not found, try again 1s later"
        sleep 1
    fi
done

# test failed
echo "wlan0 not found, no more try"
SEND_CMD_PIPE_FAIL $3
exit 1
