from flask import current_app, render_template
from SmartHomeServer import db

def init_index(app):

    @app.route("/")
    def index():
        return render_template("index.html", image_src=app.config["CAPTURE_PATH"])