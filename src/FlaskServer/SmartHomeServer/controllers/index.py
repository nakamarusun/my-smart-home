from flask import current_app, render_template

def init_index(app):

    @app.route("/")
    def index():

        return render_template("index.html")