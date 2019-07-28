# **Metrics for FRC Vision Processing** <br>
Library for FRC Vision Processing that calculates the distance (in inches) and angle the camera is from the vision target.

## **Documentation**
- ``` void TargetInit(Mat *img)``` <br>
This method pairs up the vision targets. It takes in a cv::Mat image that has the vision targets as white pixels. It performs a rough edge detection by iterating through the image horizontally and vertically, and it saves the start and end of a individual vision target by looking at spots where the pixel changes from black (non target) to white (target) and vise versa. It compares the target height and with ratio to the other targets in order to pair it.

- ```int getAngle()``` <br>
Returns the angle from the vision target. The angle is determined by the offset of the center of the closet vision target and the center of the frame converted to degrees.

- ``` double getDistance(int length);``` <br>
Returns the distance in inches to the vision target. The distance is determined by the length of the vision target in pixels from a linear regression function.

- ```int findClosetTarget();```
Returns the index of the closet target (target pair with largest length) from pair_list vector.

- ``` int findClosestTargetLength();```
 Returns the length (in pixels) of the closet target.
 
- ```void lineOfRegression()```
Uses line of regression to create a formula that converts the target pair length in pixels to distance from the camera in inches. 

- ```bool readMetrics(string filename)```
Reads config data from a file into the target_pair_length and distance vectors used for creating the line of regression. The first line of the file is the number of data.

- ```string getAngleAndDistance()```
Returns the angle and distance in a string formatted like "angle distance"







  
  

