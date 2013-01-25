#!/bin/sh
##############################################################################
# \version     1.0.0
# \date        2012年05月31日
# \author      James Deng <csjamesdeng@allwinnertech.com>
# \Descriptions:
#			create the inital version

# \version     1.1.0
# \date        2012年09月26日
# \author      Martin <zhengjiewen@allwinnertech.com>
# \Descriptions:
#			add some new features:
#			1.wifi hotpoint ssid and single strongth san
#			2.sort the hotpoint by single strongth quickly
##############################################################################
source send_cmd_pipe.sh
source script_parser.sh

WIFI_PIPE=/tmp/wifi_pipe
wlan_try=0

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

while true ; do
    if ifconfig -a | grep wlan0; then
        # enable wlan0
        for i in `seq 3`; do
            ifconfig wlan0 up > /dev/null
            if [ $? -ne 0 -a $i -eq 3 ]; then
                echo "ifconfig wlan0 up failed, no more try"
                SEND_CMD_PIPE_FAIL $3
                exit 1
            fi
            if [ $? -ne 0 ]; then
                echo "ifconfig wlan0 up failed, try again 1s later"
                sleep 1
            else
                #echo "ifconfig wlan0 up done"
                break
            fi
        done
        
		wifi=`iwlist wlan0 scan | awk -F"[:|=]" '(NF&&$1~/^[[:space:]]*ESSID/) \
										{printf "%s:",substr($2,2,length($2)-2)}\
								  	    (NF&&/[[:space:]]*Signal level/){printf "%d\n",$3 }'\
								  	    | sort -n -r -k 2 -t :`
			for item in $wifi ; do
				echo $item >> $WIFI_PIPE
				done
				
			echo $flag >> $WIFI_PIPE
            # update in 3s
            sleep 3

        # disable wlan0
        ifconfig wlan0 down
        if [ $? -ne 0 ]; then
       		echo $flag >> $WIFI_PIPE
            SEND_CMD_PIPE_FAIL $3
            exit 1
        fi

        # test done
        SEND_CMD_PIPE_OK $3
    else
        echo "wlan0 not found, try it again later"
        let "wlan_try=$wlan_try+1" 
        if [ $wlan_try -eq 3 ]; then
            break
        fi
        sleep 1
    fi
done

# test failed
echo "wlan0 not found, no more try"
SEND_CMD_PIPE_FAIL $3
exit 1
