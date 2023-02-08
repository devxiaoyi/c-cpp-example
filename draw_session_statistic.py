import matplotlib.pyplot as plt
import numpy as np
import time
import sys
import os

session_file = './statistic_chart/session_log_data.txt'
ping_file = './statistic_chart/ping_data.txt'
dataArry = []
pingArry = []

if os.path.exists(session_file):
    dataArry = np.loadtxt(session_file)
if os.path.exists(ping_file):
    pingArry = np.loadtxt(ping_file)

# 行数
lineNum = 3
if (len(pingArry) > 0):
    lineNum = 4

recv = []
send = []
index = []
i = 0

for item in dataArry:
    recv.append(item[0])
    send.append(item[1])
    index.append(i)
    i += 1

# 散点图
plt.subplot(lineNum, 1, 1)
plt.title('latency_value', fontsize=10)
plt.scatter(index, send, s=1, label='send')
plt.scatter(index, recv, s=1, label='recv')

box = 3
# 饼图
titles = ['recv_latency_rate', 'send_latency_rate']
for classes in [recv,send]:
    zero2seven = 0
    seven2twelve = 0
    seven2twenty = 0
    others = 0
    for it in classes:
        if it <= 7:
            zero2seven+=1
        elif it <= 12:
            seven2twelve+=1
        elif it <= 20:
            seven2twenty+=1
        else:
            others+=1

    recv_size = len(classes)
    rateA = zero2seven / recv_size
    rateB = seven2twelve / recv_size
    rateC = seven2twenty / recv_size
    rateD = others / recv_size

    label = ['[0,7ms]', '(7ms,12ms]', '(12ms,20ms]', '(20ms,...)']
    explode = [0.1, 0, 0, 0]
    pieData = [rateA, rateB, rateC, rateD]
    plt.subplot(lineNum, 2, box)
    plt.pie(pieData, explode = explode, labels = label, autopct = '%1.1f', radius=1.5, textprops={'fontsize':10})
    plt.title(titles[box - 3], fontsize=10)
    box+=1

# 动态分布
fenbuTotalCount = 30
nor_titles = ['recv_normal_distribution', 'send_normal_distribution']
for classes in [recv,send]:
    fenbu = [0] * fenbuTotalCount
    for i in classes:
        if i >= fenbuTotalCount / 2:
            fenbu[fenbuTotalCount - 1]+=1
            continue
        for j in range(int(fenbuTotalCount / 2)):
            if i < j + 0.5:
                fenbu[j*2]+=1
                break
            elif i < j + 1:
                fenbu[j*2+1]+=1
                break

    plt.subplot(lineNum, 2, box)
    plt.title(nor_titles[box - 5], fontsize=10)
    xindex = []
    for i in range(15):
        xindex.append(i)
        xindex.append(i+0.5)
    plt.plot(xindex, fenbu, 'b.-', alpha=0.5, linewidth=1, label='X')
    box+=1

# ping
if (len(pingArry) > 0):
    plt.subplot(lineNum, 1, lineNum)
    plt.title('ping', fontsize=10)
    plt.plot(pingArry)

# save
# 设置子图间距
plt.subplots_adjust(None,None,None,None,None, 0.9)
#设置坐标刻度字体大小
plt.xticks(fontsize=10)
plt.yticks(fontsize=10)

picname = "./statistic_chart/" + sys.argv[1] + time.strftime('_%Y%m%d_%H%M%S.png', time.localtime())
print('Complete file: ' + picname)
plt.savefig(picname, dpi=500, bbox_inches = 'tight')
