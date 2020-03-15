
unsigned long last_time          = 0;
unsigned long actualizar         = 0;
unsigned long lastMicros         = 0;
unsigned long antiBounceButtons  = 0;
unsigned long antiBounceJoyStick = 0;
unsigned long playButton         = 0;

#define antiBounceButtonsTime     50  // mseg
#define antiBounceJoyStickTime    50  // mseg
#define playButtonTime          3000  // mseg

#define LE_GREEN_PIN    11
#define LE_RED_PIN      10
#define LE_BLUE_PIN      9

#define LC_GREEN_PIN     6
#define LC_RED_PIN       5
#define LC_BLUE_PIN      3


#define J_DOWN       12
#define J_UP          8
#define J_RIGHT       7
#define J_LEFT        4

#define B_LED_REPAIR     15
#define B_LED_UPGRADE    16
#define B_LED_EMPLOYEE   17
#define B_REPAIR         14
#define B_UPGRADE        19
#define B_EMPLOYEE        2

String JoyStick = "";
String Button   = "";

String inputString  = "";      // a String to hold incoming data
bool stringComplete = false;   // whether the string is complete

String FREQ = "";
String FUN = "";
String COL = "";

int FadeIndex  = 1;
int FadeAmount = 1;

char lightType = "";

void setup()
{
  Serial.begin(115200);

  pinMode ( LE_RED_PIN   , OUTPUT ) ;
  pinMode ( LE_GREEN_PIN , OUTPUT ) ;
  pinMode ( LE_BLUE_PIN  , OUTPUT ) ;

  pinMode ( LC_RED_PIN   , OUTPUT ) ;
  pinMode ( LC_GREEN_PIN , OUTPUT ) ;
  pinMode ( LC_BLUE_PIN  , OUTPUT ) ;

  pinMode ( J_UP    , INPUT_PULLUP ) ;
  pinMode ( J_RIGHT , INPUT_PULLUP ) ;
  pinMode ( J_DOWN  , INPUT_PULLUP ) ;
  pinMode ( J_LEFT  , INPUT_PULLUP ) ;

  pinMode ( B_EMPLOYEE , INPUT_PULLUP ) ;
  pinMode ( B_UPGRADE  , INPUT_PULLUP ) ;
  pinMode ( B_REPAIR   , INPUT_PULLUP ) ;

  pinMode ( B_LED_EMPLOYEE , OUTPUT ) ;
  pinMode ( B_LED_UPGRADE  , OUTPUT ) ;
  pinMode ( B_LED_REPAIR   , OUTPUT ) ;

  digitalWrite ( LE_RED_PIN   , LOW) ;
  digitalWrite ( LE_GREEN_PIN , LOW) ;
  digitalWrite ( LE_BLUE_PIN  , LOW) ;

  digitalWrite ( LC_RED_PIN   , LOW) ;
  digitalWrite ( LC_GREEN_PIN , LOW) ;
  digitalWrite ( LC_BLUE_PIN  , LOW) ;

  digitalWrite ( B_LED_EMPLOYEE , LOW ) ;
  digitalWrite ( B_LED_UPGRADE  , LOW ) ;
  digitalWrite ( B_LED_REPAIR   , LOW ) ;
}

void loop()
{
  // Print a heartbeat
  if ( millis() > last_time + 2000 )
  {
    //Serial.print("XaviaGame esta prendido desde hace:\t"); Serial.print(millis()); Serial.println("\tmseg.");
    last_time = millis();
  }

  if (Serial.available())
    serialEvent();

  if ( FUN == "L" )
  {
    if ( (micros() - lastMicros) > FREQ.toInt() )
    {
      if ( FadeIndex <= 0 || FadeIndex >= 255 )
        FadeAmount = -FadeAmount;

      FadeIndex += FadeAmount;

      switch (COL[0])
      {
        case 'K':
          digitalWrite( LE_RED_PIN   , LOW );
          digitalWrite( LE_GREEN_PIN , LOW );
          digitalWrite( LE_BLUE_PIN  , LOW );
          break;

        case 'B':
          digitalWrite ( LE_RED_PIN   , LOW );
          digitalWrite ( LE_GREEN_PIN , LOW );
          analogWrite  ( LE_BLUE_PIN  , FadeIndex );
          break;

        case 'M':
          analogWrite  ( LE_RED_PIN   , FadeIndex );
          digitalWrite ( LE_GREEN_PIN , LOW );
          analogWrite  ( LE_BLUE_PIN  , FadeIndex );
          break;

        case 'R':
          analogWrite  ( LE_RED_PIN   , FadeIndex );
          digitalWrite ( LE_GREEN_PIN , LOW );
          digitalWrite ( LE_BLUE_PIN  , LOW );
          break;

        case 'C':
          digitalWrite( LE_RED_PIN   , LOW );
          analogWrite ( LE_GREEN_PIN , FadeIndex );
          analogWrite ( LE_BLUE_PIN  , FadeIndex );
          break;

        case 'W':
          analogWrite( LE_RED_PIN ,   FadeIndex );
          analogWrite( LE_GREEN_PIN , FadeIndex );
          analogWrite( LE_BLUE_PIN ,  FadeIndex );
          break;

        case 'Y':
          analogWrite  ( LE_RED_PIN   , FadeIndex );
          analogWrite  ( LE_GREEN_PIN , FadeIndex );
          digitalWrite ( LE_BLUE_PIN  , LOW );
          break;

        case 'G':
          digitalWrite ( LE_RED_PIN   , LOW );
          analogWrite  ( LE_GREEN_PIN , FadeIndex );
          digitalWrite ( LE_BLUE_PIN  , LOW );
          break;
      }
      lastMicros = micros();
    }
  }

  else if ( FUN == "F" )
  {
    // Acá va el código que deja la Luz Común fija. Hardcodear la FREQ = 0

    switch (COL[0])
    {
      case 'K':
        digitalWrite( LE_RED_PIN   , LOW );
        digitalWrite( LE_GREEN_PIN , LOW );
        digitalWrite( LE_BLUE_PIN  , LOW );
        break;

      case 'B':
        digitalWrite ( LE_RED_PIN   , LOW );
        digitalWrite ( LE_GREEN_PIN , LOW );
        digitalWrite  ( LE_BLUE_PIN  , HIGH );
        break;

      case 'M':
        digitalWrite  ( LE_RED_PIN   , HIGH );
        digitalWrite ( LE_GREEN_PIN , LOW );
        digitalWrite  ( LE_BLUE_PIN  , HIGH );
        break;

      case 'R':
        digitalWrite  ( LE_RED_PIN   , HIGH );
        digitalWrite ( LE_GREEN_PIN , LOW );
        digitalWrite ( LE_BLUE_PIN  , LOW );
        break;

      case 'C':
        digitalWrite( LE_RED_PIN   , LOW );
        digitalWrite ( LE_GREEN_PIN , HIGH );
        digitalWrite ( LE_BLUE_PIN  , HIGH );
        break;

      case 'W':
        digitalWrite( LE_RED_PIN ,   HIGH );
        digitalWrite( LE_GREEN_PIN , HIGH );
        digitalWrite( LE_BLUE_PIN ,  HIGH );
        break;

      case 'Y':
        digitalWrite ( LE_RED_PIN   , HIGH );
        digitalWrite ( LE_GREEN_PIN , HIGH );
        digitalWrite ( LE_BLUE_PIN  , LOW );
        break;

      case 'G':
        digitalWrite ( LE_RED_PIN   , LOW );
        digitalWrite ( LE_GREEN_PIN , HIGH );
        digitalWrite ( LE_BLUE_PIN  , LOW );
        break;
    }
  }

  if ( !digitalRead(J_UP) | !digitalRead(J_RIGHT) | !digitalRead(J_DOWN) | !digitalRead(J_LEFT) )
  {

    digitalWrite ( B_LED_REPAIR   , HIGH ) ;
    digitalWrite ( B_LED_UPGRADE  , HIGH ) ;
    digitalWrite ( B_LED_EMPLOYEE , HIGH ) ;

    if ( ( millis() - antiBounceJoyStick ) > antiBounceJoyStickTime )
    {
      if ( digitalRead(J_UP) == LOW )
      {
        JoyStick = "J_UP";
      }

      else if ( digitalRead(J_RIGHT) == LOW )
      {
        JoyStick = "J_RIGHT";
      }

      else if ( digitalRead(J_DOWN) == LOW )
      {
        JoyStick = "J_DOWN";
      }

      else if ( digitalRead(J_LEFT) == LOW )
      {
        JoyStick = "J_LEFT";
      }

      //Serial.print("JoyStick:\t"); //Serial.println(JoyStick);
      antiBounceJoyStick = millis();
    }
  }

  if ( ( millis() - playButton ) > playButtonTime )
  {
    JoyStick = "";
    digitalWrite ( B_LED_REPAIR   , LOW ) ;
    digitalWrite ( B_LED_UPGRADE  , LOW ) ;
    digitalWrite ( B_LED_EMPLOYEE , LOW ) ;
    playButton = millis();
  }

  else // chequeo botones
  {
    if ( ( !digitalRead(B_EMPLOYEE) | !digitalRead(B_UPGRADE) | !digitalRead(B_REPAIR) ) & (JoyStick != "") )
    {
      if ( ( millis() - antiBounceButtons ) > antiBounceButtonsTime )
      {
        if ( digitalRead(B_EMPLOYEE) == LOW )
        {
          Button = "B_EMPLOYEE";
          Serial.print(JoyStick); Serial.print(":"); Serial.println(Button);

          digitalWrite ( B_LED_REPAIR   , LOW ) ;
          digitalWrite ( B_LED_UPGRADE  , LOW ) ;
          digitalWrite ( B_LED_EMPLOYEE , LOW ) ;
          delay(150);
          digitalWrite ( B_LED_EMPLOYEE , HIGH ) ;
          delay(150);
          digitalWrite ( B_LED_EMPLOYEE , LOW ) ;
          delay(150);
          digitalWrite ( B_LED_EMPLOYEE , HIGH ) ;
          delay(150);
          digitalWrite ( B_LED_EMPLOYEE , LOW ) ;

        }

        else if ( digitalRead(B_UPGRADE) == LOW )
        {
          Button = "B_UPGRADE";
          Serial.print(JoyStick); Serial.print(":"); Serial.println(Button);

          digitalWrite ( B_LED_REPAIR   , LOW ) ;
          digitalWrite ( B_LED_UPGRADE  , LOW ) ;
          digitalWrite ( B_LED_EMPLOYEE , LOW ) ;
          delay(150);
          digitalWrite ( B_LED_UPGRADE , HIGH ) ;
          delay(150);
          digitalWrite ( B_LED_UPGRADE , LOW ) ;
          delay(150);
          digitalWrite ( B_LED_UPGRADE , HIGH ) ;
          delay(150);
          digitalWrite ( B_LED_UPGRADE , LOW ) ;

        }

        else if ( digitalRead(B_REPAIR) == LOW )
        {
          Button = "B_REPAIR";
          Serial.print(JoyStick); Serial.print(":"); Serial.println(Button);

          digitalWrite ( B_LED_REPAIR   , LOW ) ;
          digitalWrite ( B_LED_UPGRADE  , LOW ) ;
          digitalWrite ( B_LED_EMPLOYEE , LOW ) ;
          delay(150);
          digitalWrite ( B_LED_REPAIR , HIGH ) ;
          delay(150);
          digitalWrite ( B_LED_REPAIR , LOW ) ;
          delay(150);
          digitalWrite ( B_LED_REPAIR , HIGH ) ;
          delay(150);
          digitalWrite ( B_LED_REPAIR , LOW ) ;

        }

        //Serial.print("Button:\t"); //Serial.println(Button);
        antiBounceButtons = millis();
      }
    }
  }

}


void serialEvent()
{
  inputString = "";
  stringComplete = false;

  //Serial.println("Evento de puerto Serie");

  while (Serial.available())
  {
    char inChar = (char) Serial.read();
    inputString += inChar;
    if (inChar == '\n')
      stringComplete = true;

    delay(10);
  }

  //Serial.println(inputString);

  switch ( inputString[0] )
  {
    case 'L':

      switch ( inputString[1] )
      {
        case 'E':

          lightType = true;

          if ( inputString[3] == '1' )
            Serial.println("Luz de Emergencia: ON");
          else
            Serial.println("Luz de Emergencia: OFF");

          break;

        case 'C':

          lightType = true;

          //Serial.println("Prender Luz Común");

          FUN = "";
          FREQ = "";
          COL = "";

          int i = 3;
          for ( i ; inputString[i] != ',' ; i++ )
          {
            FUN += inputString[i];
            delay(10);
          }

          //Serial.print("FUN: "); Serial.println(FUN);

          for ( ++i ; inputString[i] != ',' ; i++ )
          {
            FREQ += inputString[i];
            delay(10);
          }

          //Serial.print("FREQ: "); Serial.println(FREQ);

          for ( ++i ; inputString[i] != '.' ; i++ )
          {
            COL += inputString[i];
            delay(10);
          }

          //Serial.print("COL: "); Serial.println(COL);

          break;
      }

      inputString = "";
      stringComplete = false;
      break;
  }
}

/*
  ISR( TIMER0_COMPA_vect )
  {
  static unsigned long count = 0;
  if ( ++count > TIMER0_COUNT )
  {

    if (functionIndex < FUNCTION_LENGTH)
      analogWrite( LED_BUILTIN, LIN[++functionIndex] );
    else
      functionIndex = 0;

    count = 0;
    time = true;                          // time is set to true every TIMER0_COUNT ms
  }                                       // (time needs to be cleared in the main routine)
  }
*/
