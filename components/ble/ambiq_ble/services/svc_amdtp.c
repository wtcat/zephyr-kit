//*****************************************************************************
//
//! @file svc_amdtp.c
//!
//! @brief AM data transfer protocol service implementation
//!
//
//*****************************************************************************

//*****************************************************************************
//
// Copyright (c) 2020, Ambiq Micro
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are met:
//
// 1. Redistributions of source code must retain the above copyright notice,
// this list of conditions and the following disclaimer.
//
// 2. Redistributions in binary form must reproduce the above copyright
// notice, this list of conditions and the following disclaimer in the
// documentation and/or other materials provided with the distribution.
//
// 3. Neither the name of the copyright holder nor the names of its
// contributors may be used to endorse or promote products derived from this
// software without specific prior written permission.
//
// Third party software included in this distribution is subject to the
// additional license terms as defined in the /docs/licenses directory.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
// AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
// IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
// ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
// LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
// CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
// SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
// INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
// CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
// ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
// POSSIBILITY OF SUCH DAMAGE.
//
// This is part of revision 2.4.2 of the AmbiqSuite Development Package.
//
//*****************************************************************************
#include "wsf_types.h"
#include "att_api.h"
#include "wsf_trace.h"
#include "bstream.h"
#include "svc_ch.h"
#include "svc_amdtp.h"
#include "svc_cfg.h"
#include "srv_ble_data.h"
//*****************************************************************************
//
// Macro definitions
//
//*****************************************************************************
//#define SOME_MACRO              42          //!< This is the answer

//*****************************************************************************
//
// Global Variables
//
//*****************************************************************************
//uint32_t g_ui32Stuff;

/**************************************************************************************************
 Static Variables
**************************************************************************************************/
/* UUIDs */
static const uint8_t svcRxUuid_GET[] = {UINT16_TO_BYTES(ATT_UUID_AMDTP_GET_RX_PART)};
static const uint8_t svcTxUuid_GET[] = {UINT16_TO_BYTES(ATT_UUID_AMDTP_GET_TX_PART)};

static const uint8_t svcRxUuid_POST[] = {UINT16_TO_BYTES(ATT_UUID_AMDTP_POST_RX_PART)};
static const uint8_t svcTxUuid_POST[] = {UINT16_TO_BYTES(ATT_UUID_AMDTP_POST_TX_PART)};

static const uint8_t svcRxUuid_PUSH[] = {UINT16_TO_BYTES(ATT_UUID_AMDTP_PUSH_RX_PART)};
static const uint8_t svcTxUuid_PUSH[] = {UINT16_TO_BYTES(ATT_UUID_AMDTP_PUSH_TX_PART)};

/**************************************************************************************************
 Service variables
**************************************************************************************************/

/* service */
static const uint8_t amdtpSvc[] = {UINT16_TO_BYTES(ATT_UUID_AMDTP_SERVICE)};
static const uint16_t amdtpLenSvc = sizeof(amdtpSvc);


/* Get write characteristic */ 
static const uint8_t amdtpRxCh_GET[] = {ATT_PROP_WRITE_NO_RSP, UINT16_TO_BYTES(AMDTPS_GET_RX_HDL), UINT16_TO_BYTES(ATT_UUID_AMDTP_GET_RX)};
static const uint16_t amdtpLenRxCh_GET = sizeof(amdtpRxCh_GET);
static const uint8_t amdtpRx_GET[] = {0};
static const uint16_t amdtpLenRx_GET = sizeof(amdtpRx_GET);

/* Get Notify characteristic */ 
static const uint8_t amdtpTxCh_GET[] = {ATT_PROP_NOTIFY, UINT16_TO_BYTES(AMDTPS_GET_TX_HDL), UINT16_TO_BYTES(ATT_UUID_AMDTP_GET_TX)};
static const uint16_t amdtpLenTxCh_GET = sizeof(amdtpTxCh_GET);
static const uint8_t amdtpTx_GET[] = {0};
static const uint16_t amdtpLenTx_GET = sizeof(amdtpTx_GET);
static uint8_t amdtpTxChCcc_GET[] = {UINT16_TO_BYTES(0x0000)};
static const uint16_t amdtpLenTxChCcc_GET = sizeof(amdtpTxChCcc_GET);

/* Post write characteristic */ 
static const uint8_t amdtpRxCh_POST[] = {ATT_PROP_WRITE_NO_RSP, UINT16_TO_BYTES(AMDTPS_POST_RX_HDL), UINT16_TO_BYTES(ATT_UUID_AMDTP_POST_RX)};
static const uint16_t amdtpLenRxCh_POST = sizeof(amdtpRxCh_POST);
static const uint8_t amdtpRx_POST[] = {0};
static const uint16_t amdtpLenRx_POST = sizeof(amdtpRx_POST);


/* Post Notify characteristic */ 
static const uint8_t amdtpTxCh_POST[] = {ATT_PROP_NOTIFY, UINT16_TO_BYTES(AMDTPS_POST_TX_HDL), UINT16_TO_BYTES(ATT_UUID_AMDTP_POST_TX)};
static const uint16_t amdtpLenTxCh_POST = sizeof(amdtpTxCh_POST);
static const uint8_t amdtpTx_POST[] = {0};
static const uint16_t amdtpLenTx_POST = sizeof(amdtpTx_POST);
static uint8_t amdtpTxChCcc_POST[] = {UINT16_TO_BYTES(0x0000)};
static const uint16_t amdtpLenTxChCcc_POST = sizeof(amdtpTxChCcc_POST);

/* Push Write characteristic */ 
static const uint8_t amdtpRxCh_PUSH[] = {ATT_PROP_WRITE_NO_RSP, UINT16_TO_BYTES(AMDTPS_PUSH_RX_HDL), UINT16_TO_BYTES(ATT_UUID_AMDTP_PUSH_RX)};
static const uint16_t amdtpLenRxCh_PUSH = sizeof(amdtpRxCh_PUSH);
static const uint8_t amdtpRx_PUSH[] = {0};
static const uint16_t amdtpLenRx_PUSH = sizeof(amdtpRx_PUSH);

/* Push Notify characteristic */ 
static const uint8_t amdtpTxCh_PUSH[] = {ATT_PROP_NOTIFY, UINT16_TO_BYTES(AMDTPS_PUSH_TX_HDL), UINT16_TO_BYTES(ATT_UUID_AMDTP_PUSH_TX)};
static const uint16_t amdtpLenTxCh_PUSH = sizeof(amdtpTxCh_PUSH);
static const uint8_t amdtpTx_PUSH[] = {0};
static const uint16_t amdtpLenTx_PUSH = sizeof(amdtpTx_PUSH);
static uint8_t amdtpTxChCcc_PUSH[] = {UINT16_TO_BYTES(0x0000)};
static const uint16_t amdtpLenTxChCcc_PUSH = sizeof(amdtpTxChCcc_PUSH);



/* Attribute list for AMDTP group */
static const attsAttr_t amdtpList[] =
{
  {
    attPrimSvcUuid, 
    (uint8_t *) amdtpSvc,
    (uint16_t *) &amdtpLenSvc, 
    sizeof(amdtpSvc),
    0,
    ATTS_PERMIT_READ
  },
// 1  
  {
    attChUuid,
    (uint8_t *) amdtpRxCh_GET,
    (uint16_t *) &amdtpLenRxCh_GET,
    sizeof(amdtpRxCh_GET),
    0,
    ATTS_PERMIT_READ
  },
  {
    svcRxUuid_GET,
    (uint8_t *) amdtpRx_GET,
    (uint16_t *) &amdtpLenRx_GET,
    ATT_VALUE_MAX_LEN,
    (ATTS_SET_VARIABLE_LEN | ATTS_SET_WRITE_CBACK),
    ATTS_PERMIT_WRITE
  },
  {
    attChUuid,
    (uint8_t *) amdtpTxCh_GET,
    (uint16_t *) &amdtpLenTxCh_GET,
    sizeof(amdtpTxCh_GET),
    0,
    ATTS_PERMIT_READ
  },
  {
    svcTxUuid_GET,
    (uint8_t *) amdtpTx_GET,
    (uint16_t *) &amdtpLenTx_GET,
    sizeof(amdtpTx_GET), 
    0, 
    0,  
  },
  {
    attCliChCfgUuid,
    (uint8_t *) amdtpTxChCcc_GET,
    (uint16_t *) &amdtpLenTxChCcc_GET,
    sizeof(amdtpTxChCcc_GET),
    ATTS_SET_CCC,
    (ATTS_PERMIT_READ | ATTS_PERMIT_WRITE)
  },


// 2 
  {
    attChUuid,
    (uint8_t *) amdtpRxCh_POST,
    (uint16_t *) &amdtpLenRxCh_POST,
    sizeof(amdtpRxCh_POST),
    0,
    ATTS_PERMIT_READ
  },
  {
    svcRxUuid_POST,
    (uint8_t *) amdtpRx_POST,
    (uint16_t *) &amdtpLenRx_POST,
    ATT_VALUE_MAX_LEN,
    (ATTS_SET_VARIABLE_LEN | ATTS_SET_WRITE_CBACK),
    ATTS_PERMIT_WRITE
  },
  {
    attChUuid,
    (uint8_t *) amdtpTxCh_POST,
    (uint16_t *) &amdtpLenTxCh_POST,
    sizeof(amdtpTxCh_POST),
    0,
    ATTS_PERMIT_READ
  },
  {
    svcTxUuid_POST,
    (uint8_t *) amdtpTx_POST,
    (uint16_t *) &amdtpLenTx_POST,
    sizeof(amdtpTx_POST), 
    0,  
    0,  
  },
  {
    attCliChCfgUuid,
    (uint8_t *) amdtpTxChCcc_POST,
    (uint16_t *) &amdtpLenTxChCcc_POST,
    sizeof(amdtpTxChCcc_POST),
    ATTS_SET_CCC,
    (ATTS_PERMIT_READ | ATTS_PERMIT_WRITE)
  },
  
// 3 
  {
    attChUuid,
    (uint8_t *) amdtpRxCh_PUSH,
    (uint16_t *) &amdtpLenRxCh_PUSH,
    sizeof(amdtpRxCh_PUSH),
    0,
    ATTS_PERMIT_READ
  },
  {
    svcRxUuid_PUSH,
    (uint8_t *) amdtpRx_PUSH,
    (uint16_t *) &amdtpLenRx_PUSH,
    ATT_VALUE_MAX_LEN,
    (ATTS_SET_VARIABLE_LEN | ATTS_SET_WRITE_CBACK),
    ATTS_PERMIT_WRITE
  },
  {
    attChUuid,
    (uint8_t *) amdtpTxCh_PUSH,
    (uint16_t *) &amdtpLenTxCh_PUSH,
    sizeof(amdtpTxCh_PUSH),
    0,
    ATTS_PERMIT_READ
  },
  {
    svcTxUuid_PUSH,
    (uint8_t *) amdtpTx_PUSH,
    (uint16_t *) &amdtpLenTx_PUSH,
    sizeof(amdtpTx_PUSH),
    0,
    0,
  },
  {
    attCliChCfgUuid,
    (uint8_t *) amdtpTxChCcc_PUSH,
    (uint16_t *) &amdtpLenTxChCcc_PUSH,
    sizeof(amdtpTxChCcc_PUSH),
    ATTS_SET_CCC,
    (ATTS_PERMIT_READ | ATTS_PERMIT_WRITE)
  },  
  
};

/* AMDTP group structure */
static attsGroup_t svcAmdtpGroup =
{
  NULL,
  (attsAttr_t *) amdtpList,
  NULL,
  NULL,
  AMDTPS_START_HDL,
  AMDTPS_END_HDL
};

/*************************************************************************************************/
/*!
 *  \fn     SvcAmdtpsAddGroup
 *        
 *  \brief  Add the services to the attribute server.
 *
 *  \return None.
 */
/*************************************************************************************************/
void SvcAmdtpsAddGroup(void)
{
  AttsAddGroup(&svcAmdtpGroup);
}

/*************************************************************************************************/
/*!
 *  \fn     SvcAmdtpRemoveGroup
 *        
 *  \brief  Remove the services from the attribute server.
 *
 *  \return None.
 */
/*************************************************************************************************/
void SvcAmdtpsRemoveGroup(void)
{
  AttsRemoveGroup(AMDTPS_START_HDL);
}

/*************************************************************************************************/
/*!
 *  \fn     SvcAmdtpsCbackRegister
 *        
 *  \brief  Register callbacks for the service.
 *
 *  \param  readCback   Read callback function.
 *  \param  writeCback  Write callback function.
 *
 *  \return None.
 */
/*************************************************************************************************/
void SvcAmdtpsCbackRegister(attsReadCback_t readCback, attsWriteCback_t writeCback)
{
  svcAmdtpGroup.readCback = readCback;
  svcAmdtpGroup.writeCback = writeCback;
}
