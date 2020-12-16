import math
import pandas as pd
import os

import time

refpath='/home/walidone/catkin_ws/src/qt_ctrl/ref.csv'
respath='/home/walidone/catkin_ws/src/qt_ctrl/res.csv'
parampath='/home/walidone/catkin_ws/src/qt_ctrl/param.txt'
pathGUI='/home/walidone/catkin_ws/src/qt_ctrl/'

run_time=100

bestval=1000000000000
sol=[]

def load_txt(path):
    try:
        return pd.read_csv(path, sep='	', header=None ,dtype = {"x": "float64","y": "float64"})
    except:
        return []

def get_res_vector(path):
   L=load_txt(path)
   if(len(L)>0):
       x=L[1]
       x.pop(0)
       y=L[2]
       y.pop(0)
       x=[float(e)for e in x.tolist()]
       y=[float(e)for e in y.tolist()]
       return x,y
   else:
       return [1000000 for e in range(100)],[100000 for e in range(100)]

def save_PID_param(path,param):
    f = open(path, "w")
    for i in param:
      f.write(str(i)+"\n")
    f.close()

def run_GUI(path,t):
    cmd='cd '+path+' && export ROS_WS=~/catkin_ws && timeout '+str(t)+' ./GUI'
    print("av")
    os.system("gnome-terminal -e 'bash -c \"" + cmd + ";bash\"'")
    time.sleep(t+30)
    print("app")
    cmd='killall bash'
    os.system("gnome-terminal -e 'bash -c \"" + cmd + ";bash\"'")

def getVect(param):
    save_PID_param(parampath, param)
    run_GUI(pathGUI,run_time)
    return get_res_vector(respath)

def get_dist(v1,v2):
    d=0
    for i in range(max(len(v1[0]),len(v2[0]))):
         x1,x2,y1,y2=0,0,0,0
         if i<len(v1[0]):
             x1,y1=v1[0][i],v1[1][i]
         if i<len(v2[0]):
             x2,y2=v2[0][i],v2[1][i]
         d+=math.sqrt((x1-x2)**2+(y1-y2)**2)
    return d

ref=get_res_vector(refpath)

def param_eval(param):
    global bestval
    global sol
    hyp=getVect(param)
    v=get_dist(ref,hyp)
    if(v<bestval):
        bestval=v
        sol=param
        print("//////////// best /////////",param,bestval)
    return v
### test
'''
ps=[[1000,0,0,0,0,0],[9.4,3.6,8,25.3,4.4,25.8]]
for i in ps:
    print(param_eval(i))
'''

#print param_eval([ 1.68510835,  2.99389584, -3.18911341,  3.42182375,  0.01848806, 2.41676452])
