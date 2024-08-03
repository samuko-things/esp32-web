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
    <h4>Asynchronous Web Server</h4>
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

    // get sensor data
    function getSensorData(){
      const xhttp = new XMLHttpRequest();
      xhttp.onload = function() {
        const data = JSON.parse(this.responseText)
        sensorA.textContent = `${data.sensorA}`
        sensorB.textContent = `${data.sensorB}`
        sensorC.textContent = `${data.sensorC}`
        // console.log(this.responseText);
      }
      xhttp.open("GET", "/sensordata", true);
      xhttp.send();
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
      const xhttp = new XMLHttpRequest();
      xhttp.onreadystatechange = function() {
        if (this.readyState == 4 && this.status == 200) {
          ledState = parseInt(this.responseText);
          toggleLedDisplay();
          // console.log(ledState, this.responseText);
        }
      };
      xhttp.open("GET", "/ledstate", true);
      xhttp.send();
    }

    // toggleLed
    function toggleLed(state){
      const xhttp = new XMLHttpRequest();
      xhttp.onload = function() {
        ledState = parseInt(this.responseText);
        toggleLedDisplay();
        // console.log(ledState, this.responseText);
      }
      xhttp.open("GET", `/ledstate?state=${state}`, true);
      xhttp.send();
    }
    
    btn.addEventListener('click', (e) => {
    // console.log("led state", typeof ledState, ledState);
      if(ledState === 1){
        toggleLed(0);
      }
      else if(ledState === 0){
        toggleLed(1);
      }
    })


    getLedState();
    setInterval(() => {
      getSensorData()
    }, 500)

    
  </script>
</body>

</html>
)====";