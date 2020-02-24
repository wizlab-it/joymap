/**
 * @package Joymap - Joystick and joypad mapper
 * @author WizLab.it
 * @version 0.7.1
 * @date 2017-08-26
 *
 * How to compile:
 * $ gcc joymap.c -lxdo -o joymap
 * Requirements: libxdo-dev
 *
 * LICENSE
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * To get a copy of the GNU General Public License, see <http://www.gnu.org/licenses/>.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <xdo.h>
#include <fcntl.h>

#define VERSION "0.7.1"
#define MAX_AXES 6
#define MAX_BUTTONS 26
#define MAP_MAX_STRING_LENGTH 20
#define DEVICE_MAX_STRING_LENGTH 40
#define XDO_KEYSEQUENCE_DELAY 12000
#define JS_EVENT_BUTTON 0x01
#define JS_EVENT_AXIS 0x02

struct js_event {
  unsigned int time;
  short value;
  unsigned char type;
  unsigned char number;
};

int main (int argc, char **argv) {
  char device[DEVICE_MAX_STRING_LENGTH + 1];
  char axes[MAX_AXES][2][MAP_MAX_STRING_LENGTH + 1];
  char buttons[MAX_BUTTONS][MAP_MAX_STRING_LENGTH + 1];
  int axesCounters[MAX_AXES][2];
  int buttonCounters[MAX_BUTTONS];
  struct js_event jsEvent;
  xdo_t* xdo = xdo_new(":0.0");
  int js;
  int rawDataFlag = 0;
  char *t;
  int c, i;

  //Initialize data
  memset(device, 0, (DEVICE_MAX_STRING_LENGTH + 1));
  memset(axes, 0, (MAX_AXES * 2 * (MAP_MAX_STRING_LENGTH + 1)));
  memset(buttons, 0, (MAX_BUTTONS * (MAP_MAX_STRING_LENGTH + 1)));
  for(i=0; i<MAX_AXES; i++) {
    axesCounters[i][0] = 0;
    axesCounters[i][1] = 0;
  }
  for(i=0; i<MAX_BUTTONS; i++) buttonCounters[i] = 0;

  //parse arguments
  while((c = getopt(argc, argv, "0:1:2:3:4:5:6:7:8:9:A:B:C:D:E:F:G:H:I:J:K:L:M:N:O:P:d:p:q:r:s:t:u:hvx")) != -1) {
    switch (c) {
      //Usage
      case 'h':
        printf("  Usage: joymap [OPTIONS]\n");
        printf("    -[0-1A-P]\t\tSet the button character map (i.e. \"-0 X\" map button 0 to character X). Button A = Button 10, Button P = Button 25\n");
        printf("    -[pqrstu]\t\tAxis 0/1/2/3/4/5, data set as \"up/left down/right\" (i.e. '-r \"Up Down\"' map up/down arrow keys to axis 2)\n");
        printf("    -d\t\t\tDevice (i.e. /dev/input/js0)\n");
        printf("    -x\t\t\tOnly print raw data (for debug purposes)\n");
        printf("    -h\t\t\tShow this message\n");
        printf("    -v\t\t\tPrint version information and exit\n");
        printf("  Use any character or X11 KeySym name to map buttons and axes\n\n");
        exit(0);

      //Author, version and license
      case 'v':
        printf("  Joymap - Joystick and joypad mapper ver. %s\n", VERSION);
        printf("  Copyright (C) 2017 WizLab.it <https://www.wizlab.it>\n\n");
        printf("  This program is free software: you can redistribute it and/or modify\n");
        printf("  it under the terms of the GNU General Public License as published by\n");
        printf("  the Free Software Foundation, either version 3 of the License, or\n");
        printf("  (at your option) any later version.\n\n");
        printf("  This program is distributed in the hope that it will be useful,\n");
        printf("  but WITHOUT ANY WARRANTY; without even the implied warranty of\n");
        printf("  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the\n");
        printf("  GNU General Public License for more details.\n\n");
        printf("  To get a copy of the GNU General Public License, see <http://www.gnu.org/licenses/>.\n\n");
        exit(0);

      //Device
      case 'd':
        strncpy(device, optarg, DEVICE_MAX_STRING_LENGTH);
        break;

      //Axes
      case 'p': case 'q': case 'r': case 's': case 't': case 'u':
        if((c >= 'p') && (c <= 'u')) c = c - 'p';
        else break;
        i = 0;
        t = strtok(optarg, " ");
        while(t != NULL) {
          strncpy(axes[c][i], t, MAP_MAX_STRING_LENGTH);
          i++;
          if(i == 2) break;
          t = strtok(NULL, " ");
        }
        if(i < 2) memset(axes[c], 0, (2 * (MAP_MAX_STRING_LENGTH + 1)));
        break;

      //Buttons
      case '0': case '1': case '2': case '3': case '4': case '5': case '6': case '7':
      case '8': case '9': case 'A': case 'B': case 'C': case 'D': case 'E': case 'F':
      case 'G': case 'H': case 'I': case 'J': case 'K': case 'L': case 'M': case 'N':
      case 'O': case 'P': 	
        if((c >= '0') && (c <= '9')) c = c - '0';
        else if((c >= 'A') && (c <= 'I')) c = c - 'A' + 10;
	else if((c >= 'J') && (c <= 'P')) c = c - 'J' + 20;
        else break;
        strncpy(buttons[c], optarg, MAP_MAX_STRING_LENGTH);
        break;

      //Set raw data flag
      case 'x':
        rawDataFlag = 1;
        break;

      //Unknown and invalid arguments
      case '?':
        fprintf(stderr, "  Invalid argiments\n\n");
        return 1;

      default:
        abort();
    }
  }

  //If device is not set, use the default one
  if(device[0] == '\0') strcpy(device, "/dev/input/js0");

  //Print configuration
  printf(" Using device %s\n", device);
  for(i=0; i<MAX_BUTTONS; i++) {
    if(buttons[i][0] != '\0') printf("  Button %d => %s\n", i, buttons[i]);
  }
  for(i=0; i<MAX_AXES; i++) {
    if(axes[i][0][0] != '\0') printf("  Axis %d:\n    High => %s\n    Low => %s\n", i, axes[i][0], axes[i][1]);
  }

  //Open device
  js = open(device, O_RDONLY);
  if(js < 0) {
    fprintf(stderr, "\n  ERROR: device %s is not available\n\n", device);
    exit(0);
  }

  //Device opened
  printf("\n  Device mapping is running. Press CTRL+C to stop...\n\n");
  if(rawDataFlag == 1) printf("Device raw data:\n");

  //Run!
  while(1) {
    read(js, &jsEvent, sizeof(jsEvent));
    if(rawDataFlag == 1) {
      printf("Time: %d; Type: %d; Button/Axis ID: %d; Value: %d\n", jsEvent.time, jsEvent.type, jsEvent.number, jsEvent.value);
    } else {
      switch(jsEvent.type) {
        case JS_EVENT_BUTTON:
          if((jsEvent.number >= 0) && (jsEvent.number < MAX_BUTTONS)) {
            if(jsEvent.value == 1) {
              xdo_send_keysequence_window_down(xdo, CURRENTWINDOW, buttons[jsEvent.number], XDO_KEYSEQUENCE_DELAY);
              buttonCounters[jsEvent.number]++;
            } else {
              for(; buttonCounters[jsEvent.number]>0; buttonCounters[jsEvent.number]--) xdo_send_keysequence_window_up(xdo, CURRENTWINDOW, buttons[jsEvent.number], XDO_KEYSEQUENCE_DELAY);
            }
          }
          break;

        case JS_EVENT_AXIS:
          if((jsEvent.number >= 0) && (jsEvent.number < MAX_AXES)) {
            if(jsEvent.value > 0) {
              for(; axesCounters[jsEvent.number][0]>0; axesCounters[jsEvent.number][0]--) xdo_send_keysequence_window_up(xdo, CURRENTWINDOW, axes[jsEvent.number][0], XDO_KEYSEQUENCE_DELAY);
              xdo_send_keysequence_window_down(xdo, CURRENTWINDOW, axes[jsEvent.number][1], XDO_KEYSEQUENCE_DELAY);
              axesCounters[jsEvent.number][1]++;
            } else if(jsEvent.value < 0) {
              for(; axesCounters[jsEvent.number][1]>0; axesCounters[jsEvent.number][1]--) xdo_send_keysequence_window_up(xdo, CURRENTWINDOW, axes[jsEvent.number][1], XDO_KEYSEQUENCE_DELAY);
              xdo_send_keysequence_window_down(xdo, CURRENTWINDOW, axes[jsEvent.number][0], XDO_KEYSEQUENCE_DELAY);
              axesCounters[jsEvent.number][0]++;
            } else {
              for(; axesCounters[jsEvent.number][1]>0; axesCounters[jsEvent.number][1]--) xdo_send_keysequence_window_up(xdo, CURRENTWINDOW, axes[jsEvent.number][1], XDO_KEYSEQUENCE_DELAY);
              for(; axesCounters[jsEvent.number][0]>0; axesCounters[jsEvent.number][0]--) xdo_send_keysequence_window_up(xdo, CURRENTWINDOW, axes[jsEvent.number][0], XDO_KEYSEQUENCE_DELAY);
            }
          }
          break;
      }
    }
  }

  return 0;
}
