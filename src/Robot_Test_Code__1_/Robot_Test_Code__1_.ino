#include "CRSFforArduino.hpp"
#include <Servo.h>

CRSFforArduino *crsf = nullptr;

/* A flag to hold the fail-safe status. */
bool isFailsafeActive = false;

/* RC Channels data. */
int rcChannelCount = 8;
const char *rcChannelNames[] = {
    "A",
    "E",
    "T",
    "R",
    "Aux1",
    "Aux2",
    "Aux3",
    "Aux4",

    "Aux5",
    "Aux6",
    "Aux7",
    "Aux8",
    "Aux9",
    "Aux10",
    "Aux11",
    "Aux12"};

    
Servo left;  // create servo object to control the left esc 
Servo right;  // create servo object to control the right esc 
int spdl = 0;
int spdr = 0;

/* RC Channels Event Callback. */
void onReceiveRcChannels(serialReceiverLayer::rcChannels_t *rcData);

void setup()
{
    crsf = new CRSFforArduino();

    /* Initialise CRSF for Arduino. */
    if (crsf->begin() == true)
    {
        /* CRSF for Arduino initialised successfully.
        We can now register the RC Channels event. */
        crsf->setRcChannelsCallback(onReceiveRcChannels);

        /* Constrain the RC Channels Count to the maximum number
        of channels that are specified by The Crossfire Protocol.*/
        rcChannelCount = rcChannelCount > crsfProtocol::RC_CHANNEL_COUNT ? crsfProtocol::RC_CHANNEL_COUNT : rcChannelCount;
    }
    else
    {
        /* Clean up any resources,
        if initialisation fails. */
        crsf->end();
        delete crsf;
        crsf = nullptr;
    }
    left.attach(2, 1000, 2000);  // attaches the servo on pin 2 to the servo object
    // Initialise the serial port & wait for the port to open.
    right.attach(3, 1000, 2000);
}

void loop()
{
    /* Guard CRSF for Arduino's API with a null check. */
    if (crsf != nullptr)
    {
        /* Call CRSF for Arduino's main function here. */
        crsf->update();
    }
}

void onReceiveRcChannels(serialReceiverLayer::rcChannels_t *rcData)
{
    /* This is your RC Channels Event Callback.
    Here, you have access to all 16 11-bit channels,
    plus an additional "failsafe" flag that tells you whether-or-not
    your receiver is connected to your controller's transmitter module.

    Using the rcData parameter that was passed in,
    you have access to the following:

    - failsafe - A boolean flag indicating the "Fail-safe" status.
    - value[16] - An array consisting of all 16 received RC channels.
      NB: RC Channels are RAW values and are NOT in "microseconds" units.

    For the purposes of this example, the fail-safe flag is used to centre
    all channels except for Channel 5 (AKA Aux1). Aux1 is set to the
    "Disarmed" position.
    The RC Channels themselves are all converted to "microseconds" for
    visualisation purposes, and printed to the Serial Monitor at a rate
    of 100 Hz. */

    if (rcData->failsafe)
    {
        if (!isFailsafeActive)
        {
            isFailsafeActive = true;

            /* Centre all RC Channels, except for Channel 5 (Aux1). */
            for (int i = 0; i < rcChannelCount; i++)
            {
                if (i != 4)
                {
                    rcData->value[i] = 992;
                }
            }

            /* Set Channel 5 (Aux1) to its minimum value. */
            rcData->value[4] = 191;

            Serial.println("[Sketch | WARN]: Failsafe detected.");
        }
    }
    else
    {
        /* Set the failsafe status to false. */
        if (isFailsafeActive)
        {
            isFailsafeActive = false;
            Serial.println("[Sketch | INFO]: Failsafe cleared.");
        }
    }

    /* Here is where you may write your RC channels implementation.
    For this example, RC Channels are simply sent to the Serial Monitor. */
    static uint32_t lastPrint = millis();

    if (millis() - lastPrint >= 10)
    {
        lastPrint = millis();
                spdl = crsf->rcToUs(crsf->getChannel(3));
                spdr = crsf->rcToUs(crsf->getChannel(2));
                map(spdl,989,2012,0,180);
                map(spdr,989,2012,0,180);
                left.write(spdl);
                right.write(spdr);
        /*
        Serial.print("[Sketch | INFO]: RC Channels: <");
        for (int i = 0; i < rcChannelCount; i++)
        {
            Serial.print(rcChannelNames[i]);
            Serial.print(": ");
            Serial.print(crsf->rcToUs(rcData->value[i]));

            if (i < (rcChannelCount - 1))
            {
                Serial.print(", ");
            }
        }
        Serial.println(">");
        */
    }
}
