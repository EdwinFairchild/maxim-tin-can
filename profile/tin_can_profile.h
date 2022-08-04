#ifndef  _TIN_CAN_PROFILE_H_
#define _TIN_CAN_PROFILE_H_

#include "wsf_types.h"
#include "att_api.h"
#include "app_api.h"
/**************************************************************************************************
  Data Types
**************************************************************************************************/
/*! \brief Maxim custom service configurable parameters */
typedef struct {
    wsfTimerTicks_t     period;     /*!< \brief Button timer expiration period in seconds */
    uint16_t            count;      /*!< \brief Perform button after this many timer periods */
    uint8_t             threshold;  /*!< \brief Send button notification to peer when below this level. */
} tincanCfg_t;


/*************************************************************************************************/
/*!
 *  \brief  ATTS write callback for maxim custom service Use this function as a parameter
 *          to SvcMcsCbackRegister().
 *
 *  \param  connId      DM connection identifier.
 *  \param  handle      ATT handle.
 *  \param  operation   ATT operation.
 *  \param  offset      Write offset.
 *  \param  len         Write length.
 *  \param  pValue      Value to write.
 *  \param  pAttr       Attribute to write.
 *
 *  \return ATT status.
 */
/*************************************************************************************************/
uint8_t WriteCback(dmConnId_t connId, uint16_t handle, uint8_t operation,
                      uint16_t offset, uint16_t len, uint8_t *pValue, attsAttr_t *pAttr);

/*************************************************************************************************/
/*!
 *  \brief  Setting characteristic value and send the button value to the peer device.
 *
 *  \param  features       The button value.
 *
 *  \return None.
 */
/*************************************************************************************************/
void setAndNotify(uint8_t features);

/*************************************************************************************************/
/*!
 *  \brief  Initialize the mcs server.
 *
 *  \param  handlerId    WSF handler ID of the application using this service.
 *  \param  pCfg         mcs configurable parameters.
 *
 *  \return None.
 */
/*************************************************************************************************/
void tincanInit(wsfHandlerId_t handlerId, tincanCfg_t *pCfg);

/*************************************************************************************************/
/*!
 *  \brief  Start periodic mcs button state read.  This function starts a timer to perform
 *          periodic button read.
 *
 *  \param  connId      DM connection identifier.
 *  \param  timerEvt    WSF event designated by the application for the timer.
 *  \param  mcsCccIdx   Index of mcs level CCC descriptor in CCC descriptor handle table.
 *  \param  btnState    State of the push button.
 *
 *  \return None.
 */
/*************************************************************************************************/
void ButtonCheckStart(dmConnId_t connId, uint8_t timerEvt, uint8_t mcsCccIdx, uint8_t btnState);


//TODO: this button stuff should be interrupt based... if it doesnt mess up BLE
/*************************************************************************************************/
/*!
 *  \brief  Stop periodic mcs button read.
 *
 *  \param  connId      DM connection identifier.
 *
 *  \return None.
 */
/*************************************************************************************************/

void ButtonCheckStop(dmConnId_t connId);




#endif