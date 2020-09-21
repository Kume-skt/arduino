# WioLTE

## 目次

- WioLTE とは
- センサーの差込口
- 電力周り
- 省電力化
- 参考ページ

## WioLTE とは

WioLTE では、SIM を使ったデータ通信が可能になる。
また、SD カードも積むことができるので log データを簡単に簡単にとることができる。

## センサーの差込口

- Uart × １
- I2c × １
- デジタル × ２
- アナログ × ２

## 電力周り

※ 個人での測定（センサー未使用）

- 通常時：100mAh
- 通信時：600mAh
- スリープ時：50mAh

動作電圧

- USB 給電：5v
- 電池給電：3.3v

USB 給電時には、3.3 ｖ、3.7 ｖでは、動作しなかったです。
5v での給電で動作しました。
１ A での動作もできますが、通信はできませんでした。

## 省電力化

WioLTE では、電源供給に悩まなければ考えなくでよいことです。
ですがたいていの場合、コンセントでの給電でなく電池などの電源を使用すると考えています。

```Arudino
wio.sleep()
```

一番簡単な省電力化が上記のコードです。
これで、50mAh まで消費電力を抑えることができます。

長期間動作させるとなるとさらに、抑える必要があります。ソフトウェア側からのアプローチになるとかなりめんどくさくなるので、Wio_extension_RTC というものを使用を考えます。

Wio_extension_RTC(以下 eRTC)は、I2C ポートを使用します。

### eRTC のモードが A と B があります。

- A:常時給電
- B:間隔給電

WioLTE がこの eRTC に対して何秒間寝ることを宣言します。eRTC は、その間 Wio への電源供給をやめます。なので、省電力性が格段に高くなります。

```Arudino
#include <WioRTC.h>

WioRTC Rtc;

～～～略～～～
void loop(){
    SerialUSB.println("sleep");

    //60秒間寝ます
    Rtc.SetWakeupPeriod(60);//sec

    //電源供給をしないでください
    Rtc.Shutdown();

    //loop関数だと電源が落ちる前に繰り返してしまうので足止めコード
    while (1){}
}
```

## 参考ぺージ

[WioLTE リファレンス](https://seeedjp.github.io/Wiki/Wio_LTE_for_Arduino/Reference-ja.html)
