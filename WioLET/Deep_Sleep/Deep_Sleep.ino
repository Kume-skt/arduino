#include <WioLTEforArduino.h>
#include <RTClock.h>
#include <stdio.h>
#include <time.h>
typedef int IRQn_Type;
#define __NVIC_PRIO_BITS          4
#define __Vendor_SysTickConfig    1
#include <libmaple/libmaple_types.h>
#include <libmaple/usbF4/VCP/core_cm4.h>
#include <libmaple/usbF4/VCP/core_cmInstr.h>
#include <libmaple/pwr.h>
#include <libmaple/bkp.h>

WioLTE Wio;
static RTClock rtc(RTCSEL_LSI);

void setup() {
  // put your setup code here, to run once:
  Wio.Init();
}

void loop() {
  // Standbyモードに入っているか確認
  if (PWR_BASE->CSR & (1UL << PWR_CSR_SBF)) {
    PWR_BASE->CSR |= (1UL << PWR_CR_CSBF);
  }

  SerialUSB.println("standby");
  enterStandbyMode(60);　// 60秒後に復帰する
}

void clearWUF(){
  // WakeUp Flagビットをクリア
  PWR_BASE->CR |= (1UL << PWR_CR_CWUF); 
}

// Standbyモードへ移行
void setBitStandbyMode() {
  delay(1000UL);

  // PDDS(Power-Down DeepSleep)ビットをセット
  PWR_BASE->CR |= (1UL << PWR_CR_PDDS);

  // WakeUp Flagビットをクリア
  clearWUF();

  // WakeUp Flagビットがクリアされるまで待つ
  while(PWR_BASE->CSR & (1UL << PWR_CSR_WUF)) {
  }

  // Cortex-M4にSLEEPDEEPビットマスクをかける
  SCB->SCR |= (SCB_SCR_SLEEPDEEP_Msk);

  // 割り込み待ち（Standbyモード）
  __WFI();
}

void enterStandbyMode(uint16 sleep_sec){
  rtc.setPeriodicWakeup(sleep_sec); // 待機時間(秒数)をセット

  clearWUF();

  setBitStandbyMode();  // Standbyモードに入る
}
