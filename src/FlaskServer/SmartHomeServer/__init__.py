from flask import (Flask, render_template)
from os import environ, makedirs
from SmartHomeServer.controllers import index
from SmartHomeServer import uncache

def create_app():

    # Buat app utamanya
    app = Flask(__name__)

    # Ambil konfigurasi utama dari json.
    app.config.from_json(environ.get("FLASK_TEST_CONF", "conf.json"))
    app.config.from_mapping(SECRET_KEY="devsecret")

    # Buat semua rutenya.
    index.init_index(app)

    # Buat aplikasi agar bisa diload tanpa cache.
    uncache.reg_static_uncache(app)

    return app