#!/bin/sh

source send_cmd_pipe.sh

while true; do
    for nr in a b c d e f g h i j k l m n o p q r s t u v w x y z; do
        udisk="/dev/sd$nr"
        udiskp=$udisk"1"
        part=$udisk
    
        while true; do
            while true; do
                if [ -b "$udisk" ]; then
                    sleep 1
                    if [ -b "$udisk" ]; then
                        echo "udisk insert"
                        break;
                    fi
                else
                    sleep 1
                fi
            done
            
            if [ ! -d "/tmp/udisk" ]; then
                mkdir /tmp/udisk
            fi
            
            mount $udisk /tmp/udisk
            if [ $? -ne 0 ]; then
                mount $udiskp /tmp/udisk
                if [ $? -ne 0 ]; then
                    SEND_CMD_PIPE_FAIL $3
                    sleep 3
                    # goto for nr in ...
                    # detect next plugin, the devno will changed
                    continue 2
                else
                    part=$udiskp
                fi
            fi
    
            break
        done
    
        capacity=`df -h | grep $part | awk '{printf $2}'`
        echo "$part: $capacity"
        
        SEND_CMD_PIPE_OK_EX $3 $capacity
    
        while true; do
            if [ -b "$udisk" ]; then
                sleep 1
            else
                echo "udisk removed"
                break
            fi
        done
    done
done
