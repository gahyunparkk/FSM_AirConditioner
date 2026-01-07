#include "macro.h"
#include <stdio.h>

static void gotoxy(short row, short col) { printf("%c[%d;%dH", 27, row + 1, col + 1); }

static const char *FANSPEED_STR[] = {
    "무풍", "미풍", "약풍", "강풍", "파워"};

static const char *OUTDOOR_STR[] = {
    "미가동", "냉방 ", "난방 "};

static const char *MODE_STR[] = {
    "      운전 정지            \n",
    "      냉방운전 중          \n",
    "      무풍 냉방운전 중      \n",
    "      제습운전 중          \n",
    "      무풍 제습운전 중      \n",
    "      난방운전 중          \n",
    "      송풍운전 중          \n"};

void printstatus(aircon_t aircon[], int amount, int select)
{
  gotoxy(0, 0);
  printf("==================== Aircon Management System ====================\n");
  for (int i = 1; i <= amount; i++)
  {
    int sel = select ? (select == i) : 1; // 0이면 반드시 선택됨
    printf(" %s Air Conditioner #%d %s\n", (sel ? "<<<<" : "    "), i, (sel ? ">>>>   " : "     "));
    printf(MODE_STR[aircon->state]);
    switch (aircon->state)
    {
    case MODE_OFF:
      printf("                                                                \n");
      break;
    case MODE_COOL:
    case MODE_CALM_COOL:
    case MODE_DEHUM:
    case MODE_CALM_DEHUM:
    case MODE_WARM:
      printf("      설정 온도: %d, 바람 세기: %s, 실외기 가동 여부: %s          \n", aircon->target_temp, FANSPEED_STR[aircon->fan], OUTDOOR_STR[aircon->outdoor_unit]);
      break;
    case MODE_BLOW:
      printf("                     바람 세기: %s, 실외기 가동 여부: %s          \n", FANSPEED_STR[aircon->fan], OUTDOOR_STR[aircon->outdoor_unit]);
      break;
    }
    printf("\n");
    aircon++;
  }
  printf("==================================================================\n");
  printf("                                                                  \n");
  gotoxy(4 * MAX_AIRCONS + 1, 0);
  printf("\n[Input] >> ");
}