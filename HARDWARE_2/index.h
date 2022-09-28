const char MAIN_page[] PROGMEM = R"=====(
<!DOCTYPE html>
<html>
<body>

 <div id="demo">
 <h1>ESP32 SCANNER Vol. 1.2 E</h1>

 <button type="button" onclick="sendData(11)" style="background: rgb(202, 60, 60);">Relay_1 ON..  </button>
 <button type="button" onclick="sendData(21)" style="background: rgb(202, 60, 60);">Relay_2 ON..  </button>
 <button type="button" onclick="sendData(31)" style="background: rgb(202, 60, 60);">Relay_3 ON..  </button>
 <button type="button" onclick="sendData(41)" style="background: rgb(202, 60, 60);">Relay_4 ON..  </button><br><br>
 
 <button type="button" onclick="sendData(10)" style="background: rgb(100,116,255);">Relay_1 OFF</button>
 <button type="button" onclick="sendData(20)" style="background: rgb(100,116,255);">Relay_2 OFF</button>
 <button type="button" onclick="sendData(30)" style="background: rgb(100,116,255);">Relay_3 OFF</button>
 <button type="button" onclick="sendData(40)" style="background: rgb(100,116,255);">Relay_4 OFF</button><br><br><br>
 
 <button type="button" onclick="sendData(51)" style="background: rgb(202, 60, 60);">LED1 ON..  </button>
 <button type="button" onclick="sendData(61)" style="background: rgb(202, 60, 60);">LED2 ON..  </button>
 <button type="button" onclick="sendData(71)" style="background: rgb(202, 60, 60);">BUZZER ON..  </button><br><br>
 
 <button type="button" onclick="sendData(50)" style="background: rgb(100,116,255);">LED1 OFF</button> 
 <button type="button" onclick="sendData(60)" style="background: rgb(100,116,255);">LED2 OFF</button>
 <button type="button" onclick="sendData(70)" style="background: rgb(100,116,255);">BUZZER OFF</button><br><br><br>
 
 State : <span id="LEDState">NA</span><br><br>
 </div>

 <div>
 <br>Data : <span id="ADCValue">0</span><br>
 </div>

 <script>
 function sendData(led) {
 var xhttp = new XMLHttpRequest();
 xhttp.onreadystatechange = function() {
 if (this.readyState == 4 && this.status == 200) {
 document.getElementById("LEDState").innerHTML =
 this.responseText;
 }
 };
 xhttp.open("GET", "setLED?LEDstate="+led, true);
 xhttp.send();
 }

 setInterval(function() {
 // Call a function repetatively with 2 Second interval
 getData();
 }, 200 ); //2000mSeconds update rate

 function getData() {
 var xhttp = new XMLHttpRequest();
 xhttp.onreadystatechange = function() {
 if (this.readyState == 4 && this.status == 200) {
 document.getElementById("ADCValue").innerHTML =
 this.responseText;

 }
 };
 xhttp.open("GET", "readDHTx", true);
 
 xhttp.send();
 }
 </script>

 <br><br><a href="https://www.google.com">Service</a>
 </div>


</body>
</html>
)====="; 
