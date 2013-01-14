#!/bin/sh

source send_cmd_pipe.sh

nr="0"
mmcblk="/dev/mmcblk$nr"
mmcp=$mmcblk

while true; do
    while true; do
        while true; do
            if [ -b "$mmcblk" ]; then
                sleep 1
                if [ -b "$mmcblk" ]; then
                    echo "card$nr insert"
                    break
                fi
            else
                sleep 1
            fi
        done
        
        if [ ! -d "/tmp/extsd" ]; then
            mkdir /tmp/extsd
        fi
        
        mmcp=$mmcblk
        mount $mmcp /tmp/extsd
        if [ $? -ne 0 ]; then
            mmcp=$mmcblk"p1"
            mount $mmcp /tmp/extsd
            if [ $? -ne 0 ]; then
                SEND_CMD_PIPE_FAIL $3
                sleep 3
                continue 2
            fi
        fi

        break
    done
    
    capacity=`df -h | grep $mmcp | awk '{printf $2}'`
    echo "$mmcp: $capacity"
    
    umount /tmp/extsd
    
    SEND_CMD_PIPE_OK_EX $3 $capacity

    while true; do
        if [ -b "$mmcblk" ]; then
            sleep 1
        else
            echo "card$nr remove"
            break
        fi
    done
done
