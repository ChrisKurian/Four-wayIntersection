#include <reg51.h>
// Western lights on Proteus
sbit RED1 = P1^0;
sbit YEL1 = P1^1;
sbit GRE1 = P1^2;
// Northern lights on Proteus
sbit RED2 = P1^3;
sbit YEL2 = P1^4;
sbit GRE2 = P1^5;
// Eastern lights on Proteus
sbit RED3 = P2^0;
sbit YEL3 = P2^1;
sbit GRE3 = P2^2;
// Southern lights on Proteus
sbit RED4 = P2^3;  
sbit YEL4 = P2^4; 
sbit GRE4 = P2^5; 
// Left turn light
sbit BLUE = P2^6;
// Defining 7-segment pins
sbit a = P0^0; // 7-segment a
sbit b = P0^1; // 7-segment b 
sbit c = P0^2; // 7-segment c
sbit d = P0^3; // 7-segment d
sbit e = P0^4; // 7-segment e
sbit f = P0^5; // 7-segment f
sbit g = P0^6; // 7-segment g
sbit h = P0^7; // 7-segment h

// 0 = lights are red, 1 = lights are yellow, 2 = lights are green
unsigned char WEST;
unsigned char NORTH;
unsigned char EAST;
unsigned char SOUTH;

// declaring the names of interrupts
unsigned char LeftTurn;
unsigned char Crosswalk;

// loop for creating a delay 
// j = 1275 made the delay very long so we use 200 to make simulation quicker
void MSDelay(unsigned int);
void MSDelay(unsigned int itime){
		unsigned int i, j;
		for(i = 0; i < itime; i++)
			for(j = 0; j < 200; j++);
}

// interruts for left turn and cross walk buttons 	
void Left(void) interrupt 0{
	     LeftTurn = 1; // stores a value to be checked later
}

void Crossing(void) interrupt 2{
	     Crosswalk = 1; // stores a value to be checked later 
}

// timers for the 7-segment
void CrosswalkDelay(){
	     // select mode 1 of timer 1 (16-bit timer mode)
			 TMOD = 0x10;
	
			 // 4BFD = 19453 in decimal and FFFF = 65536 in decimal
	     // 65536 - 19453 = 46083
	     // 46083 * 1.085 microseconds = 50 milliseconds
			 TL1 = 0xFD;
			 TH1 = 0x4B;
	
	     // start timer 1
			 TR1 = 1;
	
	     // do machine cycles until overflow flag is raised
			 while (TF1 == 0);
	
	     // stop the timer
			 TR1 = 0;
	
	     //reset the flag
			 TF1 = 0;
}

///////////////////////////////////////////////////////////////////////////////////
void main(void){
	
		EA = 1; // enables all interrupts
		EX0 = 1; // external interrupt 0 is enabled
		EX1 = 1; // external interrupt 1 is enabled
		BLUE = 0; // set left turn to be off initially
	
		// Loop for running the lights forever 
		while(1){   
			
			 // sets North and South to red
       RED2 = 1; YEL2 = 0; GRE2 = 0;
 			 RED4 = 1; YEL4 = 0; GRE4 = 0; 
			 NORTH = 0; SOUTH = 0; 

			 // sets West and East to green
			 RED1 = 0; YEL1 = 0; GRE1 = 1;
			 RED3 = 0; YEL3 = 0; GRE3 = 1; 
			 EAST = 2; WEST = 2; 	 
			 			
			 // Checks if crosswalk has been pressed and displays countdown on 7-segment.
			 // Only applied to the road heading West/East 
			 if(Crosswalk == 1 && WEST == 2 && EAST == 2){
				 
					// counts down from 6 to 0 on the 7-segment
					// works based on negative logic
				  // 0.40 second delays between each number by calling 50 ms timer 8 times
				  unsigned char x;
					a = 0; b = 1; c = 0; d = 0; e = 0; f = 0; g = 0; CrosswalkDelay();
				  for(x = 0; x < 8; x++){CrosswalkDelay();}
					a = 0; b = 1; c = 0; d = 0; e = 1; f = 0; g = 0; CrosswalkDelay();
					for(x = 0; x < 8; x++){CrosswalkDelay();}
					a = 1; b = 0; c = 0; d = 1;	e = 1; f = 0; g = 0; CrosswalkDelay();
					for(x = 0; x < 8; x++){CrosswalkDelay();}
					a = 0; b = 0; c = 0; d = 0; e = 1; f = 1; g = 0; CrosswalkDelay();
					for(x = 0; x < 8; x++){CrosswalkDelay();}
					a = 0; b = 0; c = 1; d = 0; e = 0; f = 1; g = 0; CrosswalkDelay();
					for(x = 0; x < 8; x++){CrosswalkDelay();}
					a = 1; b = 0; c = 0; d = 1; e = 1; f = 1; g = 1; CrosswalkDelay();
					for(x = 0; x < 8; x++){CrosswalkDelay();}
					a = 0; b = 0; c = 0; d = 0; e = 0; f = 0; g = 1; CrosswalkDelay();
					for(x = 0; x < 8; x++){CrosswalkDelay();}
					a = 1; b = 1; c = 1; d = 1; e = 1; f = 1; g = 1; CrosswalkDelay();
					for(x = 0; x < 8; x++){CrosswalkDelay();}
				 
				  // resets the crosswalk interruption
					Crosswalk = 0;
			 } 
				 
			 // delay 3.5 seconds
			 MSDelay(3500);
				
			 // sets West and East to yellow
			 RED1 = 0; YEL1 = 1; GRE1 = 0;
			 RED3 = 0; YEL3 = 1; GRE3 = 0; 
			 EAST = 1; WEST = 1; 
						 
       // delay for 0.75 seconds
			 MSDelay(750);  
			 
	     // sets West and East to red
			 RED1 = 1; YEL1 = 0; GRE1 = 0;
			 RED3 = 1; YEL3 = 0; GRE3 = 0; 
			 EAST = 0; WEST = 0; 
			 
       // delay for 0.75 seconds
			 MSDelay(750);
			 
			 // sets North and South to green
       RED2 = 0; YEL2 = 0; GRE2 = 1;
 			 RED4 = 0; YEL4 = 0; GRE4 = 1; 
			 NORTH = 2; SOUTH = 2; 

			 // checks to see if the left turn lane has cars waiting to turn
			 if(LeftTurn == 1 && WEST == 0 && EAST == 0 && SOUTH == 2 && NORTH == 2){
				 
				  // cars heading South must wait for left turning cars to finish turning
				  // cars heading West are free to go the whole time
				  RED2 = 0; YEL2 = 0; GRE2 = 1;
				  RED4 = 1; YEL4 = 0; GRE4 = 0;			 
				  NORTH = 2; SOUTH = 0; 
				 
				  //delay 0.25 seconds and alternating on/off of blue LED
				  BLUE = 1; MSDelay(250); 
				  BLUE = 0; MSDelay(250); 
				  BLUE = 1; MSDelay(250); 
				  BLUE = 0; MSDelay(250); 
				  BLUE = 1; MSDelay(250); 
				  BLUE = 0; MSDelay(250); 
				  BLUE = 1; MSDelay(250);
   			  BLUE = 0; MSDelay(250);
				  BLUE = 1; MSDelay(250);
				  BLUE = 0; MSDelay(250);
				 
				  // sets North and South to green
				  RED2 = 0; YEL2 = 0; GRE2 = 1;
				  RED4 = 0; YEL4 = 0; GRE4 = 1;	
				  NORTH = 2; SOUTH = 2; 
				 
				  // resets the left turn interruption
				  LeftTurn = 0; 				 
			 }  
						 
			 // delay 3.5 seconds
			 MSDelay(3500);
								 
			 // sets North and South to yellow
       RED2 = 0; YEL2 = 1; GRE2 = 0;
 			 RED4 = 0; YEL4 = 1; GRE4 = 0; 
			 NORTH = 1; SOUTH = 1; 
			 
			 // delay for 0.75 seconds
			 MSDelay(750);
			 
			 // sets North and South to red
       RED2 = 1; YEL2 = 0; GRE2 = 0;
 			 RED4 = 1; YEL4 = 0; GRE4 = 0; 
			 NORTH = 0; SOUTH = 0; 
				 
		 	 // delay for 0.75 seconds
			 MSDelay(750);
		 } 
	 } 