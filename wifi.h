void get_creds(char *t , const char *a , int num) {
  bool yes = false;
  short secu = 0;
  int next = 0;
  int cha = 0;

  while (num) {

    int aa = 0;

    for (int i = next ; aa < strlen(a) ; ++i) {
      if (t[i] == a[aa]) {
        yes = true;
        t[i] = ' ';
        secu = i;

        ++aa;
      }

      else {
        yes = false;
      }
    }
    int j = secu + 1;      ///

    for (int i = next ; i < strlen(t) ; ++i , ++j) {
      t[i] = t[j];                                     /// covers up space..
    }

    secu = next;

    if (yes) {


      if (t[secu] == '0') {
        t[secu] = 'O';
      }
      else if (t[secu] > '0') {
        t[secu] = 'C';
      }

      for (int i = secu + 1 ; i < strlen(t); ++i) {

        if (t[i] == '"') {


          t[i] = ' ';
          for (int p = i + 1; p < strlen(t) ; ++p) {
            cha = p;
            if (t[p] == '"') {
              t[p] = ' ';
              break;
            }

          }

          break;
        }


      }
      ++cha;

      for (int i = cha ; i < strlen(t) ; ++i) {

        if (t[i] == '\n') {

          next = i + 1;
          break;
        }

        else {
          t[i] = ' ';
        }
      }

      int j = next - 1;  /// -> newline

      /// cha-1 = "...
      for (int i = cha - 1 ; i < strlen(t) ; ++i) {
        t[i] = t[j];
        ++j;

      }
      next = cha;


    } ///
    --num;
  }
  t[next] = '\0';


}

void extract(char *ptr , int num , char *net) {

  for (int i = 0  ; i < strlen(ptr) ; ++i) {

    if (num == 1) {

      for (int p = 0; p < strlen(ptr) ; ++p) {

        if (ptr[p] == '\n') {
          ptr[p] = '\0';
          break;
        }

      }

      break;
    }   ///

    else if (num > 1) {

      int count_t = 1;
      for (int i = 0 ; i < strlen(ptr) ; ++i) {

        if (ptr[i] == '\n') {
          ++count_t;

          if (count_t == num) {

            int ff = i + 1;

            for (int t = 0; t < strlen(ptr) ; ++t , ++ff) {
              ptr[t] = ptr[ff];
            }
            for (int t = 0; t < strlen(ptr) ; ++t) {
              if (ptr[t] == '\n') {
                ptr[t] = '\0';
                break;
              }
            }
            break;
          }

        } ///


      } ///

      break;

    } ///

  }

  if (ptr[0] == 'C') {

    net[0] = 'C';

  }
  else if (ptr[0] == 'O') {
    net[0] = 'O';
  }

  net[strlen(net)] = '\0';

  for (int i  = 0 ; i < strlen(ptr) ; ++i) {
    int ff;

    if (ptr[i] == ' ' && ptr[i - 1] == ',') {
      ff = i;
      ++ff;

      for (int t = 0 ; t < strlen(ptr) ; ++t, ++ff) {
        ptr[t] = ptr[ff];
      }
      break;
    }

  }

}  ///

void milli(uint16_t x) {

  unsigned long long current = millis();
  while (1)
  {
    if (  (millis() - current)  >= x ) {

      break;                                               /////         delay(funtion using millis()............
    }

  }

}
