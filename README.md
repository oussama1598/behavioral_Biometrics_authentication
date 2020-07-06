Behavioral Biometrics Authentication
====================================

A project that utilizes Behavioral Biometrics for user authentication, in mobile devices

## Project Structure
### Data gatherer

This is a small application that utilizes different approaches to gather data from the device.

- ####Touch events
    I used adb to get the events directly from the touch screen drives. 
    Adb is the reason why I had to root the device. the parsed the data sent from the driver, which had a specific protocol.
    You can take a look at the multi touch events protocol in here https://www.kernel.org/doc/html/v4.18/input/multi-touch-protocol.html?fbclid=IwAR0BR7xXPjYubsk6em5Hyg2hF_i6cpENp6rMWBmUGboWJjZPknso1PMuqso
   
 - ####Keyboard strokes
    Keyboard events, was a bit tricky since it does not have any driver, it's on the software level,
    i couldn't use adb in this case, and the only solution that exists is to create a costume keyboard,
    this was way complex than what i could make, especially with android.
    I ended up using an existing open sourced keyboard, which i patched to send data back to the server which lives inside of the data gatherer.
    I uploaded the patched application here https://github.com/oussama1598/LokiBoard-Android-Keylogger
 
 - ####Accelerometer/Gyro events
   For events from those sensors i used again adb with the help of a monitoring application called senslogs(https://github.com/tyrex-team/senslogs).
   the application was used to keep the sensors running, since not doing that will result in no events coming from the sensors,
   the events retrieved used a again a specific protocol which had to be parsed, protocol reference is here https://www.kernel.org/doc/Documentation/input/event-codes.txt
   
 - ### Device orientation events
   Not implemented yet.
