#include <SPI.h>

const int pin_SS        = 10;

void setup_ECG();
void writeRegister(byte reg, byte data);
byte readRegister(byte reg);
void readAllRegs();

SPISettings write_settings(4000000, MSBFIRST, SPI_MODE0);
SPISettings read_settings( 4000000, MSBFIRST, SPI_MODE1);

void setup() {
    Serial.begin(9600);
    pinMode(pin_SS, OUTPUT); 
    
    digitalWrite(pin_SS, HIGH);
    
    SPI.begin();  
    
    setupECG();
}
   
void setupECG()
{
    SPI.beginTransaction(write_settings);

    writeRegister(0x00,0x00);
    writeRegister(0x01, 0x12);
    
    writeRegister(0x02,0x19);
    writeRegister(0x03,0x00);
    writeRegister(0x04,0x00);
    writeRegister(0x05,0x00);
    writeRegister(0x06,0x00);
    writeRegister(0x07,0x0f);
    writeRegister(0x08,0xff);
    writeRegister(0x09,0x00);
    writeRegister(0x0a,0x07);
    writeRegister(0x0b,0x00);
    writeRegister(0x0c,0x04);
    writeRegister(0x0d,0x00);
    writeRegister(0x0e,0x00);
    writeRegister(0x0f,0x00);
    writeRegister(0x10,0x00);
    writeRegister(0x11,0x00);
    writeRegister(0x12,0x04);
    writeRegister(0x13,0x00);
    writeRegister(0x14,0x00);
    writeRegister(0x15,0x00);
    writeRegister(0x16,0x00);
    writeRegister(0x17,0x05);
    
    /*writeRegister(0x18,0x07);
    writeRegister(0x19,0x00);
    writeRegister(0x1a,0x21);
    writeRegister(0x1b,0x20);
    writeRegister(0x1c,0x20);
    writeRegister(0x1d,0x00);*/
    
    writeRegister(0x21,0x02);
    writeRegister(0x22,0x02);
    writeRegister(0x23,0x02);
    writeRegister(0x24,0x02);
    writeRegister(0x25,0x00);
    writeRegister(0x26,0x00);
    writeRegister(0x27,0x08);
    writeRegister(0x28,0x00);
    writeRegister(0x29,0x00);
    writeRegister(0x2a,0x00);
    
    /*writeRegister(0x2b,0x00);
    writeRegister(0x2c,0x00);
    writeRegister(0x2d,0x00);*/
    
    writeRegister(0x2e,0x33);
    writeRegister(0x2f,0x30);
    
    /*writeRegister(0x30,0x00);
    writeRegister(0x31,0x00);
    writeRegister(0x32,0x00);
    writeRegister(0x33,0x00);
    writeRegister(0x34,0x00);
    writeRegister(0x35,0x00);
    writeRegister(0x36,0x00);
    writeRegister(0x37,0x00);
    writeRegister(0x38,0x00);
    writeRegister(0x39,0x00);
    writeRegister(0x3a,0x00);
    writeRegister(0x3b,0x00);
    writeRegister(0x3c,0x00);
    writeRegister(0x3d,0x00);
    writeRegister(0x3e,0x00);
    writeRegister(0x3f,0x00);
    writeRegister(0x40,0x01);
    writeRegister(0x50,0x00);
    writeRegister(0x60,0x00);
    writeRegister(0x62,0x00);*/

    
    /*writeRegister(0x01, 0x20); //Connect channel 1’s INP to IN2 and INN to IN1.
    writeRegister(0x02, 0x19); //Connect channel 2’s INP to IN3 and INN to IN1.
    writeRegister(0x0A, 0x07); //Enable the common-mode detector on input pins IN1, IN2 and IN3.
    writeRegister(0x0C, 0x04); //Connect the output of the RLD amplifier internally to pin IN4.
    writeRegister(0x12, 0x04); //Use external crystal and feed the internal oscillator's output to the digital.
    writeRegister(0x14, 0x24); //Shuts down unused channel 3’s signal path.
    writeRegister(0x21, 0x02); //Configures the R2 decimation rate as 5 for all channels.
    writeRegister(0x22, 0x02); //Configures the R3 decimation rate as 6 for channel 1.
    writeRegister(0x23, 0x02); //Configures the R3 decimation rate as 6 for channel 2.
    writeRegister(0x27, 0x08); //Configures the DRDYB source to channel 1 ECG (or fastest channel).
    writeRegister(0x2F, 0x30); //Enables channel 1 ECG and channel 2 ECG for loop read-back mode.
    writeRegister(0x00, 0x01); //Starts data conversion.*/

    Serial.println("********************");

    readAllRegs();

    SPI.endTransaction();
 
}


void readAllRegs()
{
    while(Serial.read() != 'g') {}
    
    readRegister(0x00);
    readRegister(0x01);
    readRegister(0x02);
    readRegister(0x03);
    readRegister(0x04);
    readRegister(0x05);
    readRegister(0x06);
    readRegister(0x07);
    readRegister(0x08);
    readRegister(0x09);
    readRegister(0x0a);
    readRegister(0x0b);
    readRegister(0x0c);
    readRegister(0x0d);
    readRegister(0x0e);
    readRegister(0x0f);
    readRegister(0x10);
    readRegister(0x11);
    readRegister(0x12);
    readRegister(0x13);
    readRegister(0x14);
    readRegister(0x15);
    readRegister(0x16);
    readRegister(0x17);
    readRegister(0x21);
    readRegister(0x22);
    readRegister(0x23);
    readRegister(0x24);
    readRegister(0x25);
    readRegister(0x26);
    readRegister(0x27);
    readRegister(0x28);
    readRegister(0x29);
    readRegister(0x2a);
    readRegister(0x2e);
    readRegister(0x2f);


}

void loop() 
{
    //setupECG();

}


byte readRegister(byte reg)
{
    byte data;
    
    Serial.print(reg, HEX);
    
    reg |= 1 << 7; //set read/write bit of command field to 1
    
    digitalWrite(pin_SS, LOW);
    
    SPI.transfer(reg);  //transfer read register address with read bit set
    data = SPI.transfer(0); //transfer dummy data in order to get back the read data from the SPI bus
    
    digitalWrite(pin_SS, HIGH);

    Serial.print(" = ");
    Serial.println(data, HEX);
    return data;  //return byte read from the SPI bus
}

void writeRegister(byte reg, byte data)
{
    Serial.print(reg, HEX);
    reg &= ~(1 << 7);  //set read/write bit of command field to 0
    //bool s = false;
    byte miso = -1;
    
    while(1) {
        reg |= 1 << 7; //set read/write bit of command field to 1
    
        digitalWrite(pin_SS, LOW);
    
        SPI.transfer(reg);  //transfer read register address with read bit set
        miso = SPI.transfer(0); //transfer dummy data in order to get back the read data from the SPI bus
        
        digitalWrite(pin_SS, HIGH);

        if(miso != data) {
            reg &= ~(1 << 7);
        
            digitalWrite(pin_SS, LOW);
    
            SPI.transfer(reg);
            SPI.transfer(data);
        
            digitalWrite(pin_SS, HIGH);
            
        }
        else {
            break;
            //s = true;   
        }
    }    
    
    //Serial.println(data);
    Serial.print(" = ");
    Serial.print(miso, HEX);
    Serial.println(" DONE");
}


