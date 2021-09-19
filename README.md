# Ultrasonic-SLAM
Ultrasonic slam is a library that can be used for navigation of IoT-based or automated devices especially low processing power devices. It consists of minimal hardware and works great for indoor navigation.

## Why is Ultrasonic-SLAM needed?
Generally, SLAM is used for mapping and navigation of the environment. It is used in smart home robots, indoor cleaning robots or security robots, etc. It can be used where the importance of current position and environmental awareness is critical. But the SLAM requires high processing power not offered by ESP and Arduino devices. Also, SLAM works with lasers. Since the basic range finding lasers or LIDARs starts with $100. So it is beneficial from the financial view, too, to use a single ultrasonic sensor with a price of around $1 for mapping with Ultrasonic SLAM.

## Requirements
- Arduino(any model, even nano works) or ESP or other Arduino compatible board
- Ultrasonic Sensor(hc-sr04 or any)
- Servo motor
- Arduino motor shield
- 4 DC Motors and wheels
- Basic car chassis
- Battery 

## Concept of the library
<img src="https://github.com/PatelVatsalB21/Ultrasonic-SLAM/blob/main/UltraSLAM%20Intro.png"/>

The concept of mapping is displayed in the above figure. Here we consider a car with **L** and **W** dimensions for length and width. We also consider the speed of the car to be constant **S**. We map the room of dimensions **R<sub>L</sub>** and **R<sub>W</sub>**. Hence the room can be turned into a matrix of several blocks each representing area covered by the car. Matrix is made of **m** columns and **n** rows which can be calculated from shown formula.


## Working

### Moving Horizontally

<img src="https://github.com/PatelVatsalB21/Ultrasonic-SLAM/blob/main/Moving_h.png"/>

Since we don't use encoders we cannot track how much the wheels travelled but we can access the uptime of the board(Arduino or ESP). Hence we can use the basic physics formula for distance, speed, and velocity. As we know the length of the car (L), we derived the time(T<sub>H</sub>) required to travel 1 block. We want to travel random distance x in the horizontal direction. Hence, we divided **x** by **L** to get the **number of blocks** and then multiplied with speed **S** to get the **time interval for which car should travel**.

### Moving Horizontally

<img src="https://github.com/PatelVatsalB21/Ultrasonic-SLAM/blob/main/Moving_v.png"/>

The math behind the vertical movement is the same as the horizontal except replacing **L** with **H**.

### Basics of Mapping

<img src="https://github.com/PatelVatsalB21/Ultrasonic-SLAM/blob/main/Mapping_b.png"/>

Mapping of an environment is a complex mathematical work. We need to calculate the distance travelled by the car from wall to wall until the ultrasonic sensor detects the wall. As we do not know the distance travelled but by the time passed in travelling distance so, we can get the distance travelled as derived in the figure. The calculation is similar for vertical distance too.

### Mapping Algorithm

<img src="https://github.com/PatelVatsalB21/Ultrasonic-SLAM/blob/main/Mapping.png"/>

A mapping algorithm is a code that maps an entire unknown room. It starts with the approach of the top right corner. Hence it starts mapping from the top right corner of the room and maps the room in iterations of the loop on a trajectory as shown in the figure. The map is saved in form of a 2D array on the board. The 2D array has n rows and 3 columns as the dimensions. Here n rows are taken for n horizontal iterations. Also, 3 columns are used to store the reading of each iteration respectively.


### Detailed Mapping Case

<img src="https://github.com/PatelVatsalB21/Ultrasonic-SLAM/blob/main/Mapping_2.png"/>

Here we are using a test case to demonstrate the working of the mapping algorithm. As shown above, the room has obstacles on four corners and this test case covers most of the critical edge cases. The car starts mapping from the top right corner of the room and maps till the end of the room in 7 iterations. Here we use 3 columns in the array to store values from each iteration as l,b and r. As shown b is used to store regular horizontal reading while l and r are used for storing extra spaces in the left and right directions of each iteration if available. Hence l and r are 0 for iterations 1,2,6,7 while they are measurable for 3,4,5. 

### Result of Mapping

<img src="https://github.com/PatelVatsalB21/Ultrasonic-SLAM/blob/main/Results_m.png"/>

After the mapping, all the readings are stored in the 2D array and can be visualized. On visualization shape of the usable area of the room can be known. These results can be stored on the EEPROM memory of the board for later extraction. Also, the map can be serially printed on USB or can be transmitted if WiFi or Bluetooth is available.

