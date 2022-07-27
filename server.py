from datetime import datetime
from database import*
from flask import Flask

app = Flask(__name__)


@app.route("/api/noti", methods=['POST'])
def handler_notification():
    db = Database()
    d_t = datetime.now()
    db.insert_datas(800703982, d_t, d_t, "false")
    
    return "The door is opened!"


@app.route("/api/noti/new", methods=['POST'])
def handler_notification_new_finger_print():
    db = Database()
    d_ts = datetime.now()
    db.insert_noti_new_fingers(800703982, d_ts, d_ts, "false")

    return "The new finger is stored successfully"

if __name__ == "__main__":
    app.run(debug=True, host='0.0.0.0', port=8090)    