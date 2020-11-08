const char style_html[] PROGMEM = R"rawliteral(
body {
    width: 600px;
    margin: auto;
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
}
.head_button_container {
    display: flex;
    justify-content: center;
}
.bu {
    font-family: sans-serif;
    padding: 20px 20px 20px 20px;
    width: 130px;
    border-radius: 25px;
    margin: 10px;
    transition: filter 0.02s;
    cursor: pointer;
    user-select: none;
}
.bu:hover {
    filter: brightness(0.95);
}
.s_bu {
    width: 100px;
    height: 120px;
}
.lg {
    font-size: 48pt;
    text-align: center;
    margin-bottom: 10px;
}
.bu .cp {
    font-size: 12pt;
    text-align: left;
    padding-top: 10px;
    word-wrap: break-word;
}
.button_container {
    display: flex;
    flex-wrap: wrap;
    justify-content: center;
    max-width: 500px;
    margin: auto;
}
a {
    color: inherit;
    outline-width: 0px;
    text-decoration: none;
}
.large_circle_button {
    display: block;
    line-height: 300px !important;
    font-size: 20pt;
    font-family: sans-serif;
    text-align: center;
    background-color: #b4e5f8;
    border-radius: 50%;
    height: 300px;
    width: 300px;
    margin: auto;
    
}
.large_circle_button:hover {
    background-color: #a3ccdf;
    cursor: pointer;
}
.center_text {
    text-align: center;
}
input {
    width: 200px;
}
input[id="color"] {
    width: 50px;
}
input[type="submit"] {
    width: 100px;
}
.del_form {
    text-align: center;
}
)rawliteral";

const char button_html[] PROGMEM = R"rawliteral(
<a href="/remote/?id=%S" class="bu s_bu lg" style="background: %S;">%S<div class="cp">%S::%S</div></a>
)rawliteral";

const char add_button_html[] PROGMEM = R"rawliteral(
<!DOCTYPE HTML>
<head>
<title> Add a new Button </title>
<link rel="stylesheet" href="/style">
</head>
<body>
<div class="center_text">
<h1>Sensor IR terbuka!</h1>
<h2>Tekan remote untuk merekamnya di gadget.</h2>
<h2>Jika sudah, klik tombol dibawah</h2>
</div>
<a href="/remote/done" class="large_circle_button">sudah!</a>
</body>
)rawliteral";

const char add_button_done_html[] PROGMEM = R"rawliteral(
<!DOCTYPE HTML>
<head>
<title> Signal Received! </title>
<link rel="stylesheet" href="/style">
</head>
<body>
<div class="center_text">
<h1>Terrekam.</h1>
<h2>Data IR telah berhasil direkam dan diterima.</h2>
<h4>Data IR yang diterima:</h4>
<h4>%DATA%</h4>
</div>
<form id="form" method="POST" style="margin-top: 50px;">
    <h3>Button Caption: </h3><input id="caption" name="caption" type="text" class="input" placeholder="Caption" maxlength="20" autofocus/><p>
    <h3>Button Symbol(1 character only!): </h3><input id="symbol" name="symbol" type="text" placeholder="Symbol" maxlength="1"/><p>
    <h3>Color: </h3><input type="color" id="color" name="color" value="#aef2dd"><br><br><br>
    <input type="submit">
</form>
<h1 class="center_text">Result:</h1>
<div class="button_container">
<div id="b_btn" class="bu s_bu" style="background: #aef2dd;"><div id="b_sym" class="lg">@</div><div id="b_name" class="cp">Change the text above!</div></div>
</div>
<script>
var $=function(id){return document.getElementById(id);};
$("caption").onkeyup = function() {$('b_name').innerText = $('caption').value;}
$("symbol").onkeyup = function() {$('b_sym').innerText = $('symbol').value;}
$("color").addEventListener("change", function(){$('b_btn').style.background = $('color').value}, false);
</script>
</body>
)rawliteral";

const char index_html[] PROGMEM = R"rawliteral(
<!DOCTYPE HTML>
<head>
<title>IR Wi-Fi</title>
<link rel="stylesheet" href="/style">
</head>
<body>
<div class="center_text">
<h1>Wi-Fi IR</h1>
<h2>v1.0 Smarthome System</h2>
<h2>Laman ini bisa diakses dari: %IP%</h2>
<h2 id="but_res"></h2>
</div>
<div class="head_button_container">
    <a href="/remote/add" class="bu" style="background-color: #8be3f3;"><div class="lg">+</div><div class="cp">Add new Button</div></a>
    <div onclick="toggleDel()" class="bu" style="background-color: #FFF; border: 1px solid rgb(118, 28, 141);"><div class="lg">-</div><div class="cp">Delete a Button</div></div>
</div>
<form action="/remote/del" class="del_form" id="del" method="POST" style="display: none;">
    <h3>ID to delete: </h3>
    <input id="del_id" name="del_id" type="text" class="input" placeholder="ID" maxlength="5" autofocus/>
    <input type="submit" value="Enter">
</form>
<div style="width: 400px; border-bottom: 1px solid #AAA; margin: 20px auto 20px auto;"></div>
<div class="button_container">
    %REMOTES_LIST%
</div>
<script>
    function toggleDel() {
        var form = document.getElementById("del");
        if (form.style.display == "block") form.style.display = "none"; else form.style.display = "block";
    }
    function btn(num) {
        fetch("/remote/?id=" + num)
        .then(response => response.text())
        .then(html => document.getElementById("but_res").innerHTML = html);
    }
</script>
</body>
)rawliteral";