/*
   Copyright (c) 2015, Digi International Inc.

   Permission to use, copy, modify, and/or distribute this software for any
   purpose with or without fee is hereby granted, provided that the above
   copyright notice and this permission notice appear in all copies.

   THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
   WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
   MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
   ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
   WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
   ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
   OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
*/
/***************************************************************************
	ad_calma_ch.c

	This sample program is for the RCM3400 series controllers with
	prototyping boards.


	Description
	===========
	This program demonstrates how to recalibrate an ADC milli-Amp channel using
	two known currents to generate two coefficients, gain and offset,
	which will be rewritten into to. It will also continuously display
	the current that is being monitored.

	This program must be compiled to Flash.

	Connections
	===========
	Make sure pins 2-3 are connected on JP1, JP2, and JP9.  Connect pins 1-2,
	3-4, 5-6, 7-8 on JP5.

	----------------|									|---------------------------
						 |	(optional)					| 4-20ma board
	Power supply	 |  100 ohm	 |-------|		|
	0 - 5V		  + |-/\/\/\----|current|-----| AIN channels 3 - 6
	                |	1/4 watt  | meter |		|
	                |				 ---------		|
	              - |-------------------------| AGND
					    |									|
	----------------|									|---------------------------

	Instructions
	============
	1. Connect a power supply of 0 - 5 volts 	to one of the AIN channel
		3 - 6 on the controller
	2. Compile and run this program.
	3. Follow the prompted directions of this program during execution.
	4. Vary voltage (0-5v) on power supply to see the CURRENT meter track
		what is displayed by Dynamic C (4-20ma).

	Note: For best results use a 4 1/2 digit current meter
***************************************************************************/
#class auto

#define ADC_SCLKBAUD 115200ul
#define STARTCHAN	3
#define ENDCHAN 6
#define GAINCODE GAIN_4   //milli-Amp multiplier for 0-5V range


main ()
{
	auto unsigned int rawdata, data1, data2;
	auto float locurrent, hicurrent;
	auto float currentequ;
	auto int inputnum;
	auto char buffer[64];

	brdInit();

	while (1)
	{
		printf("\nChoose the AD mAmp channel %d to %d .... ", STARTCHAN, ENDCHAN);
		gets(buffer);
		inputnum = atoi(buffer);

		/////Get two data points using known currents
		printf("\n\nAdjust the current to approx. 5.0 mA and then ENTER the actual\n");
		printf("current being measured, (floating point value) = ");
		gets(buffer);
		locurrent = atof(buffer);
		data1 = anaIn(inputnum, mAMP, GAINCODE);
		printf("data1 = %d\n", data1);

		printf("\n\nAdjust the current to approx. 19.0 mA and ENTER the actual\n");
		printf("current being measured, (floating point value) = ");
		gets(buffer);
		hicurrent = atof(buffer);
		data2 = anaIn(inputnum, mAMP, GAINCODE);
		printf("data2 = %d\n", data2);

		/////Calculate gains and offsets
		anaInCalib(inputnum, mAMP, GAINCODE, data1, locurrent, data2, hicurrent);

		/////Store coefficients into eeprom
		anaInEEWr(inputnum, mAMP, GAINCODE);
		printf("Wrote coefficients to eeprom\n");

		/////Read back coefficients from eeprom
		anaInEERd(inputnum, mAMP, GAINCODE);
		printf("Read coefficients from eeprom\n");

		printf("Vary current on channel %d\n", inputnum);
		while (strcmp(buffer,"q") && strcmp(buffer,"Q"))
		{
			currentequ = anaInmAmps(inputnum);
			printf("Current at CH%d is %.2fma\n", inputnum, currentequ);
			printf("\npress enter key to read value again or 'Q' to calibrate another channel\n\n");
			gets(buffer);
		}
	}

}	//end main


