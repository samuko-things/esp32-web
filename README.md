# esp32-web
collection of esp32 IOT webserver and websocket code implementation for streaming sensor data developed in `Platform IO`


## the webserver project
- the webserver project file uses the Asynchronous Web Server and AJAX (XMLHttp). the Client (i.e the webpage) makes http requests using the XMLHttpRequest() to communicate with the server to control the BUILTIN LED and stream sensor data.
- this method is easy but not as fast as that of the WebSocket.
- sensor data are transferred using JSON

## the WebSocket project
- the WebSocket project file uses the Web Server and WebSocket. The client (i.e. the webpage) uses a WebSocket to communicate with the server to control the BUILTIN LED and stream sensor data.
- this method is very very fast and good for streaming data especially when multiple clients are interacting with the server.
- sensor data are transferred using JSON

https://github.com/user-attachments/assets/bbdbf879-1e32-43bc-8e2d-fb4a9c91feb9

https://github.com/user-attachments/assets/803eb95b-867a-431b-a5e2-0dc130e551de

