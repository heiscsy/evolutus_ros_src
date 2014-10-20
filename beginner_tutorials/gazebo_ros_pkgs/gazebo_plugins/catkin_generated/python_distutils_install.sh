#!/bin/sh -x

if [ -n "$DESTDIR" ] ; then
    case $DESTDIR in
        /*) # ok
            ;;
        *)
            /bin/echo "DESTDIR argument must be absolute... "
            /bin/echo "otherwise python's distutils will bork things."
            exit 1
    esac
    DESTDIR_ARG="--root=$DESTDIR"
fi

cd "/home/yuhan/catkin_ws/src/gazebo_ros_pkgs/gazebo_plugins"

# Note that PYTHONPATH is pulled from the environment to support installing
# into one location when some dependencies were installed in another
# location, #123.
/usr/bin/env \
    PYTHONPATH="/usr/local/lib/python2.7/dist-packages:/home/yuhan/catkin_ws/src/beginner_tutorials/lib/python2.7/dist-packages:$PYTHONPATH" \
    CATKIN_BINARY_DIR="/home/yuhan/catkin_ws/src/beginner_tutorials" \
    "/usr/bin/python" \
    "/home/yuhan/catkin_ws/src/gazebo_ros_pkgs/gazebo_plugins/setup.py" \
    build --build-base "/home/yuhan/catkin_ws/src/beginner_tutorials/gazebo_ros_pkgs/gazebo_plugins" \
    install \
    $DESTDIR_ARG \
    --install-layout=deb --prefix="/usr/local" --install-scripts="/usr/local/bin"
