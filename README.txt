Dependencies: Qt, opencv

Run: tracker\release\tracker.exe

Using scenarios:

  For use web camera:
    1) Select camera number (0 - is default web camera)
    2) Click 'Run camera' button

  For use video file:
    1) Click 'Run video'
    2) Select video

  Video should start.

  For start tracking select object using mouse:
    1) Push down left mouse button on object corner
    2) Move mouse for select object
    3) Push up mouse button


Press ' ' for pause/start video.
Press 'Esc' for exit
Press 'r' for reset tracking and select new object.

Tracker contains in trackerCore/ folder. Another souses using for GUI, right frame generation, time counting...

Tracker api - trackerCore/trackerAPI. API description added in comments.