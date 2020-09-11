#include <M5Stack.h>

#define TONE_FREQ 900
#define TONE_STOP 0
#define TONE_PLAY 1

static unsigned char gucVol = 0;
static unsigned char gucToneSts = 0;

void setup() {
    // M5Stackの初期化
    M5.begin();
    // Speaker初期化
    M5.Speaker.begin();

    // BLACK Screen
    M5.Lcd.fillScreen(BLACK);
    // 横線
    M5.Lcd.drawFastHLine(0, 220, 320, WHITE);
    // 縦線
    M5.Lcd.drawFastVLine(106, 220, 20, WHITE);
    M5.Lcd.drawFastVLine(219, 220, 20, WHITE);
    // 矢印
    M5.Lcd.fillTriangle(  49, 232,  53, 224,  57, 232,WHITE );
//    M5.Lcd.fillTriangle( 159, 224, 167, 228, 159, 232,WHITE );
    M5.Lcd.fillTriangle( 269, 224, 273, 232, 277, 224,WHITE );

    // テキストサイズ指定
    M5.Lcd.setTextSize(1);
    // START
    M5.Lcd.setCursor(140, 224);
    M5.Lcd.print("START");
    // ボリューム値
    gucVol = 4;
    // トーン状態
    gucToneSts = TONE_STOP;
}

void loop() {
    M5.update();

    // Aボタン
    if (M5.BtnA.wasPressed()) {
        // 再生中
        if( TONE_PLAY == gucToneSts) {
            // 音量UP
            if( gucVol < 9 ){
                gucVol++;
            } else {
                gucVol = 9;
            }
            SetVolume( gucVol );
        }
    }
    // Bボタン
    if (M5.BtnB.wasPressed()) {
        // 開始
        if( TONE_STOP == gucToneSts) {
            ledcSetup( TONE_PIN_CHANNEL, TONE_FREQ, 13);
            SetVolume( gucVol );
            gucToneSts = TONE_PLAY;
            // テキストサイズ指定
            M5.Lcd.setTextSize(1);
            // START
            M5.Lcd.fillRect(107, 221, 87, 19, BLACK);
            M5.Lcd.setCursor(140, 224);
            M5.Lcd.print("STOP");
        }
        // 停止
        else {
            // ミュート
            M5.Speaker.mute();
            gucToneSts = TONE_STOP;
            // テキストサイズ指定
            M5.Lcd.setTextSize(1);
            // START
            M5.Lcd.fillRect(107, 221, 87, 19, BLACK);
            M5.Lcd.setCursor(140, 224);
            M5.Lcd.print("START");
        }
    }
    // Cボタン
    if (M5.BtnC.wasPressed()) {
        // 再生中
        if( TONE_PLAY == gucToneSts) {
            // 音量DOWN
            if( gucVol > 0 ){
                gucVol--;
            } else {
                gucVol = 0;
            }
            SetVolume( gucVol );
        }
    }

    //音量
    // テキストサイズ指定
    M5.Lcd.setTextSize(2);
    // ENTER
    M5.Lcd.setCursor(100, 100);
    M5.Lcd.printf("VOLUME: %d", gucVol);

    delay(100);

    return;
}

static void SetVolume( unsigned char ucVol )
{
    ledcWrite( TONE_PIN_CHANNEL, 0x1FF>>(9-ucVol) );
    
    return;
}
