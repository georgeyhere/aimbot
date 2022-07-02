`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: Cal Poly Pomona 
// Engineer: Mario Andres Molina 
// 
// Create Date: 02/20/2021 12:31:28 PM
// Design Name: Ultrasonic Sensor Driver 
// Module Name: main
// Project Name: NGCP UGV, Pomona 2020-2021
// Target Devices: HCSR04/SEN0208
// Tool Versions: 
// Description: 
// 
// Dependencies: 
// 
// Revision:
// Revision 2.01 - Original File corrupted due to me moving files in a really dumb way 
// Additional Comments:
// 
//////////////////////////////////////////////////////////////////////////////////


module main(                                                                                                                                                
    input logic clock,                          //Input logic clock, It should be 100 MHz (10ns cycles)                                                                                                                               
    input logic reset,                          //Input logic reset, This isn't really used, I should remove it     
    input logic echo_signal,                    //Input logic echo_signal, This should be a pulse that corresponds to the distance, and we'll count the amount of cycles while it's high                                                                                                        
    output logic trigger_signal,                //Output logic trigger_signal, this should be outputing a 10uS pulse every 5 ms                                                                                                                                                                                                                                      
    output logic [21:0] counter,                //Output logic counter, this should be the amount of cycles that pass, it's commented out when you want to use hardware manager                                                                                                                  
    output logic [5:0] led                      //Output logic led, this led should the counter in real time                                                                                                                       
    );                                                                                                                                                       
                                                                                                                                                             
                                                                                                                                                             
                                                                                                                                                                                                                                                                             
                                                //Comment out line 35 when ready to move to custom IP, uncomment line 28                                                                                                                                                          
    //logic [21:0] counter;                     //this is the 22 bit counter, which means it's highest is 4,194,303 which works for us since the maximum range of the SEN0208 is 4.5m (450 cm) 
                                                // 4.5cm is 2,610,000 cycles (26.1 ms) , 21 bits bearly misses the cut , the DATA SHEET describes the range based on time should be 150us to 25 ms                                                                                                                                                                                                                                                                           
    logic [24:0] internal_counter  = 1;         //This is the 25 bit internal_counter and it starts off as 1, used to measure space between trig and echo                                                                                                                                                                                          
    logic [9:0] i = 0;                          //This is the 10 bit i and it start off as 0, used for 10us trigger pulse                                                                                                               
    logic a = 0;                                //This is the 1 bit a and it starts off as 0, used for initial echo signal                                                                                                              
    logic [21:0] echo_counter = 1;              //This is the 22 bit echo_counter and starts off as 1, used to measure echo                                                                                                              
    logic [25:0] j = 0;                         //This is the 26 bit j and it starts off as 0, used for "before new trigger delay"
    logic waiting = 0;                          //This is the waiting varibale, this is suppose to indicate when the sensor is waiting for the echo to go down 
    logic trigger = 1;                          //This is the trigger variable, this is suppose to indicate when the sensor is doing it's "before new trigger delay" and 10us trigger                                                                                                          
    logic b = 0;                                //Flag for out of range
                                                                                                                                                             
    always_ff@(posedge clock)                       //At the positive edge of the clock  BEGIN                                                                                                            
    begin     
                                                 // j is the delay after the echo signal goes to a zero, good for removing late feed back 
      Trigger:                                   //Begining of the Trigger Stage 
      if(j<5100000 && trigger == 1)              // if the j is the "before new trigger delay" and the trigger var. has to be equal to 1 so that there is no delay outside of the trigger stage 
                begin                            // Here we have set to 5,100,000 (51ms) , the DATA SHEET says "before new trigger delay" should be no less than 50ms 
                        j <= j + 1;              // This adds one to j everytime, this creates the "before new trigger delay"         
                end

        
      if(i<1000 && j == 5100000 && trigger == 1) //This if statement is only entered when 1) "before new trigger delay" is done 2)we're in the trigger stage 3)the 25 bit i is less than 1,000 (10us) 
                 begin 
                        trigger_signal <= 1;     //Turn on the Trigger Signal for Output 
                        internal_counter <= 1;   //Return the internal_counter back to 1
                        echo_counter <= 1;       //Return the echo_counter back to 1
                        i = i + 1;               //This created the 10us trigger pulse 
                        b <= 0;
                 end
      else                                       //This else statement occurs when the 10us pulse is over 
                 begin
                        i = 0;                   //Return the i varible to 0 for future uses / keep i at 0 if "before new trigger delay" isn't done yet     
                        trigger_signal <= 0;     //Turn off the Trigger Signal for Output                                                 
                        if(j == 5100000)         //If the "before new trigger delay" is over (remember the code will be entering here while the "before new trigger delay" is occuring [line 58] 
                        begin                    //and we don't want it to switch stages
                        waiting <= 1;            //Make the waiting var. a 1 and as such we move onto the waiting stage 
                        trigger <= 0;            //Make the trigger var. a 0 and as such we leave the trigger state 
                        a <= 0;                  //Return a to 0, we do this here because it's within this statement we know that the whole trigger stage was done properly 
                                                 //and we know the waiting stage is awaiting it's "first" echo
                        end
                         
                 end           
                
    Waiting: 
    if(waiting == 1 && echo_signal == 0)        //This if statement is when we are in the waiting stage, so the US is issuing it's 8 40KHz pulses but the echo signal is NOT HIGH 
    begin 
    internal_counter <= internal_counter + 1;   //This statement increase the internal_counter var. for every 10ns the echo signal is NOT HIGH, this can be used to calculate distance                                                                                                    
    if(waiting == 1 && internal_counter == 30000 && echo_signal == 0)//if echo doesn't come back in 0.3 ms return to the trigger stage
    begin                                       // so if the SEN0208 sends out 8 40 Khz pulse, 40 Khz is 40,000 cyles per sec, 25us a cycle x 8 = 0.2 ms, so if its 0.1ms late we resend trig
       trigger <= 1;                            //Make the trigger var. a 1 and as such we move the trigger state
       waiting <= 0;                            //Make the waiting var. a 0 and as such we leave the waiting stage
       j <= 0;                                  //Return the j var., the "before new trigger delay" to 0 
    end           
    if(waiting == 1 && a==1 && echo_signal == 0)//The inital echo has finished, and now it's time to return to the tirgger stage 
    begin
       trigger <= 1;                            //Make the trigger var. a 1 and as such we move the trigger state
       waiting <= 0;                            //Make the waiting var. a 0 and as such we leave the waiting stage
       j <= 0;                                  //Return the j var., the "before new trigger delay" to 0 
    end                                 
    end
    else if (waiting == 0 && echo_signal == 1)  //This if statement enters when the initial echo signal is encounter 
    begin
            a <= 1;                             //Setting a to 1 indicates that the very first echo signal has been recieved, remember we only want 1 echo signal to be counter
                                                //per trigger signal because other wise we'll be counting possible unwanted feed back so we don't want to reenter if echo signal ever goes LOW before new Trig.
        if (echo_counter < 2_320_000)           //Echo counter will count up to 2_320_000, which equates to 400 cm, its max distance
            begin
                echo_counter <= echo_counter + 1; //This counter increases by 1 every 10ns  
            end   
        else 
            begin
                counter = counter * 0;
                b <= 1;
            end          
    end    
    if(waiting == 0 && a==1 && echo_signal == 0 && trigger_signal == 0) 
    begin   
        counter = echo_counter;                 //Counter will equal echo counter right after it finishes incrementing and right before it is reset to 1 
    end
    end                                                                                                                                                                                                                                                                                                                                                                                              
endmodule 