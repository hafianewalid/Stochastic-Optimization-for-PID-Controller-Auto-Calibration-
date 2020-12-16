# Controlling a ROS robot using Qt build

## ROS installation

This software requires several [http://www.ros.org](ROS) packages:
* ros-kinetic-desktop-full,
* ros-kinetic-qt-build,
* ros-kinetic-turtlebot,
* ros-kinetic-turtlebot-gazebo,
* ros-kinetic-rosdoc-lite.

The simplest way to install those is under
[http://ubuntu.com/](Ubuntu) (but also [http://xubuntu.com/](XUbuntu)
or [http://kubuntu.com/](KUbuntu)), using either
[https://help.ubuntu.com/community/AptGet/Howto](apt-get)
in a terminal or the GUI 
[https://help.ubuntu.com/community/SynapticHowto](Synaptic).

However, this will not work under 18.04 (or newer), as it only has
Melodic packages.
A Kinetic (or older) ROS installation is needed, as the application
uses both [http://wiki.ros.org/qt_build](qt-build) and
[http://wiki.ros.org/turtlebot](turtlebot), which are not available 
for Lunar and Melodic.

## ROS configuration

As any ROS package, this software requires the ROS environment
to be set up (/cf/ [http://wiki.ros.org/ROS/Tutorials/InstallingandConfiguringROSEnvironment](here)).
First, get ROS environment variables
with `source /opt/ros/kinetic/setup.bash`.
Then, create a ROS directory and configure it, using:
```
mkdir -p ROS/src
cd ROS
catkin_make
```

## Software compilation and execution

You should first copy the content of this directory in a `qt_ctrl`
sub-directory of the `ROS/src` directory.
You can then compile either by calling `catkin_make` in `ROS`
directory, or using `make` in `qt_ctrl` directory.

To start the software, use `GUI` script in `qt_ctrl` directory.

At first use, `gazebo` may have troubles.
As said [http://answers.gazebosim.org/question/4153/gazebo-crashes-immediately-using-roslaunch-after-installing-gazebo-ros-packages](here),
the solution is to call:
```
source ../../devel/setup.bash
killall gzserver
```

You can verify wether `gazebo` works using
`roslaunch qt_ctrl void_world.launch`.

When started, the software does nothing untill you ask to start
a controller (using the button).
This triggers a dialog box, which allows to
- cancel the controller's start,
- start the controller with an already started Gazebo/ROS
  (`No` button), or
- start both the controller and Gazebo/ROS (`Yes` button).

Would you start Gazebo/ROS, a dialog asks you which environment
to start: (empty = Yes, default = No).

Once the software is connected to Gazebo/ROS, if you want to track
a trajectory with a tracking controller (Yes), or generate one with
a tele-operation controller (No).

In the first case (tracking controller), the software will ask you
where is the trajectory's file.
In the second case (tele-operation controller), it will ask you
a file listing the commands (accelerations), or switch to manual
tele-operation (see TeleopWidget::react() documentation for shortcuts).

At last, the software will ask you wether to save the odometry data
into a file: giving a file save them in it, canceling disable the save.
This allows to generate trajectory's file for future tracking, or
to use an external software to compare both trajectories (tracked and
followed).

Once a tracking controller is started, the `historic` button pops up
a dialog displaying the tracked trajectory (in green) and the followed
one (in blue).

## ISeeML integration

In order to compile, `qt_ctrl` now uses `ISeeML` library.
Simply add a symbolic link to `ISeeML`'s main directory in `qt_ctrl`
directory.

## A note about Git 

Only a part of `qt_ctrl` directory should be saved into Git:
mainly `include` and  `src` sub-directories, as well as both `*.dox`
files.

A solution would be to store them in a special sub-directory
(/e.g./ `git`), only connect this one to Git, and to create
symbolic links from `qt_ctrl` directory to the elements of
this sub-directory.
