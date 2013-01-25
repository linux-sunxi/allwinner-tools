#!/bin/sh

source script_parser.sh

# insmod touchscreen driver
tp_module_path=`script_fetch "tp" "module_path"`
if [ -n "$tp_module_path" ]; then
    insmod "$tp_module_path"

    # calibrate touchscreen if need
    tp_type=`script_fetch "tp" "type"`
    if [ $tp_type -eq 0 ]; then
        while true; do
            ts_calibrate
            if [ $? -eq 0 ]; then
                break
            fi
        done
    fi

    #ts_test
else
    echo "NO!!! touchscreen driver to be insmod"
fi

# insmod ir driver
ir_activated=`script_fetch "ir" "activated"`
if [ $ir_activated -eq 1 ]; then
    ir_module_path=`script_fetch "ir" "module_path"`
    if [ -n "$ir_module_path" ]; then
        insmod "$ir_module_path"
    fi
fi

# start camera test firstly
while true; do
    camera_activated=`script_fetch "camera" "activated"`
    if [ $camera_activated -eq 1 ]; then
        module_count=`script_fetch "camera" "module_count"`
        if [ $module_count -gt 0 ]; then
            for i in $(seq $module_count); do
                key_name="module"$i"_path"
                module_path=`script_fetch "camera" "$key_name"`
                if [ -n "$module_path" ]; then
                    insmod "$module_path"
                    if [ $? -ne 0 ]; then
                        break 2
                    fi
                fi
            done
        fi
    fi

    # run cameratester, sleep 1 second wait for layer init
    # DO NOT remove 'sleep 1'
    cameratester &
    sleep 1
    break
done

# run dragonboard core process
core &
