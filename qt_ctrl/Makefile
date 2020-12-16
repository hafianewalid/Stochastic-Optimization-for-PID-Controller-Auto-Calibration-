#
# A. Scheuer: make file for Qt Control
#

.PHONY: doc gui bkp

all:	doc gui

doc:
	rosdoc_lite .

# bash needed, catkin_make uses colored characters
# does not work well under emacs (several colors per line)
gui:
	cd $(ROS_WS); catkin_make --pkg qt_ctrl #\
#			 -DCMAKE_BUILD_TYPE=Debug

clean:
	\rm -rf $(ROS_WS)/build/qt_ctrl;
	\rm -f $(ROS_WS)/devel/lib/qt_ctrl/gui

bkp:
	cd ..; tar jcf qt_ctrl/bkp/v3_2e.tbz --exclude html \
	--exclude '*~' --exclude 'moc_*' --exclude '*.o' \
	--exclude '*Dbg' --exclude '*debug*' --exclude bkp qt_ctrl

start:
	export ROS_WS=~/catkin_ws && ./GUI
