# fetch data from script
# \param $1 section name
# \param $2 key name
# 
# example:
#   var=`script_fetch mmc display_name`
script_fetch()
{
    filp="/boot/test_config.fex"
    section=$1
    key=$2

    item=`awk -F '=' '/\['"$section"'\]/{a=1}a==1&&$1~/'"$key"'/{gsub(/[[:blank:]]*/,"",$0); print $0; exit}' $filp`
    value=${item#*=}
    start=${value:0:7}
    if [ "$start" = "string:" ]; then
        retval=${value#*string:}
    else
        start=${value:0:1}
        if [ "$start" = "\"" ]; then
            retval=${value#*\"}
            retval=${retval%\"*}
        else
            retval=$value
        fi
    fi
    echo $retval
}
