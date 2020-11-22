from flask import Blueprint, request
from werkzeug import Response
from SmartHomeServer import db
from time import time, strftime
from math import floor
import datetime
import json

bp = Blueprint("data", __name__, url_prefix="/data")

@bp.route("/")
def all_data():
    curTime = int(time())
    cursor = db.mysql.connection.cursor()

    # Berapa banyak sample yang kita ambil.
    samples = 25

    # Dapatkan argumen dari HTTP getnya.
    min_range = request.args.get("mn_rg", "")
    max_range = request.args.get("mx_rg", "")

    # Ubah jadi format integer. Jika argument yang didapatkan
    # Bukan angka, berarti buat jadi default (1 hari)
    min_range = int(min_range) if min_range.isnumeric() else curTime - 86400
    max_range = int(max_range) if max_range.isnumeric() else curTime

    str_min = datetime.datetime.fromtimestamp(min_range).strftime('%Y-%m-%d %H:%M:%S')
    str_max = datetime.datetime.fromtimestamp(max_range).strftime('%Y-%m-%d %H:%M:%S')

    # Dapatkan berapa banyak data yang ada di kurung waktu yang diberikan.
    cursor.execute("SELECT COUNT(*) FROM weather WHERE get_time BETWEEN \"{}\" AND \"{}\";".format(str_min, str_max))
    data_count = cursor.fetchall()[0][0]

    # Format jsonnya
    data_dict = {
        "get_time": [],
        "temp": [],
        "hindex": [],
        "humid": [],
        "pressure": [],
        "light": [],
        "rain": []
    }

    # Dapatkan <samples> data dari server mysql dari kurung waktu yang diberikan.
    # Jika lebih sedikit, dapatkan semuanya.
    cursor.execute("SET @ctr = 0;")
    cursor.execute("SELECT * FROM weather WHERE get_time BETWEEN \"{}\" AND \"{}\";".format(str_min, str_max))

    inc = data_count / samples if data_count > samples else 1
    i = 0
    raw_data = cursor.fetchall()
    while (i < data_count):

        data = raw_data[floor(i)]
        # Masukkan semua data ke dictionary
        data_dict["get_time"].append(data[0].timestamp())
        data_dict["temp"].append(data[1])
        data_dict["hindex"].append(data[2])
        data_dict["humid"].append(data[3])
        data_dict["pressure"].append(data[4])
        data_dict["light"].append(data[5])
        data_dict["rain"].append(data[6])

        i += inc
    
    return Response(json.dumps(data_dict), mimetype="application/json")