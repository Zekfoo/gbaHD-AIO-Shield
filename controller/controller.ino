/* 
 *  Modified from https://github.com/zwenergy/gbaHD
 *  Some code snippets also borrowed from https://github.com/ManCloud/GBAHD-Shield
 *  Edit GBA_RESET_COMBO and OSD_COMBO to configure the button combos for GBA reset and OSD toggle respectively
*/

#include <EEPROM.h>

//// Pin definitions

// GBA outputs
#define DUP 2
#define DDOWN 3
#define DLEFT 4
#define DRIGHT 5
#define BUTA 6
#define BUTB 7
#define BUTL 8
#define BUTR 9
#define BUTSTART 10
#define BUTSELECT 11
#define GBARESET 12

// SNES controller
#define SNESLATCH A0
#define SNESCLK A1
#define SNESSERIAL A2

// Communication port
#define COMPIN A3

// Polling interval in ms
#define POLLINT 10

// Some NOP definitions.
// Yes, this is very pretty. But we do not have code size problems,
// hence using stupid sequences of NOPs makes it nicely timing 
// predictable.
#define NOP1 asm volatile( "nop\n" )
#define NOP2   NOP1;  NOP1
#define NOP4   NOP2;  NOP2
#define NOP8   NOP4;  NOP4
#define NOP16  NOP8;  NOP8
#define NOP32  NOP16; NOP16;
#define NOP64  NOP32; NOP32;
#define NOP128 NOP64; NOP64;

#define WAIT4US  NOP64
#define WAIT10US NOP128; NOP32;
#define WAIT14US NOP128; NOP64; NOP32;


// OSD config struct
typedef struct config_tag {
  uint8_t smoothing :2;
  uint8_t pixelGrid :2;
  uint8_t multGrid :1;
  uint8_t color :1;
  uint8_t freq :1;
  uint8_t padDisp :1;
} config_t;

// Pad display offsets
#define PAD_DISP_A      (0x010)
#define PAD_DISP_B      (0x020)
#define PAD_DISP_UP     (0x001)
#define PAD_DISP_DOWN   (0x002)
#define PAD_DISP_LEFT   (0x004)
#define PAD_DISP_RIGHT  (0x008)
#define PAD_DISP_L      (0x040)
#define PAD_DISP_R      (0x080)
#define PAD_DISP_START  (0x100)
#define PAD_DISP_SELECT (0x200)
#define PAD_DISP_MASK   (0x3FF)

// Controller state offsets
#define CON_UP     (0x001)
#define CON_DOWN   (0x002)
#define CON_LEFT   (0x004)
#define CON_RIGHT  (0x008)
#define CON_A      (0x010)
#define CON_B      (0x020)
#define CON_L      (0x040)
#define CON_R      (0x080)
#define CON_START  (0x100)
#define CON_SELECT (0x200)
#define CON_X      (0x400)
#define CON_Y      (0x800)
#define CON_MASK   (0xFFF)

// Button combo macros
#define GBA_RESET_COMBO (CON_L | CON_R | CON_SELECT | CON_START)
#define OSD_COMBO       (CON_L | CON_R | CON_X | CON_Y)

uint16_t controllerState, lastState; // b000000, Select, Start, R, L, B, A, Right, Left, Down, Up}

uint8_t gbaReset;

uint8_t gotCombo;

uint8_t showOSD;

// Read the controller
void readController() {
  controllerState = 0;
  // Set the latch to high for 12 us.
  digitalWrite( SNESLATCH, 1 );
  delayMicroseconds( 12 );
  digitalWrite( SNESLATCH, 0 );
  delayMicroseconds( 6 );

  // Shift in the controller data.
  for ( uint8_t i = 0; i < 16; ++i ) {
    digitalWrite( SNESCLK, 0 );
    delayMicroseconds( 6 );
    // We only care about the first 12 bits.
    if ( i < 12 ) {
      uint8_t curDat = !digitalRead( SNESSERIAL );
      if (curDat) {
        switch ( i ) {
          case 0:
            controllerState |= CON_B;
            break;
          case 1:
            controllerState |= CON_Y;
            break;
          case 2:
            controllerState |= CON_SELECT;
            break;
          case 3:
            controllerState |= CON_START;
            break;
          case 4:
            controllerState |= CON_UP;
            break;
          case 5:
            controllerState |= CON_DOWN;
            break;
          case 6:
            controllerState |= CON_LEFT;
            break;
          case 7:
            controllerState |= CON_RIGHT;
            break;
          case 8:
            controllerState |= CON_A;
            break;
          case 9:
            controllerState |= CON_X;
            break;
          case 10:
            controllerState |= CON_L;
            break;
          case 11:
            controllerState |= CON_R;
            break;
          default:
            break;
        }
      }
    }
    digitalWrite( SNESCLK, 1 );
    delayMicroseconds( 6 );
  }
}

// Drive GBA signals
void updateGBASignals() {
  // In case a button is pressed, we pull the signal to ground.
  // Otherwise, we set it to high Z.
  // Since all output registers are already set to 0, we only need
  // to toggle the port direction.
  // Get D0 and D1 directions.
  uint8_t dir = DDRD & 0b00000011;
  if (controllerState & CON_UP)    dir |= 1 << 2;
  if (controllerState & CON_DOWN)  dir |= 1 << 3;
  if (controllerState & CON_LEFT)  dir |= 1 << 4;
  if (controllerState & CON_RIGHT) dir |= 1 << 5;
  if (controllerState & CON_A)     dir |= 1 << 6;
  if (controllerState & CON_B)     dir |= 1 << 7;
  // Write them.
  DDRD = dir;

  // The next ones.
  dir = 0;
  if (controllerState & CON_L)      dir |= 1;
  if (controllerState & CON_R)      dir |= 1 << 1;
  if (controllerState & CON_START)  dir |= 1 << 2;
  if (controllerState & CON_SELECT) dir |= 1 << 3;
  // Write them.
  DDRB = dir; 
}

void rebootGBA(bool hold) {
  DDRB = 1 << 4;
  if (!hold) {
    delay(10);
    DDRB = 0;
  }
}

config_t config_byte = {0,0,0,0,0,0};
config_t config_byte_bak = {0,0,0,0,0,0};

void osdWriteConfig(void) {
    sendPacket(0x4000 | *((uint8_t*)(&config_byte)));
}

void osdInit(void){
    uint8_t tmpVal = EEPROM.read((uint8_t *)(E2END-1));
    if(tmpVal == 0xFF) tmpVal = 0;
    config_byte = *((config_t *)(&tmpVal));
    osdWriteConfig();
}

void osdEnter(void) {
    config_byte_bak = config_byte;
    if(config_byte.padDisp) {
        delay(10);
        sendPacket( (0x8000));
        delay(10);
        sendPacket( (0x8000));
        delay(10);
    }
}


#define OSD_MIN_IDX 3
#define OSD_MAX_IDX 8
bool osdUpdate() {
    bool retVal = true;

    static uint8_t osd_idx = OSD_MIN_IDX;
    static uint16_t last_buttons = 0;

    if(controllerState != last_buttons) {
        last_buttons = controllerState;
        if(controllerState) {
            bool updateOSD = true;
            switch(controllerState) {
                case CON_UP: {
                    osd_idx = ((osd_idx > OSD_MIN_IDX) ? (osd_idx - 1) : OSD_MIN_IDX);
                } break;
                case CON_DOWN: {
                    osd_idx = ((osd_idx < OSD_MAX_IDX) ? (osd_idx + 1) : OSD_MAX_IDX);
                } break;
                case CON_A: {
                    updateOSD = false;
                    switch (osd_idx){
                        case 3: config_byte.pixelGrid = ( (config_byte.pixelGrid < 3) ? ((config_byte.pixelGrid << 1) + 1) : 0); break;
                        case 4: config_byte.multGrid = !config_byte.multGrid; break;
                        case 5: config_byte.smoothing = ( (config_byte.smoothing < 2) ? (config_byte.smoothing + 1) : 0); break;
                        case 6: config_byte.color = !config_byte.color; break;
                        case 7: config_byte.freq = !config_byte.freq; break;
                        case 8: config_byte.padDisp = !config_byte.padDisp; break;
                    }
                    osdWriteConfig();
                } break;
                case CON_B: {
                    config_byte = config_byte_bak;
                    osdWriteConfig();
                    retVal = false;
                    osd_idx = OSD_MIN_IDX;
                } break;
                case CON_START: {
                    EEPROM.update((uint8_t *)(E2END-1), *(uint8_t *)(&config_byte));
                    retVal = false;
                    osd_idx = OSD_MIN_IDX;
                } break;
            }
            if(updateOSD) {
                delay(10);
                sendPacket( (0x2000) | (osd_idx << 1) | (retVal ? 1 : 0) );
                delay(10);
                sendPacket( (0x2000) | (osd_idx << 1) | (retVal ? 1 : 0) );
                delay(10);
            }
        }
    }
    return retVal;
}

void padUpdate() {
    uint16_t pad_display_word = 0x8000;
    if (controllerState & CON_A)      pad_display_word |= PAD_DISP_A;
    if (controllerState & CON_B)      pad_display_word |= PAD_DISP_B;
    if (controllerState & CON_UP)     pad_display_word |= PAD_DISP_UP;
    if (controllerState & CON_DOWN)   pad_display_word |= PAD_DISP_DOWN;
    if (controllerState & CON_LEFT)   pad_display_word |= PAD_DISP_LEFT;
    if (controllerState & CON_RIGHT)  pad_display_word |= PAD_DISP_RIGHT;
    if (controllerState & CON_L)      pad_display_word |= PAD_DISP_L;
    if (controllerState & CON_R)      pad_display_word |= PAD_DISP_R;
    if (controllerState & CON_START)  pad_display_word |= PAD_DISP_START;
    if (controllerState & CON_SELECT) pad_display_word |= PAD_DISP_SELECT;
    sendPacket(pad_display_word);
}


// Send packet to FPGA
void sendPacket( uint16_t p ) {
  for ( uint16_t i = 1; i != 0; i<<=1 ) {
    if (p & i) {
      PORTC &= B11110111;
      WAIT4US;
      PORTC |= B00001000;
      WAIT10US;
    } else {
      PORTC &= B11110111;
      WAIT14US;
      PORTC |= B00001000;
    }
  }
}

// Clear the button states
void noButtons() {
  controllerState = 0;
}

// For serial debug only
void debugOutput() {
  Serial.print("DEBUG: ");
  Serial.print(controllerState, BIN);
  Serial.print(" ");
  if(controllerState & CON_A)      Serial.print("A ");
  if(controllerState & CON_B)      Serial.print("B ");
  if(controllerState & CON_X)      Serial.print("X ");
  if(controllerState & CON_Y)      Serial.print("Y ");
  if(controllerState & CON_L)      Serial.print("L ");
  if(controllerState & CON_R)      Serial.print("R ");
  if(controllerState & CON_START)  Serial.print("START ");
  if(controllerState & CON_SELECT) Serial.print("SELECT ");
  if(controllerState & CON_UP)     Serial.print("UP ");
  if(controllerState & CON_DOWN)   Serial.print("DOWN ");
  if(controllerState & CON_LEFT)   Serial.print("LEFT ");
  if(controllerState & CON_RIGHT)  Serial.print("RIGHT ");
  if(gbaReset) Serial.print("RESET ");
  Serial.println();
}

void setup() {
  // DEBUG.
  Serial.begin(115200);
  Serial.println( "Startup" );
  
  // Set up pin modes.

  // Hold GBA reset low
  pinMode( GBARESET, OUTPUT );
  digitalWrite( GBARESET, 0 );
  
  // First set all output registers of GBA buttons to 0.
  pinMode( DUP, OUTPUT );
  digitalWrite( DUP, 0 );
  pinMode( DDOWN, OUTPUT );
  digitalWrite( DDOWN, 0 );
  pinMode( DLEFT, OUTPUT );
  digitalWrite( DLEFT, 0 );
  pinMode( DRIGHT, OUTPUT );
  digitalWrite( DRIGHT, 0 );
  pinMode( BUTA, OUTPUT );
  digitalWrite( BUTA, 0 );
  pinMode( BUTB, OUTPUT );
  digitalWrite( BUTB, 0 );
  pinMode( BUTL, OUTPUT );
  digitalWrite( BUTL, 0 );
  pinMode( BUTR, OUTPUT );
  digitalWrite( BUTR, 0 );
  pinMode( BUTSTART, OUTPUT );
  digitalWrite( BUTSTART, 0 );
  pinMode( BUTSELECT, OUTPUT );
  digitalWrite( BUTSELECT, 0 );

  // Then GBA pins are set to input to hold
  // them as high Z.
  pinMode( DUP, INPUT );
  pinMode( DLEFT, INPUT );
  pinMode( DRIGHT, INPUT );
  pinMode( DDOWN, INPUT );
  pinMode( BUTA, INPUT );
  pinMode( BUTB, INPUT );
  pinMode( BUTL, INPUT );
  pinMode( BUTR, INPUT );
  pinMode( BUTSTART, INPUT );
  pinMode( BUTSELECT, INPUT );
  // Release GBA reset
  pinMode( GBARESET, INPUT );

  // And the SNES controller pins.
  pinMode( SNESLATCH, OUTPUT );
  digitalWrite( SNESLATCH, 0 );
  pinMode( SNESCLK, OUTPUT );
  digitalWrite( SNESCLK, 1 );
  pinMode( SNESSERIAL, INPUT_PULLUP );

  // Communication pin.
  pinMode( COMPIN, OUTPUT );
  digitalWrite( COMPIN, 1 );

  rebootGBA(true);
  delay(5000); // Placeholder delay to wait for FPGA to be programmed
  osdInit();
  rebootGBA(false);
}

void loop() {
  readController();

  if (lastState != controllerState) {
      lastState = controllerState;
      switch (controllerState)
      {
          case GBA_RESET_COMBO: {
              rebootGBA(false);
          } break;
          case OSD_COMBO: {
              osdEnter();
              showOSD = true;
          } break;
      }
  }
  
  if (showOSD) {
      showOSD = osdUpdate();
      noButtons();    //clear controller data as long as OSD is enabled
  } else if(config_byte.padDisp) {
      padUpdate();
  }

  // Only update GBA signals if the menu is not active.
  if ( !showOSD ) {
    //debugOutput();
    updateGBASignals();
  }
}
