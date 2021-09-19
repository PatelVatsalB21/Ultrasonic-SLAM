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
