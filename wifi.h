
void verify(char *store , char *check) {

  unsigned short re = 0;
  bool rest = false;


  while (1) {

    if (rest) {

      break;
    }


    if (Serial1.available() > 0) {
      store[re] = Serial1.read();
      Serial.print(store[re]);
      ++re;
    }
    short key = 0;
    short count = 0;
    short key2 = 0;
    short count2 = 0;

    for (short i = 0; i < re + 1; ++i) {
      key = 0;
      count = 0;
      key2 = 0;
      count2 = 0;

      if (rest) {
        break;
      }
      if ( store[i] == check[key]) {


        ++key;
        ++count;
        for (short p = i + 1; p < re + 1; ++p) {

          if (store[p] != check[key] ) {

            break;
          }

          if ( store[p] == check[key]) {
            ++count;

            if (count == strlen(check)) {
              rest = true;
              break;

            }
            ++key;
          }
        }

      }

    }


  }

}

void check() {

  bool found = false;
  char store[50];
  int a = 0;
  long long tim = 0;
  while (!found) {

    while (Serial1.available()) {

      store[a] = Serial1.read();
      Serial.print(store[a]);
      ++a;
    }

    tim = millis();

    while (!Serial1.available()) {
      if ( (millis() - tim) >= 3000) {
        Serial.println();

        found = true;
        break;
      }
    }
  }
}
