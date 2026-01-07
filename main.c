#include "aircon.h"
#include "macro.h"
#include "print.h"

#define _CRT_SECURE_NO_WARNINGS
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main()
{
  aircon_t aircon[MAX_AIRCONS] = {
      {MODE_OFF, 20, FAN_MID, OUT_OFF},
      {MODE_OFF, 20, FAN_MID, OUT_OFF},
      {MODE_OFF, 20, FAN_MID, OUT_OFF},
      {MODE_OFF, 20, FAN_MID, OUT_OFF}};

  char buffer[100], cmd[20], option[20], action[20], val_str[20];
  int current_target = 0; // 0: 전체, 1~4: 개별 번호
  int val = 0;

  while (1)
  {
    printstatus(aircon, MAX_AIRCONS, current_target);

    if (fgets(buffer, sizeof(buffer), stdin) == NULL) break;
    buffer[strcspn(buffer, "\n")] = 0;

    if (strcmp(buffer, "exit") == 0) break;

    // 1. 대상 선택
    if (strncmp(buffer, "select", 6) == 0)
    {
      if (sscanf(buffer, "%s %s", cmd, option) < 2) continue;
      if (strcmp(option, "all") == 0) current_target = 0;
      else current_target = atoi(option);
      printf(">> [System] %s 대상 설정 완료\n", current_target == 0 ? "전체" : option);
    }
    // 2. 명령어 처리
    else
    {
      // %d 대신 %s로 읽어서 'on', 'up' 등을 모두 받아냅니다.
      if (sscanf(buffer, "%s %s", action, val_str) < 2)
      {
        printf(">> [Error] 값도 입력해주세요. (예: power on)\n");
        continue;
      }

      // 전원 제어 (power on/off)
      if (strcmp(action, "power") == 0)
      {
        printf(">> [함수 호출] 대상 %d -> 전원 %s 전송\n", current_target, val_str);
        if (strcmp(val_str, "on") == 0)
        {
          if (current_target)
            powerOn(&aircon[current_target - 1]);
          else
            for (int i = 0; i < MAX_AIRCONS; i++)
              powerOn(&aircon[i]);
        }
        if (strcmp(val_str, "off") == 0)
        {
          if (current_target)
            powerOff(&aircon[current_target - 1]);
          else
            for (int i = 0; i < MAX_AIRCONS; i++)
              powerOff(&aircon[i]);
        }
      }
      // 무풍 제어 (calm on/off)
      else if (strcmp(action, "calm") == 0)
      {
        printf(">> [함수 호출] 대상 %d -> 무풍 %s 전송\n", current_target, val_str);
        if (strcmp(val_str, "on") == 0)
        {
          if (current_target)
            calmOn(&aircon[current_target - 1]);
          else
            for (int i = 0; i < MAX_AIRCONS; i++)
              calmOn(&aircon[i]);
        }
        if (strcmp(val_str, "off") == 0)
        {
          if (current_target)
            calmOff(&aircon[current_target - 1]);
          else
            for (int i = 0; i < MAX_AIRCONS; i++)
              calmOff(&aircon[i]);
        }
      }
      // 온도 제어 (temp 24 / up / down)
      else if (strcmp(action, "temp") == 0)
      {
        if (strcmp(val_str, "up") == 0)
        {
          printf(">> [함수 호출] 대상 %d -> 온도 1도 UP 전송\n", current_target);
          if (current_target)
            tempUp(&aircon[current_target - 1]);
          else
            for (int i = 0; i < MAX_AIRCONS; i++)
              tempUp(&aircon[i]);
        }
        else if (strcmp(val_str, "down") == 0)
        {
          printf(">> [함수 호출] 대상 %d -> 온도 1도 DOWN 전송\n", current_target);
          if (current_target)
            tempDown(&aircon[current_target - 1]);
          else
            for (int i = 0; i < MAX_AIRCONS; i++)
              tempDown(&aircon[i]);
        }
        else
        {
          printf(">> [함수 호출] 대상 %d -> 온도 %s도 직접 설정 전송\n", current_target, val_str);
          if (current_target)
            tempTarget(&aircon[current_target - 1], atoi(val_str));
          else
            for (int i = 0; i < MAX_AIRCONS; i++)
              tempTarget(&aircon[i], atoi(val_str));
        }
      }
      // 모드 제어 (mode cool / warm 등)
      else if (strcmp(action, "mode") == 0)
      {
        char mode_name[20];
        sscanf(buffer, "%s %s", action, mode_name);

        char *s = mode_name;
        while (*s) *s = tolower((unsigned char)*s), s++;

        mode_t mode_val = -1;
        if (strcmp(mode_name, "next") == 0)
        {
          printf(">> next\n");
          if (current_target)
            changeMode(&aircon[current_target - 1]);
          else
            for (int i = 0; i < MAX_AIRCONS; i++)
              changeMode(&aircon[i]);
        }
        else
        {
          if (strcmp(mode_name, "cool") == 0) mode_val = MODE_COOL;
          else if (strcmp(mode_name, "dehum") == 0) mode_val = MODE_DEHUM;
          else if (strcmp(mode_name, "warm") == 0) mode_val = MODE_WARM;
          else if (strcmp(mode_name, "blow") == 0) mode_val = MODE_BLOW;

          if (mode_val != -1)
          {
            printf(">> [개별 제어] %d번 에어컨 모드 [%s](%d) 설정\n", current_target, mode_name, mode_val);
            if (current_target)
              changeModeTarget(&aircon[current_target - 1], mode_val);
            else
              for (int i = 0; i < MAX_AIRCONS; i++)
                changeModeTarget(&aircon[i], mode_val);
          }
          else
          {
            printf(">> [Error] 알 수 없는 모드입니다. (cool, warm, dehum, blow 등)\n");
          }
        }
      }
      // 풍량 제어 (fan low / high 등)
      else if (strcmp(action, "fan") == 0)
      {
        char fanspeed[20];
        fanspeed_t fs_val = -1;
        sscanf(buffer, "%s %s", action, fanspeed);
        char *s = fanspeed;
        while (*s) *s = toupper((unsigned char)*s), s++;
        if (strcmp(fanspeed, "LOW") == 0) fs_val = FAN_LOW;
        else if (strcmp(fanspeed, "MID") == 0) fs_val = FAN_MID;
        else if (strcmp(fanspeed, "HIGH") == 0) fs_val = FAN_HIGH;
        else if (strcmp(fanspeed, "POWER") == 0) fs_val = FAN_POWER;

        if (strcmp(val_str, "up") == 0)
        {
          printf(">> [함수 호출] 대상 %d -> 풍량 UP 전송\n", current_target);
          if (current_target)
            fanUp(&aircon[current_target - 1]);
          else
            for (int i = 0; i < MAX_AIRCONS; i++)
              fanUp(&aircon[i]);
        }
        else if (strcmp(val_str, "down") == 0)
        {
          printf(">> [함수 호출] 대상 %d -> 풍량 DOWN 전송\n", current_target);
          if (current_target)
            fanDown(&aircon[current_target - 1]);
          else
            for (int i = 0; i < MAX_AIRCONS; i++)
              fanDown(&aircon[i]);
        }
        else
        {
          printf(">> [함수 호출] 대상 %d -> 풍량 %s 직접 설정 전송\n", current_target, val_str);
          if (current_target)
            fanTarget(&aircon[current_target - 1], fs_val);
          else
            for (int i = 0; i < MAX_AIRCONS; i++)
              fanTarget(&aircon[i], fs_val);
        }
      }
      else
      {
        printf(">> [Error] 알 수 없는 명령어: %s\n", action);
      }
    }
  }

  return 0;
}