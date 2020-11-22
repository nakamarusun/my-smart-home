from flask_mysqldb import MySQL
from flask import current_app
from flask.cli import with_appcontext
import click

mysql = None

def init_db(app):
    global mysql
    mysql = MySQL(app)
    # Buat instanstiasi mysqlnya
    # cursor = mysql.connection.cursor()

    # # Buat tablenya di database jika belum ada.
    # with app.open_resource("schema.sql", "rt") as file:
    #     for line in file.read().split(";"):

    #             string = line.strip().replace("\n", " ")
    #             cursor.execute(string)

@click.command("init-db")
@with_appcontext
def init_db_cli():
    # Initializes the database from the terminal.

    init_db(current_app)
    cursor = mysql.connection.cursor()

    # Buat tablenya di database jika belum ada.
    with current_app.open_resource("schema.sql", "rt") as file:
        for line in file.read().split(";")[:-1]:
                string = line.strip().replace("\n", " ")
                cursor.execute(string)

    click.echo("Done initializing the Database!")