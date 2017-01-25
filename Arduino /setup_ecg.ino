#include <SPI.h>
#include <GSM.h>

#define PINNUMBER ""

const int pin_SS        = 10;
const int pin_DRDYB     = 2;

GSM gsmAccess;
GSM_SMS sms;

//Array to hold the number a SMS is retreived from
char senderNumber[20];

void setup_ECG();
void writeRegister(byte reg, byte data);
byte readRegister(byte reg);
byte readRegister50(byte reg);

void readAllRegs();
void read_ECG();

float ecg_vals[11][12] = {
                {-0.012091,-0.012364,-0.012182,-0.012818,-0.012909,-0.013000,-0.012818,-0.013636,-0.013818,-0.014545},
                {-0.015273,-0.015545,-0.015727,-0.015909,-0.015727,-0.015818,-0.015727,-0.015727,-0.016364,-0.016455},
                {-0.016545,-0.016091,-0.016182,-0.015909,-0.015727,-0.015545,-0.015909,-0.015545,-0.014727,-0.013909},
                {-0.012636,-0.009455,-0.007364,-0.006273,-0.004909,-0.005636,-0.008727,-0.011182,-0.013182,-0.015182},
                {-0.015455,-0.015818,-0.015545,-0.015364,-0.016455,-0.021273,-0.022000,-0.022182,-0.001455,0.043091},
                {0.102545,0.138636,0.129000,0.102909,0.002636,-0.049182,-0.047636,-0.034000,-0.024000,-0.014273},
                {-0.012091,-0.010545,-0.009091,-0.007455,-0.005182,-0.003909,-0.001818,0.000091,0.002000,0.004273},
                {0.007364,0.010182,0.014273,0.017909,0.022455,0.027636,0.033364,0.038909,0.043182,0.046273},
                {0.048636,0.047545,0.043455,0.037455,0.028727,0.019455,0.009727,0.001455,-0.004455,-0.008727},
                {-0.011818,-0.013091,-0.014364,-0.013818,-0.014182,-0.014364,-0.013636,-0.013727,-0.014182,-0.013636},
                {-0.013273,-0.012636,-0.012364,-0.012273,-0.011182,-0.011727,-0.012182,-0.012455,-0.012364,-0.012091}
                            };




void setup() {
    int i = 0, j = 0;

    Serial.begin(9600);

    pinMode(pin_SS, OUTPUT);
    digitalWrite(pin_SS, HIGH);

    pinMode(pin_DRDYB, INPUT_PULLUP);

    //digitalWrite(pin_DRDYB, HIGH);

    SPI.setBitOrder(MSBFIRST);
    SPI.setDataMode(SPI_MODE0);
    //SPI.setClockDivider(SPI_CLOCK_DIV8);
    SPI.begin();

    setupECG();

    //connection state
    boolean notConnected = true;

      //Start GSM connection
    while (notConnected) {
        if (gsmAccess.begin(PINNUMBER) == GSM_READY)
            notConnected = false;
            else {
                Serial.println("Not connected");
                delay(1000);
            }
    }

    Serial.println("GSM initialized");
    Serial.println("Waiting for messages");

    char msg_to_send[160];

    for(i = 0; i < 11; i++) {
        for(j = 0; j < 160; j++) {
            sprintf(msg_to_send, "%f", ecg_vals[i][j]);
        }
        sendSMS(msg_to_send, "6504650205");
    }

    attachInterrupt(digitalPinToInterrupt(pin_DRDYB), read_ECG, FALLING);

    //Serial.println("done attaching interrupt");

}

void read_ECG()
{
    int i;
    uint8_t ecg1[3];
    unsigned long ecg1_data;
    ecg1_data = 0;
    uint8_t ecg2[3];
    unsigned long ecg2_data;
    ecg2_data = 0;

    digitalWrite(pin_SS, LOW);

    SPI.transfer(0xD0);
    ecg1[0] = SPI.transfer(0);
    ecg1_data |= ecg1[0];

    SPI.transfer(0xD0);
    ecg1[1] = SPI.transfer(0);
    ecg1_data = (ecg1_data << 8) | ecg1[1];

    SPI.transfer(0xD0);
    ecg1[2] = SPI.transfer(0);
    ecg1_data = (ecg1_data << 8) | ecg1[2];



    /*for(i = 0; i < 3; i++) {
        ecg1[i] = SPI.transfer(0);
        ecg1_data = (ecg1_data << (i*8)) | ecg1[i];
        //Serial.print(ecg1[i]);
        //Serial.print(", ");

    }*/
    Serial.println("");
    Serial.print(ecg1_data);


    //Serial.println("ECG 2 DATA:");
    /*for(i = 0; i < 3; i++) {
        ecg2[i] = readRegister50(0x50);
        ecg2_data = (ecg2_data << (i*8)) | (long)ecg2[i];
        //Serial.print(ecg2[i]);
        //Serial.print(", ");
    }*/
    //Serial.println("");
    //Serial.print(ecg2_data);
    //Serial.print(", ");
    //Serial.println("");

    digitalWrite(pin_SS, HIGH);

}


void setupECG()
{
    //SPI.beginTransaction(write_settings);
    while(Serial.read() != 'g') {}
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
    writeRegister(0x2e,0x33);
    writeRegister(0x2f,0x30);

    Serial.println("********************");

    readAllRegs();

    Serial.println("Configuring ECG settings");

    writeRegister(0x01, 0x20); //Connect channel 1’s INP to IN2 and INN to IN1.
    writeRegister(0x02, 0x19); //Connect channel 2’s INP to IN3 and INN to IN1.
    writeRegister(0x0A, 0x07); //Enable the common-mode detector on input pins IN1, IN2 and IN3.
    writeRegister(0x0C, 0x04); //Connect the output of the RLD amplifier internally to pin IN4.
    writeRegister(0x12, 0x04); //Use external crystal and feed the internal oscillator's output to the digital.
    writeRegister(0x14, 0x24); //Shuts down unused channel 3’s signal path.
    writeRegister(0x21, 0x02); //Configures the R2 decimation rate as 5 for all channels.
    writeRegister(0x22, 0x02); //Configures the R3 decimation rate as 6 for channel 1.
    writeRegister(0x23, 0x02); //Configures the R3 decimation rate as 6 for channel 2.
    writeRegister(0x27, 0x08); //Configures the DRDYB source to channel 1 ECG (or fastest channel).
    writeRegister(0x2F, B00010000); //Enables channel 1 ECG and channel 2 ECG for loop read-back mode.
    writeRegister(0x00, 0x01); //Starts data conversion.

    Serial.println("Done Configuring ECG settings");


}


void readAllRegs()
{
    //while(Serial.read() != 'g') {}

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
    //static byte x;
    /*int val = digitalRead(pin_DRDYB);
    if(val == 1)
        Serial.println(val);*/
    //x = readRegister50(0x50);
    //Serial.println(readRegister50(0x50));
    //while(Serial.read() != 'g') {}
    //setupECG();
    //readAllRegs();
}

byte readRegister50(byte reg)
{
    byte data;

    //Serial.print(reg, HEX);

    reg |= 1 << 7; //set read/write bit of command field to 1

    //digitalWrite(pin_SS, LOW);

    SPI.transfer(reg);  //transfer read register address with read bit set
    data = SPI.transfer(0); //transfer dummy data in order to get back the read data from the SPI bus

    //digitalWrite(pin_SS, HIGH);

    //Serial.print(" = ");
    //Serial.println(data, HEX);
    return data;  //return byte read from the SPI bus
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



void sendSMS(char txtMsg[160], char remoteNum[20])
{
    //sms text
    Serial.println(remoteNum);
    Serial.println("SENDING");
    Serial.println();
    Serial.println("Message:");
    Serial.println(txtMsg);

    //send the message
    sms.beginSMS(remoteNum);
    sms.print(txtMsg);
    sms.endSMS();
    Serial.println("\nCOMPLETE!\n");
}
