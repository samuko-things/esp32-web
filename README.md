# esp32-web
collection of esp32 IOT webserver and websocket code implementation for streaming sensor data


## the webserver project
- the webserver project file uses the Asynchronous Web Server and AJAX (XMLHttp). the Client (i.e the webpage) makes http requests using the XMLHttpRequest() to communicate with the server to control the BUILTIN LED and stream sensor data.
- this method is easy but not has fast has that of the websocket.
- sensor data are transferred using JSON

## the websocket project
- the websocket project file uses the Web Server and websocket. the Client (i.e the webpage) uses websocket to communicate with the server to control the BUILTIN LED and stream sensor data.
- this method is very very fast and good for streaming data.
- sensor data are transferred using JSON
