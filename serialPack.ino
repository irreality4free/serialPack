#include <SoftwareSerial.h>

#define BUFFER_LEN 32

#define POS_MIN_LIM 10
#define POS_MAX_LIM 90
#define DEAD_BAND 5
#define BATTERY_N 1
#define BIM_STATE_PACK_LEN  2
#define BIM_STATE_PACK_ID   1
#define BIM_STATE_PACK_VER  1
#define BIM_CONTROL_PACK_LEN  1
#define BIM_CONTROL_PACK_ID   3
#define BIM_CONTROL_PACK_VER  1
#define CRC_SIZE 2
#define HEADER_SIZE 8
#define UAV_ADDRESS 0
#define BIM_ADDRESS 0
#define MARKER 0xAEFF


#define ANS_CONTROL_PACK_LEN  1
#define ANS_CONTROL_PACK_ID   3
#define ANS_CONTROL_PACK_VER  1

char buffer_[BUFFER_LEN];
int pos_cmd[2];
int pos_fb[2];
int battery[BATTERY_N];
int control_cmd;
long uptime = 0;
long bad_crc;
int state;
int charnum = 0;
long loop_count = 0;



const byte rxPin = 2;
const byte txPin = 3;
char inByteComp = ' ';
char inByteSTM = ' ';

enum
{
  UAV_BIM_ENGINE_OFF = 0,
  UAV_BIM_ENGINE_ON = 1,
  UAV_BIM_ENGINE_SAFEOFF = 2,
  UAV_BIM_ENGINE__LAST
};

#pragma pack(push,1)
struct protocol_header_t
{
  uint16_t marker;
  uint16_t sz;
  uint8_t src;
  uint8_t dst;
  uint8_t id;
  uint8_t version;
};
#pragma pack(pop)

#pragma pack(push,1)
struct pack
{
  struct protocol_header_t header;
  uint32_t uptime;
  uint32_t badcrc;
  uint8_t left_ofs;
  uint8_t right_ofs;
  uint8_t enabled;
  uint8_t npower;
  uint8_t power[];
  uint16_t crc;
};

#pragma pack(pop)


#pragma pack(push,1)
struct pck_s
{
  struct protocol_header_t header;
  uint8_t left_ofs;
  uint8_t right_ofs;
  uint16_t crc;
};
#pragma pack(pop)

#pragma pack(push,1)
struct uav_bim_state_t
{
  uint8_t left_ofs;
  uint8_t right_ofs;
};
#pragma pack(pop)

#pragma pack(push,1)
struct uav_bim_status_t
{
  struct protocol_header_t header;
  uint32_t uptime;
  uint32_t badcrc;
  uint8_t left_ofs;
  uint8_t right_ofs;
  uint8_t enabled;
  uint8_t npower;
  uint8_t power[];
  uint16_t crc;
};
#pragma pack(pop)



#pragma pack(push,1)
struct uav_bim_control_t
{
  uint8_t engine;
};
#pragma pack(pop)#pragma pack(push,1)



#pragma pack(push,1)
struct ctrl
{
  struct protocol_header_t header;
  uint8_t engine;
  uint16_t crc;
};
#pragma pack(pop)



#pragma pack(push,1)
struct uav_bim_control_t
{
  uint8_t engine;
};
#pragma pack(pop)

#pragma pack(push,1)
struct ans
{
  struct protocol_header_t header;
  uint32_t uptime;
  uint32_t badcrc;
  uint8_t left_ofs;
  uint8_t right_ofs;
  uint8_t enabled;
  uint8_t npower;
  uint8_t power[];
  uint16_t crc;
};
#pragma pack(pop)




void setup() {
  Serial.begin(9600); // set the baud rate
  Serial1.begin(57600);
  Serial.println("Ready"); // print "Ready" once
}


void loop() {
send_ans();
//Serial.write("char");
//if (Serial.available()){
//int r = Serial.read();
//Serial.println(r);
//}
//Serial.println(get_char());
//Serial.println(get_char());
//  if (get_char()) {
//
//    if (check_answer()) {
//
//      Serial.println("ch ans if");
//    }


//}
delay(100);
}
