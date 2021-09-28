# Ultrasonic-SLAM
Ultrasonic slam is a library that can be used for navigation of IoT-based or automated devices especially low processing power devices. It consists of minimal hardware and works great for indoor navigation.

## Why is Ultrasonic-SLAM needed?
Generally, SLAM is used for mapping and navigation of the environment. It is used in smart home robots, indoor cleaning robots or security robots, etc. It can be used where the importance of current position and environmental awareness is critical. But the SLAM requires high processing power not offered by ESP and Arduino devices. Also, SLAM works with lasers. Since the basic range finding lasers or LIDARs starts with $100. So it is beneficial from the financial view, too, to use a single ultrasonic sensor with a price of around $1 for mapping with Ultrasonic SLAM.

## Concept of the library
<img src="https://github.com/PatelVatsalB21/Ultrasonic-SLAM/blob/main/Images/UltraSLAM%20Intro.png"/>

The concept of mapping is displayed in the above figure. Here we consider a car with **L** and **W** dimensions for length and width. We also consider the speed of the car to be constant **S**. We map the room of dimensions **R<sub>L</sub>** and **R<sub>W</sub>**. Hence the room can be turned into a matrix of several blocks each representing area covered by the car. Matrix is made of **m** columns and **n** rows which can be calculated from shown formula.


## Working

### Moving Horizontally

<img src="https://github.com/PatelVatsalB21/Ultrasonic-SLAM/blob/main/Images/Moving_h.png"/>

Since we don't use encoders we cannot track how much the wheels travelled but we can access the uptime of the board(Arduino or ESP). Hence we can use the basic physics formula for distance, speed, and velocity. As we know the length of the car (L), we derived the time(T<sub>H</sub>) required to travel 1 block. We want to travel random distance x in the horizontal direction. Hence, we divided **x** by **L** to get the **number of blocks** and then multiplied with speed **S** to get the **time interval for which car should travel**.

### Moving Vertically

<img src="https://github.com/PatelVatsalB21/Ultrasonic-SLAM/blob/main/Images/Moving_v.png"/>

The math behind the vertical movement is the same as the horizontal except replacing **L** with **H**.

### Basics of Mapping

<img src="https://github.com/PatelVatsalB21/Ultrasonic-SLAM/blob/main/Images/Mapping_b.png"/>

Mapping of an environment is a complex mathematical work. We need to calculate the distance travelled by the car from wall to wall until the ultrasonic sensor detects the wall. As we do not know the distance travelled but by the time passed in travelling distance so, we can get the distance travelled as derived in the figure. The calculation is similar for vertical distance too.

### Mapping Algorithm

<img src="https://github.com/PatelVatsalB21/Ultrasonic-SLAM/blob/main/Images/Mapping.png"/>

A mapping algorithm is a code that maps an entire unknown room. It starts with the approach of the top right corner. Hence it starts mapping from the top right corner of the room and maps the room in iterations of the loop on a trajectory as shown in the figure. The map is saved in form of a 2D array on the board. The 2D array has n rows and 3 columns as the dimensions. Here n rows are taken for n horizontal iterations. Also, 3 columns are used to store the reading of each iteration respectively.


### Detailed Mapping Case

<img src="https://github.com/PatelVatsalB21/Ultrasonic-SLAM/blob/main/Images/Mapping_2.png"/>

Here we are using a test case to demonstrate the working of the mapping algorithm. As shown above, the room has obstacles on four corners and this test case covers most of the critical edge cases. The car starts mapping from the top right corner of the room and maps till the end of the room in 7 iterations. Here we use 3 columns in the array to store values from each iteration as l,b and r. As shown b is used to store regular horizontal reading while l and r are used for storing extra spaces in the left and right directions of each iteration if available. Hence l and r are 0 for iterations 1,2,6,7 while they are measurable for 3,4,5. 

### Result of Mapping

<img src="https://github.com/PatelVatsalB21/Ultrasonic-SLAM/blob/main/Images/Results_m.png"/>

After the mapping, all the readings are stored in the 2D array and can be visualized. On visualization shape of the usable area of the room can be known. These results can be stored on the EEPROM memory of the board for later extraction. Also, the map can be serially printed on USB or can be transmitted if WiFi or Bluetooth is available.

### Localisation

<img src="https://github.com/PatelVatsalB21/Ultrasonic-SLAM/blob/main/Images/Localisation.png"/>

After mapping, the generated map can be used to move through the open area. But as we are not using any compass or gyro sensor, localisation is a special code that takes care of the direction of the car and moves throughout the mapped area in any angle direction and distance while keeping track of its orientation and current position which is the key feature of the SLAM.

Localisation code finds the appropriate angle and distance of the destination location concerning its current orientation and position.

### Path tracing

<img src="https://github.com/PatelVatsalB21/Ultrasonic-SLAM/blob/main/Images/Path%20Tracing.png"/>

Path tracing is the mode used for following the same path used while mapping the room. This mode covers the entire area of the room and can be used in various cleaning robots or security robots. 

### Dynamic Obstacles

<img src="https://github.com/PatelVatsalB21/Ultrasonic-SLAM/blob/main/Images/Dynamic%20Obstacles.png"/>

Practically after mapping the room, we move some of our belongings and furniture but that would not be updated in the map until mapped the room again. Sometimes it might be even some temporary items for which it is not feasible to map the entire room again. But this might cause obstacles in the calculated trajectory of the car. To avoid this delta function is used. It calculates edge distance, angle to the edge, and obstacle length(L). It also bypasses the obstacle itself, then returns on the calculated path and updates the calculation and the target time covered in passing the obstacle.


## Circuit Details

### Requirements
- Arduino(any model, even nano works) or ESP or other Arduino compatible board
- Ultrasonic Sensor(hc-sr04 or any)
- Servo motor
- Arduino motor shield
- 4 DC Motors and wheels
- Basic car chassis
- Battery 



### Circuit Diagram
<img src="https://github.com/PatelVatsalB21/Ultrasonic-SLAM/blob/main/Images/Circuit%20Diagram.jpg"/>

## Steps to use the code
1. Download the file
2. Open Arduino IDE and select the port
3. Compile and Upload the file

## Uses of files

```UltraMap``` - It is used for initially mapping of the room.

```UltraNav``` - It is used for free navigation and moving throughout the map.

```UltraTrace``` - It is used for following the same path used while mapping.


## How to extract map (For Arduino only)
While using Arduino alone with the motors, there is no other logic board for processing. And the Arduino comes with limited memory and processing power. Due to this there is no channel for transmission of mapped matrix and also due to limited space it cannot be stored also on the board. There is an option to store map in the EEPROM memory of the board but the size of the map is the constraint. So the feasible solution is print the mapp array serially and copy it. 

A possible breakthrough is to add a bluetooth or wifi module to the arduino and transmit the map to the connected device.
