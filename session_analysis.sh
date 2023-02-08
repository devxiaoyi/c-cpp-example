#!/bin/sh
# 安装 python3 并且 pip3 install matplotlib
# 注意：在 git bash 中运行
# [非必选] 将 ping ip 复制到 ping.txt

rm -f ./statistic_chart/session_log_data.txt
rm -f ./statistic_chart/ping_data.txt
mkdir -p ./statistic_chart
echo "Analysising session_log.txt ..."
cat session_log.txt | grep \"Latency\" | awk -F, '{print $6, $11}' | awk '{print $2, $4}' > ./statistic_chart/session_log_data.txt

if [ -f "ping.txt" ]; then
    echo "Analysising ping.txt ..."
    cat ping.txt | grep TTL | awk '{print $5}' | awk -F= '{print $2}' | awk -Fm '{print $1}' > ./statistic_chart/ping_data.txt
fi

pic_name=""
if [ "$1" == "" ]
    then pic_name="unamed"
else
    pic_name=$1
fi
echo "Drawing $pic_name chart ..."
python3 draw_session_statistic.py $pic_name
echo "Script is done, view in 'statistic_chart' folder"
