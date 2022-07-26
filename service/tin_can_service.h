#ifndef _TIN_CAN_SERVICE_H_
#define _TIN_CAN_SERVICE_H_

#include "wsf_types.h"
#include "util/bstream.h"
#include "wsf_msg.h"
#include "wsf_trace.h"
#include "util/bstream.h"
#include "att_api.h"

#define TIN_CAN_SERVICE_UUID_BASE 0xBE, 0xC5, 0xD1, 0x24, 0x99, 0x33, 0xC6, 0x87, 0x85, 0x41, 0xD9, 0x31, 0x7D, 0x56
#define ATT_UUID_TIN_CAN_BUILD(part)            UINT16_TO_BYTES(part), TIN_CAN_SERVICE_UUID_BASE


#define BUTTON_VALUE_CHART_PARTIAL 0x1230
#define  TIN_CAN_SERVICE_PARTIAL 0xfc85
/* Builds UUIDS from ARM base UUID and a unique partial */
#define TIN_CAN_SERVICE_UUID      ATT_UUID_TIN_CAN_BUILD(TIN_CAN_SERVICE_PARTIAL)
#define BUTTON_VALUE_CHART_UUID   ATT_UUID_TIN_CAN_BUILD(BUTTON_VALUE_CHART_PARTIAL)

/* the partial gets turn into little endian and then appended to beggning of the base
   the entire UUID is then advertised in little endian format also, in other words the reverse order
   that is seen above */

#define START_HDL 0x1000 /* arbitrary numbver chosen */
/* Characteristic Handles */
enum{
    TIN_CAN_SVC_HDL = START_HDL,
    BUTTON_CH_DECL_HDL,     // handle of button characteristic declartion 
    BUTTON_VAL_HDL,         // Handle for the button value (state)
    BUTTON_CH_CCC_HDL,      // Handle for the characteristic change descriptor
    HANDLE_COUNT

};

#define END_HANDLE (HANDLE_COUNT-1)

/**************************************************************************************************
  Function Declarations
**************************************************************************************************/

/*************************************************************************************************/
/*!
 *  \brief  Add the services to the attribute server.
 *
 *  \return None.
 */
/*************************************************************************************************/
void TinCanSvcAddGroup(void);

/*************************************************************************************************/
/*!
 *  \brief  Remove the services from the attribute server.
 *
 *  \return None.
 */
/*************************************************************************************************/
void TinCanSvcRemoveGroup(void);

/*************************************************************************************************/
/*!
 *  \brief  Register callbacks for the service.
 *
 *  \param  readCback   Read callback function.
 *  \param  writeCback  Write callback function.
 *
 *  \return None.
 */
/*************************************************************************************************/
void TinCanSvcCbackRegister(attsReadCback_t readCback, attsWriteCback_t writeCback);


#endif
