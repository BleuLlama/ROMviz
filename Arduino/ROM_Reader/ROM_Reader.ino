// ROM_Reader
//
// 2013-10 Scott Lawrence
//         yorgle@gmail.com
//
// #include "MIT_Licenseh"
//


#define kVersionString "ROM Reader. v1.2. 2013-Oct. yorgle@gmail.com"
// v1.2 Added q-p to sepcify ROM size at runtime
//
// v1.1 Basic functionality
//
// v1.0 Initial version. Read 32 bytes.
//

// NOTE: refer to the "schematic" included in this repository for
//       connection information


////////////////////////////////////////////////////////////////////////////////
// speeds, pin assignments, etc...

// baud rate we're going to run at.
#define kBaud 115200

// how big our device is to start with
unsigned long nBytesDevice = 1024;

// output for 4 bit adder chips
#define kPinClock  14 // A0 Clock (adds 1 to address)
#define kPinClear  15 // A1 Clear (resets address to 0)
#define kPinEN     16 // A2 Output Enable (turns on the ROM)
#define kPinCE     17 // A3 Chip Enable (turns on the ROM)

// output for indicator LEDs
#define kLED_UV   10 // PWM (not used)
#define kLED_1    11 // PWM?
#define kLED_2    12 // Digital

// output from the ROM (Data lines)
#define kPinData0  2 // Rom Data 0x01 (D0)
#define kPinData1  3 // Rom Data 0x02 (D1)
#define kPinData2  4 // Rom Data 0x04 (D2)
#define kPinData3  5 // Rom Data 0x08 (D3)
#define kPinData4  6 // Rom Data 0x10 (D4)
#define kPinData5  7 // Rom Data 0x20 (D5)
#define kPinData6  8 // Rom Data 0x40 (D6)
#define kPinData7  9 // Rom Data 0x80 (D7)

////////////////////////////////////////////////////////////////////////////////

void setup() {
  // set up our connection to the 4 bit adders
  pinMode( kPinClock, OUTPUT );
  pinMode( kPinClear, OUTPUT );
  pinMode( kPinEN, OUTPUT );
  pinMode( kPinCE, OUTPUT );

  // set up our indicator LEDs
  pinMode( kLED_1, OUTPUT );
  pinMode( kLED_2, OUTPUT );
  pinMode( kLED_UV, OUTPUT );
  
  // set up our data inputs, turn off internal pullups
  for( int i=kPinData0 ; i < kPinData7 ; i++ ) 
  {
    pinMode( i, INPUT );
    digitalWrite( i, LOW );
  }

  // display something on the LEDs
  digitalWrite( kLED_1, LOW );
  analogWrite( kLED_2, 255 );
  digitalWrite( kLED_UV, HIGH );
  
  // get the ROM ready
  digitalWrite( kPinEN, LOW );

  // start up serial
  Serial.begin( kBaud );
  while (false ) { //!Serial) {
    ; // wait for serial port to connect. Needed for Leonardo only
  }
}

////////////////////////////////////////////////////////////////////////////////
// interface to the 4 bit adder chips

// clearRomCounter
//   resets the address to 0
void clearRomCounter()
{
  digitalWrite( kPinClear, HIGH );
  delay( 10 );
  digitalWrite( kPinClear, LOW );
  delay( 10 );
}

// pulseRomClock
//   adds '1' to the current address
void pulseRomClock()
{
  digitalWrite( kPinClock, HIGH );
  delayMicroseconds( 1 );
  digitalWrite( kPinClock, LOW );
}

// readData
//   reads in the current data value, returns it 
int readData()
{
  int retval = 0;
  if( digitalRead( kPinData0 )) retval |= 0x01;
  if( digitalRead( kPinData1 )) retval |= 0x02;
  if( digitalRead( kPinData2 )) retval |= 0x04;
  if( digitalRead( kPinData3 )) retval |= 0x08;
  if( digitalRead( kPinData4 )) retval |= 0x10;
  if( digitalRead( kPinData5 )) retval |= 0x20;
  if( digitalRead( kPinData6 )) retval |= 0x40;
  if( digitalRead( kPinData7 )) retval |= 0x80;
  
  return retval;
}



// dumpOne
//   dumps the rom out to serial, using set params, once
void dumpOne() {
  unsigned long x;
  char buf[64];
  // put your main code here, to run repeatedly: 

  Serial.println( ".oOo.oOo." );
  Serial.println( millis() );
  sprintf( buf, "%04lx", nBytesDevice );
  Serial.print( buf );
  x=0;
  clearRomCounter(); // reset the counter to 0
  while( x < nBytesDevice ) {
    analogWrite( kLED_1, x & 0xff );
    
    sprintf( buf, "\n%04x ", x );
    Serial.print( buf ); 
    
    for( int b=0 ; b<64 ; b++ ) {
      int v = readData();
      sprintf( buf, "%02x", v );
      Serial.print( buf );
      pulseRomClock();
      x++;
    }
  }
  Serial.println( "" );
}


////////////////////////////////////////////////////////////////////////////////
// user interface

// setSize
//   sets the read size and displays the part number
void setSize( unsigned long bytes, unsigned long partNo )
{
  char buf[20];
    nBytesDevice = bytes;
    Serial.println( "" );
    Serial.println( "Device change:" );
    Serial.print( "  Part: " );
    Serial.println( partNo, DEC );
    sprintf( buf, "  Size: %lu bytes", bytes );
    Serial.println( buf );
}

// variables for the "display one again" timer
unsigned long nextMillis = 0;
int  interval = 1000;


// loop
//   check for user input via serial
//   check for timer tick/automatic dump interval 
void loop() {
  int x = 0;

  // check for another serial byte from the user
  if( Serial.available() > 0 ) {
    x = Serial.read();
  }

  // force a manual dump
  if( x == 'd' ) {
    dumpOne();
  }
  
  // dump if the timer is up
  if( nextMillis != 0 ) {
    if( millis() >= nextMillis ) {
      dumpOne();
      nextMillis = millis() + interval;
    }
  }
  
  // toggle automatic mode
  if( x == 'a' ) {
    if( nextMillis != 0 ) {
      nextMillis = 0;
      Serial.println( "Auto mode disabled" );
    } else {
      nextMillis = millis();
      Serial.println( "Auto Mode enabled" );
    }
  }
  
  // set the automatic mode interval
  if( x >= '0' && x <= '9' ) {
    interval = ((x - '0') * 1000)/2;
    Serial.print( "Interval changed to " );
    Serial.print( interval, DEC );
    Serial.println( "ms." );
  }
  
  // set the EPROM size and part number
  if( x == 'q' ) { setSize( 256, 2702 ); }
  if( x == 'w' ) { setSize( 512, 2704 ); }
  if( x == 'e' ) { setSize( 1024, 2708 ); }
  if( x == 'r' ) { setSize( 2048, 2716 ); }
  if( x == 't' ) { setSize( 4096, 2732 ); }
  if( x == 'y' ) { setSize( 8192, 2764 ); }
  if( x == 'u' ) { setSize( 16384, 27128 ); }
  if( x == 'i' ) { setSize( 32768, 27256 ); }
  if( x == 'o' ) { setSize( 65536, 27512 ); }
  
  // display help and version info
  if( x == 'h' ) {
    Serial.println( kVersionString );
    Serial.println( " 'd'     - do a single dump." );
    Serial.println( " 'a'     - toggle auto-repeat dump." );
    Serial.println( " '0'-'9' - set auto repeat ms interval. ");
    Serial.println( " 'q'-'o' - set EPROM device size." );

  }
}
