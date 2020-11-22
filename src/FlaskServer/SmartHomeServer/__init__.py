from flask import (Flask, render_template)
from flask_mysqldb import MySQL
from os import environ, makedirs, path
from SmartHomeServer.controllers import index, data_getter
from SmartHomeServer import uncache, db, mqtt

def makeDirsIfNaught(path):
    try:
        makedirs(path)
    except OSError:
        pass

def create_app():

    # Buat app utamanya
    app = Flask(__name__)

    # Ambil konfigurasi utama dari json.
    app.config.from_json(environ.get("FLASK_TEST_CONF", "conf.json"))
    app.config.from_mapping(SECRET_KEY="devsecret")

    # Mulai database
    db.init_db(app)
    app.cli.add_command(db.init_db_cli)

    # Mulai mqtt
    mqtt.init_mqtt(app)

    # Buat semua rutenya.
    index.init_index(app)

    # Buat aplikasi agar bisa diload tanpa cache.
    uncache.reg_static_uncache(app)

    # Bikin directory instances jika tidak ada
    makeDirsIfNaught(app.instance_path)
    makeDirsIfNaught(path.join(app.instance_path, "images"))

    # Daftar semua blueprintnya
    app.register_blueprint(data_getter.bp)

    return app