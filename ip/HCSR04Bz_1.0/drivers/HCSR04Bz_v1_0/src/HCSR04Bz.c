

/***************************** Include Files *******************************/
#include "HCSR04Bz.h"

/************************** Function Definitions ***************************/

/* A function to return the distance detected by an ultrasonic sensor.
 *
 */
u32 getUsDistance(u32 US_BASEADDR)
{
	u32 counterVal;
	//
	counterVal = Xil_In32(US_BASEADDR);
	if(counterVal == 0 || counterVal >= 2319999) {
		return 400; // no object in range
	}
	else {
		return counterVal / 5800; // distance in cm
	}
}
