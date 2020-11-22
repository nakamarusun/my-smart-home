from flask import Blueprint
from SmartHomeServer import db

bp = Blueprint("data", __name__, url_prefix="/data")

@bp.route("/")
def all_data():
    return "mibbu"