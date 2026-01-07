#include "macro.h"

// 전원 ON/OFF
void powerOn(aircon_t *aircon)
{
  // OFF -> ON
  // 기본값 냉방모드, 20도, 바람중간, 실외기 쿨
  if (aircon->state == MODE_OFF)
  {
    aircon->state = MODE_COOL;
    aircon->target_temp = 20;
    aircon->fan = FAN_MID;
    aircon->outdoor_unit = OUT_COOL;
  }
}

void powerOff(aircon_t *aircon)
{
  // ON -> OFF
  if (aircon->state != MODE_OFF)
  {
    aircon->state = MODE_OFF;
    aircon->outdoor_unit = OUT_OFF;
    aircon->target_temp = 20;
    aircon->fan = FAN_MID;
  }
}

// 무풍 ON/OFF
void calmOn(aircon_t *aircon)
{
  if (aircon->state == MODE_OFF) return;

  // 현재 COOL 이면 CALM COOL로
  if (aircon->state == MODE_COOL)
  {
    aircon->state = MODE_CALM_COOL;
    aircon->fan = FAN_CALM;
  }

  // 현재 DEHUM이면 CALM_DEHUM으로
  else if (aircon->state == MODE_DEHUM)
  {
    aircon->state = MODE_CALM_DEHUM;
    aircon->fan = FAN_MID;
  }
}

void calmOff(aircon_t *aircon)
{
  if (aircon->state == MODE_OFF) return;

  // 현재 CALM_COOL이면  COOL로 변경
  if (aircon->state == MODE_CALM_COOL)
  {
    aircon->state = MODE_COOL;
    aircon->fan = FAN_MID;
  }

  // 현재 CALM_DEHUM 이면 DEHUM 으로
  else if (aircon->state == MODE_CALM_DEHUM)
  {
    aircon->state = MODE_DEHUM;
    aircon->fan = FAN_CALM;
  }
}

// 모드 바꾸기
void changeMode(aircon_t *aircon)
{
  if (aircon->state == MODE_OFF) return;

  // 냉방 -> 제습: 실외기 COOL
  if (aircon->state == MODE_COOL)
  {
    aircon->state = MODE_DEHUM;
    aircon->outdoor_unit = OUT_COOL;
  }
  // 제습 -> 난방: 실외기 WARM
  else if (aircon->state == MODE_DEHUM)
  {
    aircon->state = MODE_WARM;
    aircon->outdoor_unit = OUT_WARM;
  }
  // 난방 -> 송풍: 실외기 OFF
  else if (aircon->state == MODE_WARM)
  {
    aircon->state = MODE_BLOW;
    aircon->outdoor_unit = OUT_OFF;
  }
  // 송풍 -> 냉방: 실외기 COOL
  else if (aircon->state == MODE_BLOW)
  {
    aircon->state = MODE_COOL;
    aircon->outdoor_unit = OUT_COOL;
  }
}

void changeModeTarget(aircon_t *aircon, mode_t mode)
{
  if (aircon->state == MODE_OFF) return;
  if (mode == MODE_DEHUM)
  {
    aircon->state = MODE_DEHUM;
    aircon->outdoor_unit = OUT_COOL;
  }
  // 난방: 실외기 WARM
  else if (mode == MODE_WARM)
  {
    aircon->state = MODE_WARM;
    aircon->outdoor_unit = OUT_WARM;
  }
  // 난방 -> 송풍: 실외기 OFF
  else if (mode == MODE_BLOW)
  {
    aircon->state = MODE_BLOW;
    aircon->outdoor_unit = OUT_OFF;
  }
  // 송풍 -> 냉방: 실외기 COOL
  else if (mode == MODE_COOL)
  {
    aircon->state = MODE_COOL;
    aircon->outdoor_unit = OUT_COOL;
  }
}

// 온도 조절
void tempUp(aircon_t *air)
{
  if (air->state == MODE_OFF) return;
  // 30도 초과 금지
  if (air->target_temp < 30)
  {
    air->target_temp++;
  }
}
void tempDown(aircon_t *air)
{
  if (air->state == MODE_OFF) return;
  // 16도 미만 금지
  if (air->target_temp > 16)
  {
    air->target_temp--;
  }
}
void tempTarget(aircon_t *aircon, int target)
{
  if (aircon->state == MODE_OFF) return;

  if (target >= 16 && target <= 30)
  {
    aircon->target_temp = target;
  }
}

// 바람 세기 조절
void fanUp(aircon_t *air)
{
  if (air->state == MODE_OFF) return;

  if (air->fan < 4)
  {
    air->fan++;
  }
}
void fanDown(aircon_t *air)
{
  if (air->state == MODE_OFF) return;

  if (air->fan > 2)
  {
    air->fan--;
  }
}
void fanTarget(aircon_t *aircon, fanspeed_t target)
{
  if (aircon->state == MODE_OFF) return;
  if (target >= 0 && target <= 4)
  {
    aircon->fan = target;
  }
}