#include "tin_can_service.h"

/* Service variables declarations */
static const uint8_t TinCanSvcUuid[ATT_128_UUID_LEN] =   {TIN_CAN_SERVICE_UUID};
static const uint8_t TinCanSvcVal[] = {TIN_CAN_SERVICE_UUID};
static const uint16_t TinCanSvcLen = sizeof(TinCanSvcVal);

// characteristic variable for buttton uuid
static const uint8_t tincanButtonUUID[ATT_128_UUID_LEN] = {BUTTON_VALUE_CHART_UUID};

// Characteristic variables for the button declaration
static const uint8_t tincanButtonChar[] = {ATT_PROP_READ | ATT_PROP_NOTIFY, UINT16_TO_BYTES(BUTTON_VAL_HDL), BUTTON_VALUE_CHART_UUID};
static const uint16_t tincanButtonCharLen= sizeof(tincanButtonChar);
// charcteristic variables got button value
static const uint8_t tincanButtonValue[] = {0};
static const uint16_t tincanButtonValueLen = sizeof(tincanButtonValue);
// characteristic variables for button characteristic change descriptor
static const uint8_t tincanButtonValueCCC[] = {UINT16_TO_BYTES(0x0000)};
static const uint16_t tincanButtonValueCCCLen = sizeof(tincanButtonValueCCC);


/*
typedef struct
{
  uint8_t const     *pUuid;           //!< \brief Pointer to the attribute's UUID 
  uint8_t           *pValue;          //!< \brief Pointer to the attribute's value 
  uint16_t          *pLen;            //!< \brief Pointer to the length of the attribute's value 
  uint16_t          maxLen;           //!< \brief Maximum length of attribute's value
  uint8_t           settings;         //!< \brief Attribute settings 
  uint8_t           permissions;      //!< \brief Attribute permissions 
} attsAttr_t;

*/

// each characteristic is made up of the above stuct,
// even the service itself is a charateistic and thus
//needs to have variables that hold all these values
//MY_TYPE a = { .flag = true, .value = 123, .stuff = 0.456 };

// attsAttr_t service_char = {
//     .pUuid = attPrimSvcUuid,
//     .pValue =(uint8_t *) &TinCanSvcVal,
//     .pLen   =(uint16_t *) &TinCanSvcLen,
//     .maxLen = sizeof(TinCanSvcLen),
//     .settings = 0,
//     .permissions = ATTS_PERMIT_READ
// };

static const attsAttr_t characteristicList[] = {
    // serivce
    {
        attPrimSvcUuid, //spec UUID delimeting a service 
        (uint8_t *) &TinCanSvcVal,
        (uint16_t *) &TinCanSvcLen,
        sizeof(TinCanSvcLen),
        0,
        ATTS_PERMIT_READ
    },
    // Button characterisitic declaration
   {
        attChUuid, // spec UUID delimeting a characteristic 
        (uint8_t *) &tincanButtonChar,
        (uint16_t *) &tincanButtonCharLen,
        sizeof(tincanButtonCharLen),
        0,
        ATTS_PERMIT_READ
    },
    // Button characterisitic value
   {
        tincanButtonUUID, 
        (uint8_t *) &tincanButtonValue,
        (uint16_t *) &tincanButtonValueLen,
        sizeof(tincanButtonValueLen),
        0,
        ATTS_PERMIT_READ
    },
    // Button characterisitic CCC (character changde descriptor characteristic)
   {
        attCliChCfgUuid, // spec UUID delimeting a CCC 
        (uint8_t *) &tincanButtonValueCCC,
        (uint16_t *) &tincanButtonValueCCCLen,
        sizeof(tincanButtonValueCCCLen),
        ATTS_SET_CCC,
        (ATTS_PERMIT_READ | ATTS_PERMIT_WRITE)
    },

};


static attsGroup_t TinCanSvcGroup = {
    NULL,
    (attsAttr_t *) characteristicList,
    NULL,  //write callback
    NULL,  //read callback
    START_HDL,
    END_HANDLE
};

void TinCanSvcAddGroup(void)
{
    AttsAddGroup(&TinCanSvcGroup);
}

void TinCanSvcRemoveGroup(void)
{
    AttsRemoveGroup(START_HDL);
}

void TinCanSvcCbackRegister(attsReadCback_t readCback, attsWriteCback_t writeCback)
{
    TinCanSvcGroup.readCback = readCback;
    TinCanSvcGroup.writeCback = writeCback;
}