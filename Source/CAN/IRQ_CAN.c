

#include <LPC17xx.h>                  /* LPC17xx definitions */
#include "CAN.h"                      /* LPC17xx CAN adaption layer */
#include "../GLCD/GLCD.h"
#include <stdio.h>

#ifndef SIMULATOR
extern uint8_t icr ; 										//icr and result must be global in order to work with both real and simulated landtiger.
extern uint32_t result;
extern CAN_msg       CAN_TxMsg;    /* CAN message for sending */
extern CAN_msg       CAN_RxMsg;    /* CAN message for receiving */                                

//
extern void Receive(uint8_t lives,uint8_t time,uint16_t score);
extern uint8_t drawing;
void CAN_IRQHandler (void)  {

  /* check CAN controller 1 */
	icr = 0;
  icr = (LPC_CAN1->ICR | icr) & 0xFF;               /* clear interrupts */
	
  if (icr & 1) {                          		/* CAN Controller #1 meassage is received */
		CAN_rdMsg (1, &CAN_RxMsg);	                		/* Read the message */
    LPC_CAN1->CMR = (1 << 2);                    		/* Release receive buffer */
		//STAMPA
//		static uint8_t txt[15];
//		sprintf(txt,"C1 rec: %d",CAN_RxMsg.data[0]);
//		GUI_Text(100,0,txt,White,Black);
  }
	if (icr & 2) {                         /* CAN Controller #1 meassage is transmitted */
		// do nothing in this example
	}
		
	/* check CAN controller 2 */
	icr = 0;
	icr = (LPC_CAN2->ICR | icr) & 0xFF;             /* clear interrupts */

	if (icr & 1) {                          	/* CAN Controller #2 meassage is received */
		//if(!drawing)
		{
		CAN_rdMsg (2, &CAN_RxMsg);	                		/* Read the message */
    LPC_CAN2->CMR = (1 << 2);                    		/* Release receive buffer */
		uint16_t score=0;
		uint8_t time=CAN_RxMsg.data[0];
		uint8_t lives=CAN_RxMsg.data[1];
		score=CAN_RxMsg.data[2]<<8;
		score+=CAN_RxMsg.data[3];
		Receive(lives,time,score);
		}
	}
	if (icr & 2) {                         /* CAN Controller #2 meassage is transmitted */
		// do nothing in this example
	}
}
#endif