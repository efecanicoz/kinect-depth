# kinect-depth
  This program made for getting a depth image of environment from Microsoft Kinect's depth sensor. There is 3 source code in this project:
  
  `measure-depth-example` is example code to test your set up and see that everything works.
  
  `measure-depth` is orthogonal projetion of x-z plane y is fixed to middle of frame.
  
  `depth-array` just have a function that returns depth array.
  
# Installing 
  You must install https://github.com/OpenKinect/libfreenect first. By doing this you will install all the dependencies. 
  
# Known Issues
  Can not start audio device, thus can not use accelerometer and motor.
  
  Can not generate a depth image of surrounding area, just where camera looks at.
  
