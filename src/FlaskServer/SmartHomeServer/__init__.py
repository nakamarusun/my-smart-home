from flask import (Flask, render_template)
from os import environ, makedirs

def create_app():

    # Create the main app
    app = Flask(__name__)

    # Load config from a json file
    app.config.from_json(environ.get("FLASK_TEST_CONF", "conf.json"))
    app.config.from_mapping(SECRET_KEY="devsecret")

    return app