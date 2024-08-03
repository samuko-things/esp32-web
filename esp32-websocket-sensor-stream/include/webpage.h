const char *WEB_PAGE PROGMEM = R"====(
<!DOCTYPE html>
<html lang="en">

<head>
  <meta charset="UTF-8" />
  <meta name="viewport" content="width=device-width, initial-scale=1.0" />
  <meta http-equiv="X-UA-Compatible" content="ie=edge" />
  <title>ESP32 SENSOR STREAM</title>
  <style>
      * {
        margin: 0;
        padding: 0;
        box-sizing: border-box;
      }

      body {
        font-family: Arial, Helvetica, sans-serif;
        line-height: 1.6;
        color: #333;
      }

      header {
        background: #f4f4f4;
        padding: 1rem;
        text-align: center;
      }

      main {
        margin: auto;
        width: 400px;
        overflow: auto;
        padding: 3rem 2rem;
      }

      .container {
        display: flex;
        flex-direction: column;
        row-gap: 20px;
        padding: 2rem 3rem;
        background: #f4f4f4;
      }

      .sensor-text {
        padding-left: 35px;
      }

      #sensorA-data {
        color: #ff5602
      }
      #sensorB-data {
        color: #037f49
      }
      #sensorC-data {
        color: #0274ff
      }

      .btn-led-container {
        align-self: stretch;
        display: flex;
        justify-content: center;
        align-items: center;
        column-gap: 20px;
        
        padding-left: 30px;
        padding-right: 30px;
      }

      .btn {
        flex-grow: 1;
        padding: 10px;
        border: 1px solid gray;
        border-radius: 10px;
        background: #cbf4fc;
      }
      .btn:hover {
        background: #c2e6f6;
      }

      .led {
        width: 30px;
        height: 30px;
        border-radius: 50%;
        padding: 20px;
      }
      .led-on {
        background: rgb(243, 57, 57);
      }
      .led-off {
        background: rgb(146, 136, 136);
      }
  </style>
</head>

<body>
  <header class="heading">
    <h3 >ESP32 SENSOR STREAM</h3>
    <h4>WebSocket Protocol</h4>
  </header>

  <main>
    <div class="container">
      <h4 class="sensor-text">SENSOR[A]: <b id="sensorA-data">10</b></h4>
      <h4 class="sensor-text">SENSOR[B]: <b id="sensorB-data">10</b></h4>
      <h4 class="sensor-text">SENSOR[C]: <b id="sensorC-data">10</b></h4>

      <div class="btn-led-container">
        <button class="btn" id="btn">Toggle LED</button>
        <div class="led led-off" id="led"></div>
      </div>
    </div>
  </main>

  <script type = "text/javascript">
    // https://www.w3schools.com/js/js_ajax_http.asp

    var btn = document.getElementById('btn')
    var led = document.getElementById('led')
    var sensorA = document.getElementById('sensorA-data');
    var sensorB = document.getElementById('sensorB-data');
    var sensorC = document.getElementById('sensorC-data');
    var ledState = 0;

    var webSocket = new WebSocket('ws://' + window.location.hostname + ':81/')

    webSocket.onopen = function(e){
      console.log("web sockect has started")
      getLedState();
      getSensorData();
    }

    webSocket.onmessage = function(e){
      const msg = e.data
      const jsonData = JSON.parse(msg);
      //console.log(msg)
      if (jsonData.action === "getLedState"){
        ledState = jsonData.value
        toggleLedDisplay()
      }
      else if (jsonData.action === "getSensorData"){
        displaySensorData(jsonData.value.sensorA, jsonData.value.sensorB, jsonData.value.sensorC)
      }    
    }

    // get sensor data
    function getSensorData(){
      webSocket.send("getSensorData");
    }

    function displaySensorData(valA, valB, valC){
      sensorA.textContent = `${valA}`
      sensorB.textContent = `${valB}`
      sensorC.textContent = `${valC}`
    }
    

    // toggle led display
    function toggleLedDisplay(){
      if(ledState === 1){
        led.classList.remove('led-off')
        led.classList.add('led-on')
      }
      else if(ledState === 0){
        led.classList.remove('led-on')
        led.classList.add('led-off')
      }
    }

    // get ledstate
    function getLedState(){
      webSocket.send("getLedState");
    }

    // toggleLed
    function toggleLed(){
      webSocket.send("toggleLED");
    }
    
    btn.addEventListener('click', (e) => {
      toggleLed()
    })

    setInterval(() => {
      getSensorData()
      getLedState();
    }, 100)

    
  </script>
</body>

</html>
)====";