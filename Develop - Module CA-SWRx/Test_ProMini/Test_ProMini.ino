/*Để nạp chương trình cho ProMini nhờ mạch nạp của Arduino UNO. Ta chuẩn bị một Arduino UNO bất kì. 
 * 1. Nạp một chương trình trắng cho Arduino UNO này, chương trình trắng chỉ bao gồm phần setup() và loop() mà không có các 
 * nội dung thao tác. (chọn đúng Board UNO và COM kết nối).
 * 2. Kết nối giữa Arduino UNO và ProMini như sau:
 *          UNO         ProMini
 *          5V            VCC
 *          GND           GND
 *          TX            TX
 *          RX            RX
 *          RTS           DTR
 * 3. Kết nối lại USB UART và chọn Board ProMini trong list board cùng với loại thích hợp và COM kết nối.
 * 4. Bắt đầu nạp code trực tiếp cho ProMini như bình thường.
 * 
 * Hoặc nạp code bằng TTL USB Uart bằng cách: đấu chéo chân TX-RX RX-TX
 * Khi nạp code nhấn giữ reset cho đến khi console arduino báo uploading... thì nhả nút reset ra.
 */                                                  

int PIN_1 = 2;
int PIN_2 = 3;
int PIN_3 = 4;
int PIN_4 = 5;
int PIN_5 = 6;

int PIN_6 = 7;
int PIN_7 = 8;
int PIN_8 = 9;
int PIN_9 = 10;
int PIN_10 = 11;

int PIN_11 = 12;
int PIN_12 = 13;
int PIN_13 = A0;
int PIN_14 = A1;
int PIN_15 = A2;

int PIN_16 = A3;

void setup() {
    pinMode(PIN_1, OUTPUT);
    pinMode(PIN_2, OUTPUT);
    pinMode(PIN_3, OUTPUT);
    pinMode(PIN_4, OUTPUT);
    pinMode(PIN_5, OUTPUT);

    pinMode(PIN_6, OUTPUT);
    pinMode(PIN_7, OUTPUT);
    pinMode(PIN_8, OUTPUT);
    pinMode(PIN_9, OUTPUT);
    pinMode(PIN_10, OUTPUT);

    pinMode(PIN_11, OUTPUT);
    pinMode(PIN_12, OUTPUT);
    pinMode(PIN_13, OUTPUT);
    pinMode(PIN_14, OUTPUT);
    pinMode(PIN_15, OUTPUT);
    pinMode(PIN_16, OUTPUT);
}

void loop() {
    digitalWrite(PIN_1, HIGH);
    digitalWrite(PIN_2, HIGH);
    digitalWrite(PIN_3, HIGH);
    digitalWrite(PIN_4, HIGH);
    digitalWrite(PIN_5, HIGH);
    digitalWrite(PIN_6, HIGH);
    digitalWrite(PIN_7, HIGH);
    digitalWrite(PIN_8, HIGH);
    digitalWrite(PIN_9, HIGH);
    digitalWrite(PIN_10, HIGH);
    digitalWrite(PIN_11, HIGH);
    digitalWrite(PIN_12, HIGH);
    digitalWrite(PIN_13, HIGH);
    digitalWrite(PIN_14, HIGH);
    digitalWrite(PIN_15, HIGH);
    digitalWrite(PIN_16, HIGH);
    delay(500);

    digitalWrite(PIN_1, LOW);
    digitalWrite(PIN_2, LOW);
    digitalWrite(PIN_3, LOW);
    digitalWrite(PIN_4, LOW);
    digitalWrite(PIN_5, LOW); 
    digitalWrite(PIN_6, LOW);
    digitalWrite(PIN_7, LOW);
    digitalWrite(PIN_8, LOW);
    digitalWrite(PIN_9, LOW);
    digitalWrite(PIN_10, LOW); 
    digitalWrite(PIN_11, LOW);
    digitalWrite(PIN_12, LOW);
    digitalWrite(PIN_13, LOW);
    digitalWrite(PIN_14, LOW);
    digitalWrite(PIN_15, LOW); 
    digitalWrite(PIN_16, LOW);

    delay(500);
}
