/***************************************************************************
	adc_cal_diff.c
	Z-World, 2004

	This sample program is for the BL2600 series controller.

	Description
	===========
   This program demonstrates how to recalibrate a differential
   ADC channel using two known voltages to generate calibration
   constants for the given channel and will write the data into
   reserved eeprom.

	This program will also continuously display the voltage of
	the channel that was calibrated.

	Connections
	===========
	For this calibration procedure you will need a power supply
	that has a floating output.

	NOTE:	Before doing the following steps, set the power supply
	      to zero volts and then turn it OFF.

	1. Initially connect the positive side of the power supply to
	   the positive side to one of the following ADC differential
	   channel pairs.

	    Channel   DIFF Pairs
	    -------  ------------
	      0 		 +AIN0  -AIN1
	      2		 +AIN2  -AIN3
	      4		 +AIN4  -AIN5
	      6		 +AIN6  -AIN7

	2.	Connect the negative side of the power supply to the
	   negative side to one of the following ADC differential
	   channel pairs. (Same DIFF pair from step 1)

	    Channel    DIFF Pairs
	    -------   ------------
	      0		  +AIN0   -AIN1
	      2		  +AIN2   -AIN3
	      4		  +AIN4   -AIN5
	      6 		  +AIN6   -AIN7

   3. Connect a voltmeter to monitor the ADC channel being calibrated
   at the same points you connected the power supply in steps 1 and 2.
	(For best results use a 4 1/2 digit voltmeter)


	Instructions
	============
	1. Power-on the controller.
	2. Compile and run this program.
	3. Turn ON the power supply for the ADC calibration.
	4. Follow the instructions as displayed.

***************************************************************************/

#class auto	 // Change local var storage default to "auto"

const float vmax[] = {
	20.0,
	10.0,
	5.0,
	4.0,
	2.5,
	2.0,
	1.25,
	1.00
};


nodebug
void msDelay(unsigned int delay)
{
	auto unsigned long done_time;

	done_time = MS_TIMER + delay;
   while( (long) (MS_TIMER - done_time) < 0 );
}

// set the STDIO cursor location and display a string
void DispStr(int x, int y, char *s)
{
   x += 0x20;
   y += 0x20;
   printf ("\x1B=%c%c%s", x, y, s);
}

void printrange()
{
	printf("\ngain_code\tVoltage range\n");
	printf("---------\t-------------\n");
	printf("\t0\t +- 20v\n");
	printf("\t1\t +- 10v\n");
	printf("\t2\t +- 5v\n");
	printf("\t3\t +- 4v\n");
	printf("\t4\t +- 2.5v\n");
	printf("\t5\t +- 2v\n");
	printf("\t6\t +- 1.25v\n");
	printf("\t7\t +- 1v\n\n");

}


void main ()
{
	auto long value1, value2;
	auto int rawdata;
	auto int key, valid, i;
	auto int gaincode, channel;
	auto float voltage, volts1, volts2;
	auto float pwr1, pwr2;
	auto float cal_voltage;
	auto char buffer[64];

   // Initialize the controller
	brdInit();

   // Configure channel pair 0 & 1 for differential mode of operation
   // (Max voltage range is �20V)
   anaInConfig(0, DIFF_MODE);

   // Configure channel pair 2 & 3 for differential mode of operation
   // (Max voltage range is �20V)
   anaInConfig(1, DIFF_MODE);

   // Configure channel pair 4 & 5 for differential mode of operation
   // (Max voltage range is �20V)
   anaInConfig(2, DIFF_MODE);

   // Configure channel pair 6 & 7 for differential mode of operation
   // (Max voltage range is �20V)
   anaInConfig(3, DIFF_MODE);


   while(1)
	{
		DispStr(1, 1,"!!!Caution this will overwrite the calibration constants set at the factory.");
		DispStr(1, 2,"Do you want to continue(Y/N)?");

		while(!kbhit());
		key = getchar();
		if(key == 'Y' || key == 'y')
		{
			break;
		}
		else if(key == 'N' || key == 'n')
		{
			exit(0);
		}

	}
	printf("\n");
	while(kbhit()) getchar();

	while (1)
	{
		printf("\nchannel_code\tInputs\n");
		printf("------------\t-------------\n");
		printf("\t0\t+AIN0 -AIN1\n");
		printf("\t2\t+AIN2 -AIN3\n");
		printf("\t4\t+AIN4 -AIN5\n");
		printf("\t6\t+AIN6 -AIN7\n\n");
		printf("\nChoose the AD channel 0,2,4, or 6 = ");
		do
		{
			channel = getchar();
			switch(channel)
			{
				case '0':
				 valid = TRUE;
				 break;
				case '2':
				 valid = TRUE;
				 break;
				case '4':
				 valid = TRUE;
				 break;
				case '6':
				 valid = TRUE;
				 break;
			}
		} while (!valid);
		channel = channel - 0x30;
		printf("%d", channel);
		while(kbhit()) getchar();

		printf("\n");
		printrange();
		printf("Choose gain code (0-7) =  ");
		do
		{
			gaincode = getchar();
		} while (!( (gaincode >= '0') && (gaincode <= '7')) );
		gaincode = gaincode - 0x30;
		printf("%d\n\n", gaincode);
		while(kbhit()) getchar();


		cal_voltage = vmax[gaincode]*.8;
		printf("\nAdjust Power connected to AIN%d to approx. %.2f\n", channel, cal_voltage);
		printf("and then enter actual voltage = ");
		gets(buffer);

		volts1 = atof(buffer);
		value1 = 0;
		for(i=0; i<10; i++)
			value1 += anaIn(channel, gaincode);
		value1 = value1/10;

		printf("\nSwap power supply connections and then PRESS any key\n");
		while(!kbhit());
		while(kbhit()) getchar();

		volts2 = -volts1;
		value2 = 0;
		for(i=0; i<10; i++)
			value2 += anaIn(channel, gaincode);
		value2 = value2/10;

		if (anaInCalib(channel, DIFF_MODE, gaincode,
		              (int) value1, volts1, (int) value2, volts2))
		{
			printf("cannot make calibrations\n");
			exit(0);
		}

		printf("\nVary power supply for the voltage range selected.... \n\n");
		do
		{
			voltage = anaInDiff(channel, gaincode);
			printf("Ch %2d Volt=%.5f \n", channel, voltage);

			printf("Press ENTER key to read value again or 'Q' to calibrate another\n\n");
			while(!kbhit());
			key = getchar();
			while(kbhit()) getchar();

		}while(key != 'q' && key != 'Q');

	}
}
///////////////////////////////////////////////////////////////////////////

