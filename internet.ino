
#include"wifi_.h"
#include"verify.h"

#include <Adafruit_GFX.h>
#include <Adafruit_PCD8544.h>
#include"display.h"             /// customised library header file for setting up the bitmaps(pictures)....

bool serve = false;
bool recieving = false;
bool read_d = false;

Adafruit_PCD8544 display = Adafruit_PCD8544(7, 6, 5, 4, 3);

///uint16_t mi = 0;

bool new_ra2 = true;
bool main_m = true;
bool rst_t = false;
char led_state[4] = "OFF";
bool activate_m [2] = {false , false};       //// for messages and apps light respectively.................
bool new_ra = true;
bool once = true;
bool once_2 = true;
bool sel = false;

uint8_t sizz = 0;
uint8_t incr = 1;

uint16_t data[20];
uint16_t dat = 0;

uint16_t end_o = 0;

uint16_t temp = 0;
uint16_t start = 0;
uint16_t end_d[20];
uint16_t endnum = 0;
uint16_t checkin = 0;

uint16_t bc = 0;

byte se[2] = {1, 0};
byte rem = 0;

bool wifi = false;
bool don = false;

char get_messages[60];    //// temporal

uint16_t las = 0;

bool activate_apps[3] = {false , false , false};        //// for alarm , tourchlight and settings respectively..........


const uint16_t PINUP  = A1;
const uint16_t PINDOWN = A2;
const uint16_t PINSELECT = A3;
const uint16_t PINHOME = A0;
const uint8_t light = 9;
const uint8_t vibrator = 8;
///const uint8_t BUTTONS = 2;


uint8_t point_pos = 15;  /// default pointer position for the main menu.
uint8_t point_pos_a = 12;

uint8_t point_pos_w = 0;

uint8_t point_pos_wi = 0;

bool connect_t = false;
bool connect_t2 = false;

unsigned short locate [2][2] = {
  {15 , 0},         /// default messages color.                           /// default color signifies not hihglighted
  {30 , 1},           /// default apps color
};

unsigned short locate_apps[3][2] = {
  {12 , 0},
  {24 , 1},
  {36 , 1},
};


void setup() {

  end_d[endnum] = 0;
  ++endnum;
  data[dat] = 0;
  ++dat;               /// temp

  get_messages[0] = NULL;
  const char mod[] = "OK";

  Serial.begin(115200);
  Serial1.begin(115200);
clear_b();


  pinMode(light , OUTPUT);
  digitalWrite(light, LOW);
  display.begin();
  display.setContrast(60);

  display.display();   /// starts display by showing the preset Adafruit logo.  according to the terms of the lincese...
  milli(1000);
  display.clearDisplay();

  display.drawBitmap(0 , 0 , cuwatch,  80 , 38 , 1);

  display.display();
  milli(2000);

  pinMode(PINUP , INPUT);
  pinMode(PINDOWN , INPUT);
  pinMode(PINHOME , INPUT);
  pinMode(PINSELECT , INPUT);
  pinMode(light , OUTPUT);
  pinMode(vibrator , OUTPUT);
  /// pinMode(BUTTONS , INPUT);

  digitalWrite(PINUP, LOW);
  digitalWrite(PINDOWN , LOW);
  digitalWrite(PINHOME , LOW);
  digitalWrite(PINSELECT, LOW);
  digitalWrite(vibrator, LOW);
  digitalWrite(light, LOW);

  ///digitalWrite(BUTTONS, LOW);



  milli(3000);

  /// attachInterrupt(digitalPinToInterrupt(BUTTONS) , button_actions , RISING);



  { ///  RST scope......

    char store[100];

    Serial1.println("AT+RST");
    milli(1000);
    verify(store , mod);

  }               /// end of RST....

  clear_b();
  Serial.println();


  { /// CWMODE Scope...........

    char store[50];

    Serial1.println("AT+CWMODE_CUR=1");
    milli(1000);
    verify(store , mod);

  }            /// End of CWMODE

  clear_b();
  Serial.println();


  milli(1000);

} /// end of setup


void loop() {


  wif();

  if (read_d) {

    if (serve) {
      uint16_t a = 0;
      Serial.println("obtained");

      char store[60];

      unsigned short connections = 0;
      bool conn = false;

      if (Serial1.find("+IPD,")) {

        Serial.println("yes");

        recieving = true;
        unsigned long long tim;

        bool over = false;

        tim = millis();
        while (!over) {


          char aa = Serial1.read();
          connections = atoi(&aa);




          if (Serial1.find("message=")) {
            new_ra2  = true;
            digitalWrite(vibrator , HIGH);
            Serial.print("connection is : ");
            Serial.println(connections);

            bool conn = false;

            Serial.println("found");


            while (!conn) {

              while (Serial1.available()) {
                store[a]  = Serial1.read();
                ++a;
              }

              tim = millis();

              while (!Serial1.available()) {
                if ( millis() - tim >= 3000) {
                  store[a] = '\0';
                  Serial.println("end");


                  const String recieved_page = "<h1> RECIEVED </h1>";

                  Serial1.print("AT+CIPSEND=");
                  Serial1.print(connections);
                  Serial1.print(",");
                  Serial1.println(recieved_page.length());
                  milli(3000);

                  tim = millis();

                  Serial1.print(recieved_page);
                  milli(2000);
                  bool sto = false;
                  char got[20];
                  int b = 0;
                  while (!sto) {

                    while (Serial1.available()) {
                      got[b] = Serial1.read();
                      Serial.print(got[b]);
                      ++b;
                      got[b] = '\0';
                    }

                    tim = millis();

                    while (!Serial1.available()) {
                      if ( millis() - tim >= 5000) {
                        Serial.println("over!");
                        sto = true;
                        break;
                      }
                    }
                  }



                  for (uint16_t t = 0; t < strlen(store) ; ++t) {
                    char &i = store[t];

                    if (i == '%') {
                      i = '\0';
                    }
                  }

                  for (uint16_t t = 0; t < strlen(store) ; ++t) {
                    char &i = store[t];
                    if (i == '+') {
                      i = ' ';
                    }
                  }

                  Serial.print("message : ");
                  Serial.println(store);

                  for (uint16_t i = 0; i < strlen(store); ++i, ++las) {
                    get_messages[las] = store[i];
                  }

                  get_messages[las] = '\0';
                  conn = true;

                  milli(1000);
                  Serial1.print("AT+CIPCLOSE=");
                  Serial1.println(connections);
                  milli(2000);


                  if (!Serial1.find("OK")) {
                    milli(2000);
                    Serial1.print("AT+CIPCLOSE=");
                    Serial1.println(connections);
                    milli(2000);
                  }

                  break;

                }///

              }///
            }
            digitalWrite(vibrator , LOW);
            over = true;
            recieving = false;
            break;

          }  /// if mess


          if ( (millis() - tim) >= 4000) {
            over = true;
            recieving = false;
            break;
          }


        }   /// while over

        recieving = false;
      }   /// if ipd


      read_d = false;
    }  /// serve

  }

  if (!read_d) {

    if (new_ra2) {


      if (main_m) {

        main_menu();
        wif();
      }

      else if (activate_m[0]) {

        messages_menu();
        wif();

      }
      else if (activate_m[1]) {

        apps_menu();
        wif();

      }
      else if (activate_apps[0]) {       /// test

      }
      else if (activate_apps[1]) {

        touch_light();
        wif();
      }

      else if (activate_apps[2]) {

        settings();
        wif();


      }
      else if (connect_t) {

        wif_con();
      }

      else if (connect_t2) {

        wif_sat();
      }
      new_ra2 = false;
    }
    button_actions();
    wif();

  }  ///read_d


}

///.........................................................................




void clear_b() {
  milli(1500);

  while (Serial1.available() > 0) {         /// function to erase any data left in buffer
    char a = Serial1.read();
  }

}


void main_menu() {

  display.clearDisplay();

  display.setCursor(display.width() / 4 , 0);
  display.setTextColor(BLACK);
  display.setTextSize(1);
  display.print("Main Menu");


  display.drawLine(0 , 10 , display.width() - 1, 10 , BLACK); /// basically to undeline "MAIN MENU".


  display.setCursor(5 , 15);
  display.setTextColor(locate[0][1] , BLACK);    ///locate[0][1] returns a single value of that address..
  display.print("Messages");

  /// height increasing by 15...
  display.setCursor(5 , 30);
  display.setTextColor(locate[1][1] , BLACK); ///   locate[1][1] returns a single value stored in that address..
  display.print("Apps");

  display.setCursor(0 , point_pos);
  display.setTextColor(BLACK);
  display.print('>');
  display.display();

}/// END OF MAIN MEANU FUNCTION>>>

///.......................................................................................................

void messages_menu() {

  display.clearDisplay();

  if ( strlen(get_messages) == 0) {
    display.setCursor(0 , 0);
    display.setTextColor(BLACK);
    display.setTextSize(1);
    display.print("NO MESSAGES!");
    display.display();
  }

  else {
    display.setCursor(0 , 0);
    display.setTextColor(BLACK);
    display.setTextSize(1);
    display.print(get_messages);
    display.display();
  }

}/// END OF MESSAGES MEANU FUNCTION>>>


///...............................................................................................................




void highlight_main() {

  const short *ptr = NULL;

  for (byte i = 0 ; i < 2 ; ++i) {
    ptr = locate[i];         /// points-> to the memory address of locate[i];

    if ( *ptr == point_pos) {
      locate[i][1] = 0;
    }  ///
    else {
      locate[i][1] = 1;
    }///
    ptr = NULL; /// resets pointer back to NULL (points to nothing now)...........
  }  ///

}  /// end of highlight........




void main_select() {

  for (byte i = 0 ; i < 2 ; ++i) {

    if (locate[i][1] == 0) {
      activate_m[i] = true;
      main_m = false;


    } ///
    else if (locate[i][1] != 0) {
      activate_m[i] = false;
    } ///
  }

  ///}


}  /// end of main selcet.............



void apps_menu() {

  display.clearDisplay();
  display.setCursor(display.width() / 8 , 0);
  display.setTextColor(BLACK);
  display.setTextSize(1);
  display.print("Applications");

  display.drawLine(0 , 10 , display.width() - 1, 10 , BLACK);

  display.setCursor(5 , 12);
  display.setTextColor(locate_apps[0][1], BLACK);
  display.print("Alarm");


  display.setCursor(5 , 24);
  display.setTextColor(locate_apps[1][1] , BLACK);
  display.print("Touch Light");
  /// height increasing by 12...........

  display.setCursor(5 , 36);
  display.setTextColor(locate_apps[2][1] , BLACK);
  display.print("Settings");



  display.setCursor(0 , point_pos_a);
  display.setTextColor(BLACK);
  display.print('>');
  display.display();


}/// end of apps menu...........



void highlight_apps() {

  const short *ptr = NULL;

  for (byte i = 0 ; i < 3 ; ++i) {
    ptr = locate_apps[i] ;    /// points-> to the memory address of locate[i];

    if (*ptr == point_pos_a) {
      locate_apps[i][1] = 0;
    }  ///
    else {
      locate_apps[i][1] = 1;
    }///
    ptr = NULL; /// resets pointer back to NULL (points to nothing now)...........
  }  ///


}  /// end of highlight........


void rst() {
  point_pos = 15;
  point_pos_a = 12;


  locate[0][1] = 0;
  locate[1][1] = 1;               /// just to reset the highlighter...........

  locate_apps[0][1] = 0;
  locate_apps[1][1] = 1;
  locate_apps[2][1] = 1;
  net_re();


}



void apps_select() {

  for (byte i = 0 ; i < 3 ; ++i) {

    if (locate_apps[i][1] == 0) {
      activate_m[1] = false;
      main_m = false;
      activate_apps[i] = true;
      point_pos_a = 12;



    } ///
    else if (locate_apps[i][1] != 0) {
      activate_apps[i] = false;
    } ///
  }


}  /// end of app selcet.............




void touch_light() {

  display.clearDisplay();
  display.setCursor(5, 12);
  display.setTextSize(1);
  display.setTextColor(WHITE, BLACK);
  display.print(led_state);
  /// height increasing by 12...........
  display.setCursor(0 , point_pos_a);
  display.setTextColor(BLACK);
  display.print('>');
  display.display();


}   ////


void settings() {

  display.clearDisplay();
  display.setTextSize(1);
  display.setCursor(6 , 0);
  display.setTextColor(BLACK);  ////<----- fix
  display.print("CONNECT WIFI");


  display.setCursor(6 , 12);
  display.print("WIFI-STATUS");

  display.setCursor(0 , point_pos_w);
  display.print('>');
  display.display();


}   ///


void button_actions() {


  if (!recieving) {

    new_ra2 = true;

    if (digitalRead(PINSELECT) == HIGH) {
      milli(200);
      if (main_m) {
        main_select();
      }

      else if (activate_m [1]) {
        apps_select();
      }

      else if (activate_apps[1]) {

        light_act();

      }  ///

      else if (activate_apps[2]) {


        activate_apps[2] = false;
        if (se[0]) {
          connect_t = true;
          connect_t2 = false;
        } ///

        else if (se[1]) {
          connect_t = false;
          connect_t2 = true;
        }  ///

      }  ///
      else if (connect_t) {
        if (!serve) {
          sel = true;
        }
      }

      else if (connect_t2) {
        sel = true;

      }

    }   /// end of select


    else if (digitalRead(PINUP) == HIGH) {
      milli(200);
      if (main_m) {

        if (point_pos > 15) {

          point_pos -= 15;
        }

        highlight_main();       //// highligt selected .....

      }///

      else if (activate_m [1]) {
        if (point_pos_a > 12) {

          point_pos_a -= 12;

        }

        highlight_apps();       //// highligt selected .....

      }///

      else if (activate_apps [2]) {
        if (point_pos_w > 0) {

          point_pos_w -= 12;
          --rem;
          se[rem] = 1;
          se[rem + 1] = 0;
        }

      }///

      else if (connect_t) {
        don = true;

        if (point_pos_wi > 0 && don) {
          don = false;
          point_pos_wi -= 8;
          new_ra = true;
          bc -= 8;
          if (bc == end_d[checkin - 1]) {
            --checkin;

            --incr;
          }

        }

        else if (point_pos_wi == 0 && temp > 0 && bc != 0 && don && start != 0) {
          don = false;
          bc -= 8;
          --start;
          temp = data[start];
          new_ra = true;
          if (bc == end_d[checkin - 1]) {
            --checkin;

            --incr;
          }

        }
      }  ////


    }/// end of pinup



    else if (digitalRead(PINDOWN) == HIGH) {
      milli(200);

      if (main_m) {
        if (point_pos == 15) {
          point_pos += 15;
        } ///

        highlight_main();      //// highligt selected .....

      }   /// end of main if..

      else if (activate_m [1]) {

        if (point_pos_a < 36 ) {
          point_pos_a += 12;
        } ///

        highlight_apps();      //// highligt selected .....

      }   /// end of main if..

      else if (activate_apps [2]) {

        if (point_pos_w < 12 ) {
          point_pos_w += 12;
          ++rem;
          se[rem] = 1;
          se[rem - 1] = 0;
        } ///

      }

      else if (connect_t) {
        don = true;
        if (bc <= end_o) {
          bc += 8;

          new_ra = true;
          if (bc <= 40 && don) {
            don = false;
            point_pos_wi += 8;

            if (bc == end_d[checkin + 1]) {
              ++checkin;             ///

              ++incr;
            }



          }

          else if (bc > 40 && don) {
            don = false;
            ++start;
            point_pos_wi  = 40;
            temp = data[start];
            if (bc == end_d[checkin + 1]) {
              ++checkin;             ///

              ++incr;
            }

          }



        }   ///
      }  //

    } /// end of if pinHigh


    else if (digitalRead(PINHOME) == HIGH) {
      milli(200);
      activate_m[0] = false;
      activate_m[1] = false;

      activate_apps[0] = false;
      activate_apps[1] = false;
      activate_apps[2] = false;

      main_m  = true;
      rst_t = true;

    }

    if (rst_t) {
      rst();
      rst_t = false;
    }


  }   ///


  wif();
}   ////


void light_act() {

  if (strstr(led_state, "OFF")  ) {

    digitalWrite(light , HIGH);
    led_state[0] = 'O';
    led_state[1] = 'N';
    led_state[2] = '\0';
  }


  else if ( !strstr(led_state, "OFF")  ) {

    digitalWrite(light , LOW);

    led_state[0] = 'O';
    led_state[1] = 'F';
    led_state[2] = 'F';
    led_state[3] = '\0';

  }

}



void wif() {

  if (serve) {
    if (Serial1.available()) {
      read_d = true;
    }
  }


}


void wif_con() {

  if (!serve) {

    display.clearDisplay();
    display.setTextSize(1);
    display.setCursor(6 , 0);
    display.setTextColor(BLACK);  ////<----- fix
    display.print("Searching for WIFI...");

    display.display();

    const char PROGMEM wifi[] = "WIFI CONNECTED";
    const char  PROGMEM not_wifi[] = "FAIL";

    uint16_t e = 0;
    bool conn = false;
    unsigned long long tim;
    uint16_t max_l = 400;

    uint8_t siz = 0;
    siz = 0;
    char store[max_l];                    ////
    char net[8];
    conn = false;



    Serial1.println("AT+CWLAP");
    milli(2000);


    while (!conn) {


      while (Serial1.available() > 0) {
        store[e] = Serial1.read();
        Serial.print(store[e]);
        ++e;

      }

      tim = millis();
      while (!Serial1.available()) {
        store[e] = '\0';
        if (millis() - tim >= 4000) {



          for (uint16_t t = 0 ; t < strlen(store) ; ++t) {

            if (store[t] == '+' && store[t + 1] == 'C' && store[t + 2] == 'W') {
              ++siz;
            }
          }
          if (siz > 0 && siz != 1) {
            siz -= 1;
          }
          Serial.print("number of wirless connections found : ");
          Serial.println(siz);
          sizz = siz;
          conn = true;
          break;
        }

      }


    }
    Serial.println();

    for (byte i = 0; i < 8; ++i) {
      store[i] = ' ';
    }
    Serial.print(store);

    Serial.println("getting");
    get_creds(store, "+CWLAP:(" , siz);       /// function of sort wifi details

    Serial.println();
    Serial.println(store);
    button_actions();
    clear_b();
    while (!serve && siz != 0 && !sel) {

      if (digitalRead(PINHOME)) {
        main_m = true;
        conn = true;
        rst();

        break;

      }
      button_actions();

      Serial.println(incr);        ////


      display.clearDisplay();
      display.setTextSize(1);
      display.setCursor(0, point_pos_wi);
      display.print(">");


      uint16_t cur_y;
      cur_y = 0;
      uint8_t cur;
      cur = 6;
      display.setCursor(cur , cur_y );
      uint16_t n = 1;

      if (new_ra) {

        for (uint16_t a = temp; a < strlen(store); ++a) {

          if ( (6 * n) >= 84  && store[a] != '\n') {
            cur_y += 8;

            cur = 6;
            n = 1;

            if (once) {

              data[dat] = a + 1;
              ++dat;

            }
          }

          if (store[a] == '\n') {
            cur = 6;
            cur_y += 8;
            n = 1;



            if (once) {
              end_d[endnum] = cur_y;
              data[dat] = a + 1;                           /// in testing............ for scrolling effect..
              ++dat;
              ++endnum;
            }
          }
          if (store[a] != '\n') {

            display.setCursor(cur , cur_y);
            display.print(store[a]);
            cur += 6;
            ++n;

          }
        }
        if (once) {
          endnum -= 1;
        }
        once = false;
        new_ra = false;
        display.display();
        ///


      }
      if (once_2) {

        for (uint16_t i = 0; i < strlen(store) ; ++i) {
          if (store[i] == '\n') {
            end_o += 8;
          }
        }
        once_2 = false;
      }  ///




    }


    if (sel) {

      conn = false;
      e = 0;
      tim = 0;
      extract(store, incr, net);

      display.clearDisplay();
      display.print("Attempting to connect to ");
      display.print(store);

      Serial1.print("AT+CWJAP=");
      Serial1.print("\"");
      Serial1.print(store);
      Serial1.print("\"");
      Serial1.print(",");
      Serial1.println("\"" "\"");
      milli(1000);


      store[0] = NULL;
      display.display();

      while (!conn) {
        while (Serial1.available()) {
          store[e] = Serial1.read();
          display.print(".");
          display.display();
          Serial.print(store[e]);  /// test........
          ++e;
        }

        tim = millis();
        while (!Serial1.available()) {

          store[e] = '\0';
          if (millis() - tim >= 13000) {
            display.clearDisplay();
            display.display();

            conn = true;
            break;
          }

        }

      } /// end of while !conn

      if (strstr(store, "OK")) {
        serve = true;
        new_ra = true;

        Serial1.println("AT+CIFSR");
        milli(1000);
        conn = false;
        store[0] = NULL;
        e = 0;
        while (!conn) {
          while (Serial1.available()) {
            store[e] = Serial1.read();
            display.print(".");
            display.display();
            Serial.print(store[e]);  /// test........
            ++e;
          }

          tim = millis();
          while (!Serial1.available()) {

            store[e] = '\0';
            if (millis() - tim >= 3000) {
              conn = true;
              char g[20];

              if (strstr(store , "STAIP,\"")) {



                int e = strlen(store);

                int a = 0;

                for ( int n = 0 ; n < e ; ++n) {

                  if (store[n] == '"') {

                    for ( int p = n + 1 ; p < e ; ++p, ++a) {
                      if (store[p] == '"') {
                        break;
                      }

                      g[a] = store[p];
                    }
                    g[a] = '\0';
                    break;
                  }


                }
                Serial.println(g);

              } ////


              char t[] = "TCP";
              Serial1.print("AT+CIPSTART=");
              Serial1.print("\"");
              Serial1.print(t);
              Serial1.print("\"");
              Serial1.print(",");
              Serial1.print("\"192.168.43.178\"");
              Serial1.print(",");
              Serial1.println("80");
              milli(2000);
              check();


              char dat[] = "GET /updateip.php?ip=";
              char dat2[] = "&username=Victor+Tomoloju";

              int len = strlen(dat) + strlen(dat2) + strlen(g) + 2;
              Serial1.print("AT+CIPSEND=");
              Serial1.println(len);
              milli(1000);
              check();


              Serial1.print(dat);
              Serial1.print(g);
              Serial1.println(dat2);
              milli(2000);
              check();


              Serial1.println("AT+CIPMUX=1");
              milli(1000);
              check();


              Serial1.println("AT+CIPSERVER=1,334");
              milli(1000);
              check();


              break;
            }   /// end of if mill




          }  //// end


        } /// end of while !conn
        display.clearDisplay();
        display.print("CONNECTED!");
        display.display();
        milli(500);
        net_re();
        activate_apps[2] = true;

      }
      else {
        display.clearDisplay();
        display.print("Connection Failed!");
        display.display();
        milli(500);
        net_re();
        serve = false;
        activate_apps[2] = true;

      }
      sel = false;
    }  ///
    /// temporary test....


  }

  else {
    display.clearDisplay();
    display.print("CONNECTED!");
    display.display();
    milli(500);
    net_re();
    activate_apps[2] = true;

  }

}
void net_re() {
  temp = 0 ;
  start = 0;
  point_pos_wi = 0;
  point_pos_w = 0;
  dat = 0;
  data[0] = NULL;
  data[dat] = 0;
  ++dat;
  sel = false;
  incr = 1;
  connect_t = false;
  connect_t2 = false;
  new_ra = true;
  once = true;
  once_2 = true;
  bc = 0;
  endnum = 0;
  end_d[0] = NULL;
  end_d[endnum] = 0;
  ++endnum;
  checkin = 0;

  end_o = 0;
  rem = 0;
  se[0] = 1;
  se[1] = 0;

}

void wif_sat() {


  if (new_ra) {

    char store[50];
    store[0] = NULL;
    uint8_t a = 0;
    uint16_t tim = 0;
    display.clearDisplay();
    display.print("Checking Status....");
    display.display();

    Serial1.println("AT+CWJAP?");
    milli(1000);
    bool conn = false;
    while (!conn) {
      while (Serial1.available()) {
        store[a] = Serial1.read();
        Serial.print(store[a]);
        ++a;
      }

      tim = millis();
      while (!Serial1.available()) {
        store[a] = '\0';
        if (millis() - tim >= 3000) {

          conn = true;
          break;
        }

      }



    } /// end of while !conn


    if (strstr(store, "OK") && !strstr(store, "No AP")) {

      display.clearDisplay();
      display.setTextSize(1);
      display.setCursor(6 , 0);
      display.setTextColor(BLACK);  ////<----- fix
      display.println("CONNECTED to :");
      display.display();

      uint8_t get_t = 0;
      uint8_t g;

      for (uint8_t i = 0; i < strlen(store); ++i) {
        if (store[i] == '"') {
          g = i;
          ++get_t;

          uint8_t p = i + 1;
          for (uint8_t u = p; u < strlen(store); ++u) {
            ++get_t;
            if (store[u] == '"' && store[u + 1] == ',') {

              store[u] = '\0';
              break;
            }
          }  ///
          break;
        }
      }  ///


      char wif_name[get_t + 2];

      uint8_t aa = 0;
      for (uint8_t i = g + 1; i < strlen(store); ++i, ++aa) {
        wif_name[aa] = store[i];
      }
      wif_name[aa] = '\0';

      display.println(wif_name);
      display.display();



      display.print(">");
      display.print("Disconnect");
      display.display();
      button_actions();


    }  /// if strstr......

    else {
      serve = false;
      display.clearDisplay();
      display.print("NOT CONNECTED");
      display.display();
    }

    new_ra = false;
  } ///

  if (sel) {

    display.clearDisplay();
    display.print("disconnecting...");
    display.display();

    Serial1.println("AT+CWQAP");
    milli(1000);

    bool conn = false;
    uint8_t a = 0;
    unsigned long long tim = 0;
    char store[20];

    while (!conn) {

      while (Serial1.available()) {
        store[a] = Serial1.read();
        ++a;
        display.print(".");
        display.display();
      }

      tim = millis();

      while (!Serial1.available()) {
        store[a] = '\0';
        if (millis() - tim >= 3000) {
          conn = true;
          Serial1.println("AT+CIPSERVER=0");
          milli(500);
          check();
          Serial1.println("AT+CIPMUX=0");
          milli(500);
          check();

          break;
        }
      }///
      if (strstr(store, "OK")) {
        display.clearDisplay();
        display.print("DISCONNECTED!");
        display.display();
        milli(500);
        serve = false;
        net_re();
        activate_apps[2] = true;
      }
    } ///



    sel = false;
  }///
  button_actions();

}
