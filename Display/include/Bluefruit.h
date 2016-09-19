#ifndef __BLUEFRUIT__H__
#define __BLUEFRUIT__H__

int bluefruit_init(void);
int send_at_command(char* at_cmd);
int get_at_cmd_response();

#endif