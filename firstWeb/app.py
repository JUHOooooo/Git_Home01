from flask import Flask, render_template, jsonify, request

app = Flask(__name__)

from pymongo import MongoClient

client = MongoClient('mongodb://test:test@localhost', 27017)
# client = MongoClient('localhost', 27017)
db = client.dbsparta


## HTML 화면 보여주기
@app.route('/')
def homework():
    return render_template('index.html')


# 주문하기(POST) API
@app.route('/memo', methods=['POST'])
def save_order():
    consumer_receive = request.form['consumer_give']
    count_receive = request.form['count_give']
    address_receive = request.form['address_give']
    phonenumber_receive = request.form['phonenumber_give']

    doc = {
        'consumer': consumer_receive,
        'count': count_receive,
        'address': address_receive,
        'phonenumber': phonenumber_receive
    }
    db.order.insert_one(doc)

    return jsonify({'msg': '주문완료!'})


# 주문 목록보기(Read) API
@app.route('/memo', methods=['GET'])
def view_orders():
    order = list(db.order.find({},{'_id':False}))
    return jsonify({'order':order})


if __name__ == '__main__':
    app.run('0.0.0.0', port=5000, debug=True)