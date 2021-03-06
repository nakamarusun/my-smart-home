const char index_html[] = R"rawliteral(
<!DOCTYPE HTML>
<head>
<title>Smart Doorbell</title>
<style>
    body {
        min-width: 400px;
        max-width: 800px;
        margin: auto;
        margin-bottom: 300px;
    }
    .c_text {
        text-align: center;
    }
    h1 {
        font-family: Arial, Helvetica, sans-serif;
        margin-bottom: 0px;
    }
    h2 {
        font-family: Arial, Helvetica, sans-serif;
        font-weight: normal;
        font-size: large;
        margin-top: 0px;
        color: #464646;
    }
    h3 {
        font-family: sans-serif;
        font-weight: normal;
        font-size: medium;
        margin: 5px 0px 5px 0px;
    }
    h4 {
        font-family: 'Courier New', Courier, monospace;
        font-weight: normal;
        margin: 5px 0px 5px 0px;
    }
    img {
        width: 100%;
        height: auto;
        object-fit: cover;
    }
    input[type=text] {
        width: 100%;
        box-sizing: border-box;
        border: 2px solid #ccc;
        border-radius: 5px;
        font-size: 12pt;
        background-color: white;
        padding: 12px 20px 12px 40px;
    }
    input[type=submit] {
        background-color: rgb(234, 153, 245);
        border-radius: 5px;
        border-width: 0px;
        font-size: 12pt;
        padding: 0px 30px 0px 30px;
        transition: all 0.05s;
        margin-left: 5px;
    }
    input[type=submit]:hover {
        background-color:  rgb(184, 115, 224);
        cursor: pointer;
    }
    form .icon {
        font-size: 15pt;
        position: absolute;
        transform: translate(14px, 11px);
        font-weight: bold;
    }
    form {
        display: flex;
    }
</style>
</head>
<h1 class="c_text">Smart Doorbell</h1>
<h2 class="c_text">v1.0 Smarthome System</h2>
<br>
<img id="cap_img" src="/capture" alt="Image">
<h4 class="c_text" id="date_t">date</h4>
<div style="display: flex; justify-content: center;">
<input style="background-color: #81dde4; padding: 10px;" type="submit" value="Refresh" onclick="updateImage()">
</div>
<div style="margin-bottom: 70px;"></div>
<h2>Current MQTT Server: </h2>
<h4 id="broker_host" style="margin-bottom: 20px;">blabla</h4>
<div style="display: flex;"><h2>MQTT Status: </h2><h2 style="margin-left: 20px; color: #33afb8; cursor: pointer" onclick="updateStatus()">Refresh</h2></div>
<h4 style="margin-bottom: 30px;" id="broker_status"></h4>
<form method="POST">
    <div class="icon">></div>
    <input type="text" name="mqtt" placeholder="New MQTT Broker Server">
    <input type="submit" value="Submit">
</form>
<form method="POST" style="margin-top: 50px">
    <div class="icon">></div>
    <input type="text" name="text" placeholder="Enter display text">
    <input type="submit" value="Submit">
</form>
<script>
    var $=function(id){return document.getElementById(id);};
    $("date_t").innerHTML = new Date();
    function updateStatus() {
        fetch("/brokerstatus")
        .then(response => response.text())
        .then(html => $("broker_status").innerHTML = html);
    }
    function updateImage() {
        $("cap_img").src = "/capture?time=" + new Date().getTime();
        $("date_t").innerHTML = new Date();
    }
    updateStatus()
    fetch("/brokerhost")
        .then(response => response.text())
        .then(html => $("broker_host").innerHTML = html);
</script>
</body>
)rawliteral";