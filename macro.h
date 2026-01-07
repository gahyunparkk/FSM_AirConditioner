#ifndef _MACRO_H_
#define _MACRO_H_

#define MAX_AIRCONS 4

typedef enum _mode_t
{
  MODE_OFF,
  MODE_COOL,
  MODE_CALM_COOL,
  MODE_DEHUM,
  MODE_CALM_DEHUM,
  MODE_WARM,
  MODE_BLOW
} mode_t;

typedef enum _fanspeed_t
{
  FAN_CALM,
  FAN_LOW,
  FAN_MID,
  FAN_HIGH,
  FAN_POWER
} fanspeed_t;

typedef enum _outdoor_t
{
  OUT_OFF,
  OUT_COOL,
  OUT_WARM
} outdoor_t;

typedef struct _aircon_t
{
  mode_t state;
  int target_temp;
  fanspeed_t fan;
  outdoor_t outdoor_unit;
} aircon_t;

#endif