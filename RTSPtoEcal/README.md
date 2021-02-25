
# opencv, should install opencv before building cv2ecal
    # download
        download opencv 4.0.1.  URL:https://github.com/opencv/opencv/tree/4.0.1
        download opencv 4.0.1   URL:https://github.com/opencv/opencv_contrib/tree/4.0.1
    # build
        untar those files
        mkdir opencv_build
        cd opencv_build
        cmake -DOPENCV_EXTRA_MODULES_PATH=../opencv_contrib-4.0.1/modules  ../opencv-4.0.1 -DBUILD_opencv_xfeatures2d=OFF -DBUILD_opencv_dnn_modern=OFF -DBUILD_opencv_cnn_3dobj=OFF 
        make -j6
        sudo make install





