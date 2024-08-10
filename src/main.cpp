#include <Arduino.h>
#include "WiFi.h"
#include "esp_now.h"

/* 와이파이 관련 변수들 정의*/
//board macAddress to send data (내가 board A이면 보드 B가 dest가 되고, 또 그 반대)
byte boardA[] = {0xE8,0x68,0xE7,0x0B,0xBA,0xD0};
byte boardB[] = {0x24,0x6F,0x28,0xB3,0xF5,0x60};
esp_now_peer_info_t dest;

// put function declarations here:
int myFunction(int, int);
void wifiSetup();
void OnDataRecv(const uint8_t *mac, const uint8_t *data, int len);
void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status);

void setup() {
  // put your setup code here, to run once:
  int result = myFunction(2, 3);
}

void loop() {
  // put your main code here, to run repeatedly:
}

// put function definitions here:
int myFunction(int x, int y) {
  Serial.begin(115200);
  // Serial.print(" macAddress of board B : ");
  // Serial.println(WiFi.macAddress());
  wifiSetup();

  return x + y;
}

void wifiSetup() {
  // Serial.print("My macAddress(board B) : ");
  // Serial.println(WiFi.macAddress());

  //제일 먼저 할 일 -> 와이파이 모드를 스테이션 모드로 설정!
  WiFi.mode(WIFI_STA); 
  //espnow 기능을 활성화 한다.
  if(esp_now_init() != 0) {
    Serial.println("Failed to ESPNOW Enable!!!");
    return;
  }
  else {
    //ESP Enable Success
    Serial.println("Success for ESPNOW Enable!!!!!!");
  }
  //나의 peer를 등록한다.
  /*(내가 누군가에게 데이터를 보낼 때 그 상대방의 peer를 등록한다.즉 수신 측의 주소를 등록한다.)
  만일 그 상대방이 나에게 데이터를 전송 하려고하면 나의 주소를 peer로 그 쪽 보드에 등록해야 한다. 즉 수신하는 쪽은
  peer 등록을 할 필요가 없다!!!
  */
  //상대방 보드의 macAddress를 대입한다.
  for(int i = 0; i < 6; i++) {
    dest.peer_addr[i] = boardA[i];  //board B가 board A로 전송하겠다는 의미
  }
  //상대방의 채널을 설정
  /* 그런데 외부 WiFi(iPTime WiFi 공유기 같은)와 연결할 때는 9번 채널을 사용한다!*/
  dest.channel = 1;
  dest.encrypt = false;
  //최종적으로 나의 목적지 주소를 등록한다.
  esp_now_add_peer(&dest);
  //송신완료 콜백함수 등록
  esp_now_register_send_cb(OnDataSent);
  //수신완료 콜백함수 등록
  esp_now_register_recv_cb(OnDataRecv);
}

//송신콜백 함수 원형:
//void OnDataSent(uint8_t *mac, uint8_t status) {}
/* 이 함수는 ESP32 보드가 데이터를 전송하고 데이터가 성공적으로 다 전송되었을 때 호출되는 함수이다.
   즉 데이터를 전송하기 위해 전송 함수를 실행하고 다른 함수들을 실행하여도 전송이 완료되었을 때를
   인터럽트 콜백 함수로 알려주기 때문에 프로그램이 블록킹 되지 않는다. */
void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status) {
  //status가 0일때 송신 완료
  if(status == 0) {
    Serial.println("Sent successfully!!!");
  }
  else {
    Serial.println("Failed to send!");
  }
}

//수신콜백 함수 원형:
//void OnDataRecv(uint8_t *mac, uint8_t *data, uint8_t len) {}
/* 이 함수는 다른 ESP32보드가 내 보드로 데이터를 전송 했을 때 호출되는 함수이다. */
void OnDataRecv(const uint8_t *mac, const uint8_t *data, int len) {
  String text = "";
  for(int i = 0; i < len; i++) {
    text += (char)data[i];
  }
  Serial.println(text);
}

