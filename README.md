# robot_follower

Goal: Project to learn real time systems and machine vision technologies. Creating a robot that can follow you around using a webcam, 3D printed chassis and wheels, motor, and Raspberry Pi 4. 

Webcam will be used for object detection (human). 3d Printing will be used to design chassis that webcam and wheels will be mounted to. Motor(s) - one will be used for rotating camera, and the other will be for drivetrain. Raspberry Pi 4 serves as central data processing from webcam input and converting to motor controls output. 

Phases:

Phase 1: Set up RPi and webcam to be able to detect a person in the camera image.

Phase 2: Add motor to rotate camera to follow person as they walk. Will need to create some sort of mounting to couple motor and webcam.

Phase 3: Assemble drive train and connect to rpi

Phase 4: Design and implement software for following human (following distance, speed, etc). May need to implement path finding algos around objects in home based on design (tbd). 

Phase 5: Put it all together

# Progress

Phase 1 (3/13): 
- I want to have all perception related tasks handled in this phase - understanding how the panning motor needs to move the webcam to keep the person in frame, distance between person and camera, and person movement direction.
- I originally had it set up where I used used the MobileNetSSD model to constantly track the person in the image and output the values accordingly however this used up a lot of CPU (80%).
- To minimize this, I reconfigured the code to detect a person using the model and then keep track of the person using Optical Flow Estimation. So the loop would be mobilenet to detect person first time around -> optical flow estimation -> repeat until number of features drops below threshold, reup with mobilenetssd. This helped with CPU usage a lot
- Next hurdle is distance estimation between camera and person. I'm using Monocular Depth Estimation as the robot will always be following me, however the distance outputs are extremely inaccurate. I need to add in a calibration process where I stand a known distance away from the camera and record the difference between the reading and actual to calculate some sort of correction factor.
- Another piece of this puzzle is I need the distance to output all the time, meaning for every frame, but when i switch to optical flow estimation the monocular depth estimation isn't applicable. So I'm trying to integrate a Kalman Filter to compensate. This is a WIP right now.
- Hoping to CAD the chassis soon. 
