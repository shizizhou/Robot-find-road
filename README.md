# Robot-find-road
To run the program, please enter

gcc graphics.c FileName.c
./a.exe homepositionX homepositionY directionNumber | java -jar drawapp-3.0.jar

the position X,Y can be in range(100,700),with 50 interval
the direction can be 0,1,2,3, which 0 means south,1 means east,2 means north, 3 means west.


In this coursework, I successfully finished stage1 to stage5, and include the code of stage4 and stage5 in the zip.

My main solution was to keep the robot from going down the same path. If the program detect the path the robot went before, just change the direction(function: canMoveForward). And after the robot find a marker and return to the home, the program will clear its path.(function:returnHome)
In stage 5, in order to avoid that robot will stuck in one place, I added a function: restartHome. It means that if the robot cannot find a marker in 144 steps, it will return to the home and change its direction to search again. By doing this way, the robot can run through all of the map.
