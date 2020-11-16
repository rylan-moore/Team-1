#include "msp.h"
#include "i2c.h"
#include "pca9685.h"
#include "servo.h"
#include "stIMU.h"
#include "stdio.h"
#include "forward.h"
#include "potentiometer.h"



volatile int state = 0;


void main(void)
 {
	WDT_A->CTL = WDT_A_CTL_PW | WDT_A_CTL_HOLD;		// stop watchdog timer

	//set up LED warning
    P2->DIR |= BIT2;      //configures LED to be an output
    P2->OUT &= ~BIT2;     //output is set to 0

	//set up Button Interrupt
    P1->DIR &= ~BIT1;
    P1->OUT |= BIT1;
    P1->REN |= BIT1;
    P1->IES |= BIT1;
    P1->IFG = 0;
    P1->IE  |= BIT1;

    //enable interrupts
    NVIC_EnableIRQ(PORT1_IRQn);
	__enable_irq();

    I2C_OPEN_STRUCT_TypeDef i2c_open_struct;

    i2c_open_struct.uca10 = 0;                              //no self 10-bit Address
    i2c_open_struct.ucsla10 = 0;                            //no 10-bit Addressing
    i2c_open_struct.ucmm = 0;                               //No Multi-Master
    i2c_open_struct.ucmst = EUSCI_B_CTLW0_MST;              //Master Mode
    i2c_open_struct.ucsselx = EUSCI_B_CTLW0_SSEL__SMCLK;    //SMCLK to be selected (3MHz)
    i2c_open_struct.ucbrx = 30;                             //Prescaler for Selected Clock.
                                                            //(100kHz)

    //This sets up the I2C driver to operate with the
    //correct settings.
    //EUSCI_B0 uses P1.7 as SCL and P1.6 as SDA
    i2c_open(EUSCI_B0, &i2c_open_struct);

    pca9685_init(); //sets up PCA to output at the correct frequency
    config_LIS3MDL();

    //set servos to neutral position
            servo_write(URL,90); //90 Degrees represents the Neutral Position
            servo_write(LRL,90);
            servo_write(URA,90);
            servo_write(LRA,90);
            servo_write(ULL,90);
            servo_write(LLL,90);
            servo_write(ULA,90);
            servo_write(LLA,90);




    int16_t mx, my, mz;
    uint8_t data;
    int i;


    //Neutral Position startup
    while(state == 0){
        for(i=0; i<200000; i++);
        //i2c_start(EUSCI_B0, LIS3MDL_MAG_I2C_ADDRESS, READ, &data, 1, LIS3MDL_MAG_STATUS_REG);
        i2c_start(EUSCI_B0, LIS3MDL_MAG_I2C_ADDRESS, READ, &data, 1, LIS3MDL_MAG_STATUS_REG);
        printf("status 1: %d\n",data);
        for(i=0; i<1000; i++);
        mx = read_magnetometer_x();
        my = read_magnetometer_y();
        mz = read_magnetometer_z();
        printf("x: %d\n", mx);
        printf("y: %d\n", my);
        printf("z: %d\n", mz);
        //printf("status: %d\n", data);
        printf("\n");
        printf("\n");
        //printf("\n");
        //printf("\n");
    }

    //Standing Position stopped idle
    while(state == 1){
        servo_write(URL,90-45); //Because the Orientations are opposite of the other servos,
        servo_write(LRL,90-45); //the direction the servo must move to stand up is also opposote
        servo_write(URA,90+45);
        servo_write(LRA,90+45);
        servo_write(ULL,90+45);
        servo_write(LLL,90+45);
        servo_write(ULA,90-45); //See Above Comment
        servo_write(LLA,90-45);
        P2->OUT &= ~BIT2;     //output is set to 0

    }

    while(state == 2){
        P2->OUT |= BIT2;     //output is set to 0
        forward();
    }

}


/* Port1 ISR */
void PORT1_IRQHandler(void){
    volatile uint32_t j;

    //Change State to Standing Position
    if(P1->IFG & BIT1)
        state = 1;

    // Delay for switch debounce
    for(j = 0; j < 100000; j++)

    P1->IFG &= ~BIT1;
}
 /*servo_write(URL,90); //90 Degrees represents the Neutral Position
        servo_write(LRL,90);
        servo_write(URA,90);
        servo_write(LRA,90);
        servo_write(ULL,90);
        servo_write(LLL,90);
        servo_write(ULA,90);
        servo_write(LLA,90);

        */
