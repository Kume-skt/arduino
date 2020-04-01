#!usr/bin/env python
# -*- coding: utf-8 -*-

import paho.mqtt.client as mqtt     # MQTTのライブラリをインポート
import json

# ブローカーに接続できたときの処理


class Mqtt_code_sub():
    def __init__(self, syori, ip, port=1883, mqttKeepAlve=1000):
        self.syori = syori
        self.ipa = ip
        self.cliantport = port
        self.mkal = mqttKeepAlve

    def on_connect(self, client, userdata, flag, rc):
        print("Connected with result code " + str(rc))  # 接続できた旨表示
        client.subscribe("kumeta")  # subするトピックを設定

    # ブローカーが切断したときの処理

    def on_disconnect(self, client, userdata, flag, rc):
        if rc != 0:
            print("Unexpected disconnection.")

    # メッセージが届いたときの処理

    def on_message(self, client, userdata, msg):
        # msg.topicにトピック名が，msg.payloadに届いたデータ本体が入っている
        print("Received message '" + str(msg.payload) +
              "' on topic '" + msg.topic + "' with QoS " + str(msg.qos))
        # print(msg.payload.decode("utf-8"))
        data = json.loads(msg.payload.decode("utf-8"))
        self.syori(data)
        print("===========================================")

    def mqttsub(self):
        # MQTTの接続設定
        client = mqtt.Client()                 # クラスのインスタンス(実体)の作成
        client.on_connect = self.on_connect         # 接続時のコールバック関数を登録
        client.on_disconnect = self.on_disconnect   # 切断時のコールバックを登録
        client.on_message = self.on_message  # メッセージ到着時のコールバック
        print("fasdfafa")
        #                IP, PORT, MQTT_KEEP_ALIVE
        client.connect(self.ipa, self.cliantport, self.mkal)  # 接続先は自分自身

        client.loop_forever()  # 永久ループして待ち続ける


def testsyori(data):
    print(data)


if __name__ == "__main__":
    syori = testsyori
    ip = "192.168.12.5"
    port = 1883
    mkal = 1000

    test = Mqtt_code_sub(syori, ip)
    test.mqttsub()
