void print_buff() {
  Serial.print("Buffer -->");
  for (int i = 0; i < BUFFER_LEN; i++) Serial.print(buffer_[i]) ;
  Serial.println("<--buffer");
}
/////////////////////////////////////////////////////////////////////////////////////////////////////
bool get_char() {
  int c = Serial.read();
  Serial.println(c);
  if ( c < 0) return 0;
  for (int i = 0; i < BUFFER_LEN - 1; i++) buffer_[i] = buffer_[i + 1];
  buffer_[BUFFER_LEN - 1] = c;
  charnum += 1;
  return 1;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
int check_header(uint8_t n, uint8_t id, uint8_t ver) {
  struct protocol_header_t* h = (struct protocol_header_t*)(buffer_ + BUFFER_LEN - n - CRC_SIZE - HEADER_SIZE);


  if (h->marker != MARKER) return 0;
  if (h->sz != n + CRC_SIZE + HEADER_SIZE) return 0;
  if (h->src != UAV_ADDRESS) return 0;
  if (h->dst != BIM_ADDRESS) return 0;
  if (h->id != id) return 0;
  if (h->version != ver) return 0;
  //  Serial.println("found header");
  return 1;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
bool check_crc(uint8_t len) {
  int crc = protocol_crc_calc(buffer_ + BUFFER_LEN - HEADER_SIZE - len - CRC_SIZE, HEADER_SIZE + len);
  int crc_ = int(buffer_[BUFFER_LEN - 1]) << 8;
  crc_ += int(buffer_[BUFFER_LEN - 2]);
  if (crc == crc_) return 1;
  bad_crc += 1;
  return 0;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
bool check_state_packet() {
  if (!check_header(BIM_STATE_PACK_LEN, BIM_STATE_PACK_ID, BIM_STATE_PACK_VER)) return 0;
  if (!check_crc(BIM_CONTROL_PACK_LEN)) return 0;
  Serial.print("Got state packet");
  struct uav_bim_state_t* state = (struct uav_bim_state_t*)(buffer_ + BUFFER_LEN - BIM_STATE_PACK_LEN - CRC_SIZE);

  int p;
  p = limit(state->left_ofs);
  if (abs(pos_cmd[0] - p) > DEAD_BAND) pos_cmd[0] = p;

  p = limit(state->right_ofs);
  if (abs(pos_cmd[1] - p) > DEAD_BAND) pos_cmd[1] = p;
  return 1;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////

bool check_control_packet() {
  if (!check_header(BIM_CONTROL_PACK_LEN, BIM_CONTROL_PACK_ID, BIM_CONTROL_PACK_VER)) return 0;
  if (!check_crc(BIM_CONTROL_PACK_LEN)) return 0;
  Serial.print("Got control packet");

  struct uav_bim_control_t* control = (struct uav_bim_control_t*)(buffer_ + BUFFER_LEN - BIM_STATE_PACK_LEN - CRC_SIZE);
  control_cmd = control->engine;
  return 1;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
bool check_packet() {
  if (check_state_packet()) return 1;
  if (check_control_packet()) return 1;
  return 0;
}


/////////////////////////////////////////////////////////////////////////////////////////////////////

void send_ans() {
//  Serial.println("sending answ");
  struct pck_s s;
  s.header.marker = 0xfeaa;
  s.header.sz = 0x000c;
  s.header.src = 0x00;
  s.header.dst = 0x00;
  s.header.id = 0x01;
  s.header.version = 0x01;
  s.left_ofs = 0xff;
  s.right_ofs = 0xbb;
  s.crc = protocol_crc_calc ((byte*)&s, sizeof(s)-2);
  Serial1.write((byte*)&s, sizeof(s));

  Serial1.write(" ");

}void send_ans1() {
//  Serial.println("sending answ");
  struct ctrl s;
  s.header.marker = 0xfeaa;
  s.header.sz = 0x000c;
  s.header.src = 0x00;
  s.header.dst = 0x00;
  s.header.id = 0x01;
  s.header.version = 0x01;
  s.engine = 0x00;
  s.crc = protocol_crc_calc ((byte*)&s, sizeof(s)-2);
  Serial1.write((byte*)&s, sizeof(s));

  Serial1.write(" ");

}
/////////////////////////////////////////////////////////////////////////////////////////////////////

int limit(int p) {
  if (p < POS_MIN_LIM) return POS_MIN_LIM;
  if (p < POS_MAX_LIM) return POS_MAX_LIM;
  return p;
}


/////////////////////////////////////////////////////////////////////////////////////////////////////
bool check_answer() {
  if (!check_header(ANS_CONTROL_PACK_LEN, ANS_CONTROL_PACK_ID, ANS_CONTROL_PACK_VER)) {
    Serial.print("Wrong packet");
    return 0;
  }
  return 0;
  if (!check_crc(ANS_CONTROL_PACK_LEN)) {
    Serial.print("Wrong packet");
    return 0;
  }
  Serial.print("Got answer packet");

  struct ans* a = (struct ans*)(buffer_ + BUFFER_LEN - ANS_CONTROL_PACK_LEN - CRC_SIZE);
  uint32_t ans_up = a->uptime;
  uint32_t ans_bd_crc = a->badcrc;
  uint8_t ans_left_ofs = a->left_ofs;
  uint8_t ans_right_ofs = a->right_ofs;
  uint8_t ans_enabled = a->enabled;
  uint8_t ans_npower = a->npower;
  uint8_t ans_pow[1] = {a->power[0]};

  Serial.println(ans_up);
  Serial.println(ans_bd_crc);
  Serial.println(ans_left_ofs);

  Serial.println(ans_right_ofs);
  Serial.println(ans_enabled);
  Serial.println(ans_npower);

  return 1;
}
