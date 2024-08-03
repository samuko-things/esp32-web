# esp32-web
This is a collection two esp32 webserver projects which aims at showing how to use (as well as the difference between using) the HTTP and the WebSocket protocol for streaming multiple data and controlling LED from a client webpage using the ESP32.
Programming Environment Used: `Platform IO`

- The projects can be adapted to controlling any actuator (in place of the LED) e.g a motor, a solenoid, etc. and streaming actual data from multiple sensors (in place of the multiple randomly generated dummy data being streamed).
- The web client page was built using HTML, CSS, and Javascript and stored in the ESP32’s program memory.
- The `esp32-http-ajax-sensor-stream` uses the AJAX web technology (with the http protocol) to ensure that the whole page does not reload when data is sent from or received by the client webpage.
- The `esp32-websocket-sensor-stream` uses the WebSocket protocol which also ensures that the whole page does not reload when data is sent from or received by the client webpage
- The WebSocket outperformed the HTTP(with AJAX) in communication speed.
- Data were sent using JSON format.


https://github.com/user-attachments/assets/bbdbf879-1e32-43bc-8e2d-fb4a9c91feb9

https://github.com/user-attachments/assets/803eb95b-867a-431b-a5e2-0dc130e551de

