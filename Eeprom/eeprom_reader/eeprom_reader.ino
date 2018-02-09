/* EEPROM Read
 *
 * Reads the value of each byte of the EEPROM and prints it 
 * to the computer.
 */

#include <EEPROM.h>

// start reading from the first byte (address 0) of the EEPROM
int address = 0;
byte value;

void setup()
{
  Serial.begin(9600);
  delay(1000);
  Serial.write("\033[2J");          // Clear screen
}

void loop()
{
  // read a byte from the current address of the EEPROM

  if ( address > 128 )
    return;
    
  value = EEPROM.read(address);

  if ( address % 8 == 0 )
  {
    Serial.println();
    Serial.print(address);
  }
  Serial.print("\t");
  Serial.print(value, DEC);
  
  // advance to the next address of the EEPROM
  address = address + 1;
  
  // there are only 512 bytes of EEPROM, from 0 to 511, so if we're
  // on address 512, wrap around to address 0
//  if (address == 4096)
//    address = 0;
  // Teensy 1.0 has 512 bytes
  // Teensy 2.0 has 1024 bytes
  // Teensy++ 1.0 has 2048 bytes
  // Teensy++ 2.0 has 4096 bytes
    
  // delay(500);
}
