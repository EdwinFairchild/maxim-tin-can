#include "tin_can_profile.h"
#include <stdbool.h>
#include "app_ui.h"
#include "pal_led.h"
#include "tin_can_service.h"
/**************************************************************************************************
  Macros
**************************************************************************************************/

#ifndef LED_RED
#define LED_RED         0
#endif

#ifndef LED_GREEN
#define LED_GREEN       1
#endif

#ifndef LED_BLUE
#define LED_BLUE        LED_GREEN
#endif

/**************************************************************************************************
  Local Variables
**************************************************************************************************/

/*! \brief Connection control block */
typedef struct {
    dmConnId_t    connId;              /*! \brief Connection ID */
    bool_t        mcsToSend;           /*! \brief mcs measurement ready to be sent on this channel */
    uint8_t       sentMcsBtnState;     /*! \brief value of last sent mcs button state */
} tincanConn_t;

/*! \brief Control block */
static struct {
    tincanConn_t         conn[DM_CONN_MAX];    /*! \brief connection control block */
    wsfTimer_t        btnStateChkTimer;     /*! \brief periodic check timer */
    tincanCfg_t          cfg;                  /*! \brief configurable parameters */
    uint16_t          currCount;            /*! \brief current measurement period count */
    bool_t            txReady;              /*! \brief TRUE if ready to send notifications */
    uint8_t           btnState;    	      /*! \brief value of last button state */
} tincanCb;

/*************************************************************************************************/
/*!
 *  \brief  Return TRUE if no connections with active measurements.
 *
 *  \return TRUE if no connections active.
 */
/*************************************************************************************************/
static bool_t NoConnActive(void)
{
    tincanConn_t     *pConn = tincanCb.conn;
    uint8_t       i;

    for (i = 0; i < DM_CONN_MAX; i++, pConn++) {
        if (pConn->connId != DM_CONN_ID_NONE) {
            return FALSE;
        }
    }
    return TRUE;
}

/*************************************************************************************************/
/*!
 *  \brief  ATTS write callback for maxim custom service.  Use this function as a parameter
 *          to SvcMcsCbackRegister().
 *
 *  \return ATT status.
 */
/*************************************************************************************************/
uint8_t WriteCback(dmConnId_t connId, uint16_t handle, uint8_t operation,
                      uint16_t offset, uint16_t len, uint8_t *pValue, attsAttr_t *pAttr)
{
    AttsSetAttr(handle, sizeof(*pValue), (uint8_t*)pValue);

    APP_TRACE_INFO1("Value: %d",*pValue);
    return ATT_SUCCESS;
}

/*************************************************************************************************/

/*!
 *  \brief  Setting characteristic value and send the button value to the peer device
 *
 *  \return None.
 */
/*************************************************************************************************/
void setAndNotify(uint8_t features)
{
    uint8_t fortyTwo = 0x42;
    AttsSetAttr(BUTTON_VAL_HDL, sizeof(features), (uint8_t*)&features); /*Setting mcsButtonVal characteristic value */
    dmConnId_t connId = AppConnIsOpen(); /*Getting connected */
    if (connId != DM_CONN_ID_NONE) {
        AttsHandleValueNtf(connId, BUTTON_VAL_HDL, sizeof(fortyTwo), (uint8_t*)&fortyTwo); /*Send notify */
    }
}

/*************************************************************************************************/
/*!
 *  \brief  Initialize the mcs server.
 *
 *  \param  handerId    WSF handler ID of the application using this service.
 *  \param  pCfg        mcs configurable parameters.
 *
 *  \return None.
 */
/*************************************************************************************************/
void tincanInit(wsfHandlerId_t handlerId, tincanCfg_t *pCfg)
{
    tincanCb.btnStateChkTimer.handlerId = handlerId;
    tincanCb.cfg = *pCfg;

    /* De-init the PAL LEDs so we can control them here */
    PalLedDeInit();
}

/*************************************************************************************************/
/*!
 *  \brief  Start periodic mcs button state check.  This function starts a timer to perform
 *          periodic button checks.
 *
 *  \param  connId      DM connection identifier.
 *  \param  timerEvt    WSF event designated by the application for the timer.
 *  \param  mcsCccIdx   Index of mcs button state CCC descriptor in CCC descriptor handle table.
 *
 *  \return None.
 */
/*************************************************************************************************/
void ButtonCheckStart(dmConnId_t connId, uint8_t timerEvt, uint8_t mcsCccIdx, uint8_t btnState)
{
    /* if this is first connection */
    if (NoConnActive()) {
        /* initialize control block */
        tincanCb.btnStateChkTimer.msg.event = timerEvt;
        tincanCb.btnStateChkTimer.msg.status = mcsCccIdx;
        tincanCb.btnState = btnState;
        tincanCb.currCount = tincanCb.cfg.count;

        /* start timer */
        WsfTimerStartSec(&tincanCb.btnStateChkTimer, tincanCb.cfg.period);
    }

    /* set conn id and last sent button level */
    tincanCb.conn[connId - 1].connId = connId;
    tincanCb.conn[connId - 1].sentMcsBtnState = btnState;
}

/*************************************************************************************************/
/*!
 *  \brief  Stop periodic button state check.
 *
 *  \param  connId      DM connection identifier.
 *
 *  \return None.
 */
/*************************************************************************************************/
void ButtonCheckStop(dmConnId_t connId)
{
    /* clear connection */
    tincanCb.conn[connId - 1].connId = DM_CONN_ID_NONE;
    tincanCb.conn[connId - 1].mcsToSend = FALSE;

    /* if no remaining connections */
    if (NoConnActive()) {
        /* stop timer */
        WsfTimerStop(&tincanCb.btnStateChkTimer);
    }
}

