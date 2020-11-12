const char conf_html[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html>
<head>
<title>Weather Station Configuration</title>
<link rel="stylesheet" href="/style">
</head>
<body>
<h1>MQTT Server: %CURRENT%</h1>
<form method="POST">
    <div class="icon">💻</div>
    <input type="text" name="mqtt" placeholder="MQTT Broker server">
    <input type="submit" value="Submit">
</form>
<h1>Update Time(s): %UPDATE_TIME%s</h1>
<form method="POST">
    <div class="icon">⌚</div>
    <input type="text" name="time" placeholder="Update every |X| second">
    <input type="submit" value="Submit">
</form>
</body>
</html>
)rawliteral";

const char style_html[] PROGMEM = R"rawliteral(
h1 {
    text-align: center;
    font-family: 'Courier New', Courier, monospace;
}
body {
    width: 600px;
    margin: auto;
    margin-top: 200px;
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
    background-color: rgb(152, 240, 211);
    border-radius: 5px;
    border-width: 0px;
    font-size: 12pt;
    padding: 0px 30px 0px 30px;
    transition: all 0.05s;
    margin-left: 5px;
}
input[type=submit]:hover {
    background-color:  rgb(137, 226, 211);
    cursor: pointer;
}
form .icon {
    font-size: 13pt;
    position: absolute;
    transform: translate(12px, 10px);
}
form {
    display: flex;
}
)rawliteral";