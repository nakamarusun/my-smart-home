from flask_mqtt import Mqtt
from SmartHomeServer import db
import json
from datetime import datetime
import urllib.request
from time import time
from os import path
from flask import current_app

mqtt = Mqtt()

def handle_photo(url_path, epoch):
    save_path = path.join(current_app.instance_path, "images", str(epoch) + ".jpg")
    urllib.request.urlretrieve(url_path, save_path)

def init_mqtt(app):

    # Buat semua objek mqtt, dan subscribe ke topik yang kita ingin.
    mqtt.init_app(app)
    
    @mqtt.on_connect()
    def handle_connect(client, userdata, flags, rc):
        # Subscribe ke semua yang dibutuhkan
        mqtt.subscribe(app.config["TOPIC_WEATHER"])
        mqtt.subscribe(app.config["TOPIC_PROXIMITY"])
        mqtt.subscribe(app.config["TOPIC_DOORBELL"])

    @mqtt.on_message()
    def handle_mqtt_message(client, userdata, message):

        with app.app_context():
            # Kita aproses semua messagenya.
            topic = message.topic
            payload = message.payload.decode()

            # Dapatkan waktu sekarang dalam bentuk string
            now_epoch = int(time())
            now = str(datetime.fromtimestamp(now_epoch).strftime('%Y-%m-%d %H:%M:%S'))
            
            # Masukkan ke dalam database.
            cursor = db.mysql.connection.cursor()
            if topic == app.config["TOPIC_WEATHER"]:
                
                data = json.loads(payload)
                cursor.execute("INSERT INTO weather VALUES (%s, %s, %s, %s, %s, %s, %s)", (
                    now,
                    data["temp"],
                    data["hindex"],
                    data["humid"],
                    data["pressure"],
                    data["light"],
                    data["rain"]
                ))
                cursor.execute("COMMIT;")


            elif topic == app.config["TOPIC_PROXIMITY"]:
                cursor.execute("INSERT INTO door VALUES(%s, %s)", (
                    now,
                    "proximity"
                ))
                cursor.execute("COMMIT;")
                handle_photo(app.config["CAPTURE_PATH"], now_epoch)

            elif topic == app.config["TOPIC_DOORBELL"]:
                cursor.execute("INSERT INTO door VALUES(%s, %s)", (
                    now,
                    "doorbell"
                ))
                cursor.execute("COMMIT;")
                handle_photo(app.config["CAPTURE_PATH"], now_epoch)