# Stochastic-Optimization-for-PID-Controller-Auto-Calibration

## Step 1 :

The first step is to compile (C++ code) qt ctrl to get the GUI executable.

`$ cd qt_ctrl/`
`$ export ROS_WS=~/catkin_ws`
`$ make gui`

## Step 2 : 

To fine tune PID parameters you run the python code PID.py after changing the links (link to qt_ctrl).
All optimisation parameters are in the PID code.

`$ python PID`

## Method Summary 

* The following diagram is an overview of our PID optimisation pipeline : 

<img src="https://drive.google.com/uc?export=view&id=1NSIk-A2qcj6v4MG4Udfv8v85ZD0RIh7R">

* The following diagram shows our models interactions :

<img src="https://drive.google.com/uc?export=view&id=1WD-Js69A6s2vcNITQyYhsaVFkaZgSLNP">

## Results :

* Manual Adjustment Results : 

<img src="https://drive.google.com/uc?export=view&id=1O4vRIfvhZLfuikX9qFsPb2ULNCSQc7Ue">

* Parameters Auto-Adjusted (obtained by genetic algorithm) :

<img src="https://drive.google.com/uc?export=view&id=1nmjK0ArkxAPzaW4GuvMwp06JAxRo1PTD">

