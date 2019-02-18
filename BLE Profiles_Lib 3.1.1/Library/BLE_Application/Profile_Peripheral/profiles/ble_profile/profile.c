/******************************************************************************/
/*
* THE PRESENT CODE AIMS AT PROVIDING CUSTOMERS WITH CODING INFORMATION
* REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE TIME.
* AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY DIRECT,
* INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING FROM THE
* CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE CODING
* INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
*
* (C) 2016 STMicroelectronics International NV
*
*   FILENAME        -  profile.c
*
*   COMPILER        -  EWARM
*
*******************************************************************************
*
*   CHANGE HISTORY
* ---------------------------
*   $Date$:      09/03/2016
*   $Revision$:  First version 
*   $Author$:    AMG RF Application team
*   Comments:    BLE Main Profile 
*******************************************************************************
*
*  File Description 
*  ---------------------
*  This file will have BLE Main Profile implementation. 
******************************************************************************/

/*******************************************************************************
*******************************************************************************/

/*******************************************************************************
* Include Files
*******************************************************************************/
#include <stdio.h>
#include <string.h>
#include "BlueNRG1_it.h"
#include "BlueNRG1_conf.h"
#include "ble_const.h" 
#include "bluenrg1_stack.h"
#include "gp_timer.h"
#include "SDK_EVAL_Config.h"

#include <uuid.h>
#include <hal_types.h>
#include <ble_profile.h> 
#include <ble_events.h>
#include <debug.h>
#include <osal.h>
#include <host_config.h>


/*******************************************************************************
* types
*******************************************************************************/

/**
* device information service context
* contains the handles of all the 
* characteristics added to the device
* information service as specified by bitmask of
* deviceInfoCharToBeAdded field. The value of these
* characteristics cannot be set by the
* application and will have to be set
* at the profile level itself. These values are
* hard coded at the profile level. It is assumed that
* the software, firmware revisions correspond to
* the revision of the profile code
*/ 
typedef struct _tDevInfoServiceContext
{
  /**
  * This flag keeps track of the characteristics
  * that have to be added to the device information service
  */ 
  uint16_t deviceInfoCharToBeAdded;
  
  /**
  * handle of the device information service
  */ 
  uint16_t devInfoServHandle; 
  
  /**
  * handle of the system ID characteristic
  */ 
  uint16_t systemIDCharHandle; 
  
  /**
  * handle of the model number characteristic
  */ 
  uint16_t modelNumCharHandle; 
  
  /**
  * handle of the serial number characteristic
  */ 
  uint16_t serialNumCharHandle; 
  
  /**
  * handle of the firmware revision characteristic
  */ 
  uint16_t firmwareRevisionCharHandle; 
  
  /**
  * handle of the hardware revision characteristic
  */ 
  uint16_t hardwareRevisionCharHandle; 
  
  /**
  * handle of the software revision characteristic
  */
  uint16_t softwareRevisionCharHandle; 
  
  /**
  * handle of the manufacturer name characteristic
  */
  uint16_t manufacturerNameCharHandle; 
  
  /**
  * handle of the IEEE Certification characteristic
  */
  uint16_t ieeeCretificationCharHandle; 
  
  /**
  * handle of the PNP ID characteristic
  */
  uint16_t pnpIDCharHandle; 
}tDevInfoServiceContext;

typedef struct
{
  /**
  * list of all the currently
  * registered profiles
  */ 
  tListNode bleRegstedProfileList;
  
  /**
  * callback to be called to notify the application
  */ 
  BLE_CALLBACK_FUNCTION_TYPE bleProfileApplNotifyFunc;
  
  /**
  * number of profiles currently registered
  * with the main profile
  */ 
  uint8_t bleProfileNumOfRegsProfiles;
  
  /**
  * this is a counter to maintain the status of initialization of all profiles
  * Each time a profile notifies that it has completed its initialization, 
  * this counter will be incremented. The device will be allowed to start 
  * advertising only when all the profiles have completd theur initialization
  */	
  uint8_t bleProfileCanAdvertise;
  
  /**
  * main state of the profile state
  * machine
  */ 
  tProfileState bleProfileMainState;
  
  /**
  * sub state of the profile state
  * machine
  */ 
  tProfileState bleProfileSubState;
  
  /**
  * contains the information related to
  * device information service
  */
  tDevInfoServiceContext devInfoService;
  
}tBLEProfileCtxt;


/*******************************************************************************
* Variable Declarations
*******************************************************************************/
tBLEProfileGlobalContext gBLEProfileContext;
static tBLEProfileCtxt gMainProfileCtxt;
static uint8_t bleProfileIintFlag = 0;
BOOL profileProcess = FALSE;
/*******************************************************************************
* Imported Variable
*******************************************************************************/
extern void *profiledbgfile;

/*******************************************************************************
* Function Prototypes 
*******************************************************************************/

static tBleStatus BLE_Profile_GATT_Procedure_Complete_EvtHandler(uint16_t procedureCode);

static tBleStatus BLE_Profile_Is_Registered_Profile (tProfileInterfaceContext* profileContext);

tBleStatus Add_DIS_Charac(void);

//extern void HCI_Enable_GATT_Cmd(uint16_t opcode);
/*******************************************************************************
* SAP
*******************************************************************************/

tBleStatus BLE_Profile_Init(tSecurityParams * securityParameters, 
                            BLE_CALLBACK_FUNCTION_TYPE bleCallbackFunc)
{
  PROFILE_MESG_DBG (profiledbgfile, "Initializing BLE Profile \n");
  tBleStatus status = 0;
  
  if (bleProfileIintFlag != 0)
  {
    PROFILE_MESG_DBG (profiledbgfile, "BLE Profile already Initialized !! \n");
    return (BLE_STATUS_NOT_ALLOWED);
  }
  
  if ((securityParameters == NULL) || (bleCallbackFunc == NULL))
  {
    return (BLE_STATUS_NULL_PARAM);
  }
  
  Osal_MemSet((void *)&gBLEProfileContext,0,sizeof(tBLEProfileGlobalContext));
  Osal_MemSet((void *)&gMainProfileCtxt,0,sizeof(gMainProfileCtxt));
  
  Osal_MemCpy((void *)&(gBLEProfileContext.bleSecurityParam), 
              (void *)securityParameters, 
              sizeof(tSecurityParams));
  
  /* save the application notification function ptr */
  gMainProfileCtxt.bleProfileApplNotifyFunc = bleCallbackFunc;
  
  list_init_head (&gMainProfileCtxt.bleRegstedProfileList);
  
  gBLEProfileContext.advtServUUIDlen = 1;
  gBLEProfileContext.advtServUUID[0] = AD_TYPE_16_BIT_SERV_UUID;
  
  /* initialize ble profile states */
  gMainProfileCtxt.bleProfileMainState = BLE_PROFILE_STATE_INIT_BLE;
  gMainProfileCtxt.bleProfileSubState = BLE_PROFILE_STATE_DETECT_BLE;
  
  /* set the task initialization flag */
  bleProfileIintFlag = 0x01;
  
  return (status);
}

static tBleStatus Configure_Profile(void)
{
  tBleStatus ret;
  
  PROFILE_MESG_DBG ( profiledbgfile, "Initializing GATT Server \n" );
  ret = aci_gatt_init();
  if(ret != BLE_STATUS_SUCCESS)
    goto INIT_PROFILE_FAIL;
  PROFILE_MESG_DBG (profiledbgfile, "GATT Initialized Successfully \n");
  
  ret = aci_gap_init(GAP_PERIPHERAL_ROLE, 
                     0, 
                     0x07,
                     &gBLEProfileContext.gapServiceHandle, 
                     &gBLEProfileContext.devNameCharHandle, 
                     &gBLEProfileContext.appearanceCharHandle);
  
  if(ret!=BLE_STATUS_SUCCESS)
    goto INIT_PROFILE_FAIL;
  PROFILE_MESG_DBG (profiledbgfile, "GAP Initialized Successfully \n");
  ret = aci_gap_set_io_capability(gBLEProfileContext.bleSecurityParam.ioCapability);
  if(ret!=BLE_STATUS_SUCCESS)
    goto INIT_PROFILE_FAIL;
  PROFILE_MESG_DBG (profiledbgfile, "I/O capability Set Successfully \n");
#if BLE_2_X /* BLE stack FW version >= 2.x*/  
  ret = aci_gap_set_authentication_requirement(gBLEProfileContext.bleSecurityParam.bonding_mode,
                                               gBLEProfileContext.bleSecurityParam.mitm_mode,
                                               gBLEProfileContext.bleSecurityParam.SC_Support, 
                                               gBLEProfileContext.bleSecurityParam.KeyPress_Notification_Support,
                                               gBLEProfileContext.bleSecurityParam.encryptionKeySizeMin,
                                               gBLEProfileContext.bleSecurityParam.encryptionKeySizeMax,
                                               gBLEProfileContext.bleSecurityParam.Use_Fixed_Pin,
                                               gBLEProfileContext.bleSecurityParam.Fixed_Pin,
                                               gBLEProfileContext.bleSecurityParam.Identity_Address_Type);
 
#else
  ret = aci_gap_set_authentication_requirement(gBLEProfileContext.bleSecurityParam.mitm_mode,
                                               gBLEProfileContext.bleSecurityParam.OOB_Data_Present, 
                                               gBLEProfileContext.bleSecurityParam.OOB_Data,
                                               gBLEProfileContext.bleSecurityParam.encryptionKeySizeMin,
                                               gBLEProfileContext.bleSecurityParam.encryptionKeySizeMax,
                                               gBLEProfileContext.bleSecurityParam.Use_Fixed_Pin,
                                               gBLEProfileContext.bleSecurityParam.Fixed_Pin,
                                               gBLEProfileContext.bleSecurityParam.bonding_mode);
#endif  
  if(ret!=BLE_STATUS_SUCCESS)
    goto INIT_PROFILE_FAIL;
  PROFILE_MESG_DBG (profiledbgfile, "Authentication Req set Successfully \n");
  if(gBLEProfileContext.bleSecurityParam.bonding_mode)
  {
    ret = aci_gap_configure_whitelist();
    if(ret!=BLE_STATUS_SUCCESS)
      goto INIT_PROFILE_FAIL;
    PROFILE_MESG_DBG (profiledbgfile, "White List Configured Successfully \n");
  }
  if(gMainProfileCtxt.devInfoService.deviceInfoCharToBeAdded)
  {
    /* we have to add the device information service */
    
    uint16_t uuid;
    
    HOST_TO_LE_16((uint8_t *)&uuid, DEVICE_INFORMATION_SERVICE_UUID);
    ret = aci_gatt_add_service(UUID_TYPE_16, 
                               (Service_UUID_t *) &uuid, 
                               PRIMARY_SERVICE, 
                               MAX_DIS_LEN, 
                               &gMainProfileCtxt.devInfoService.devInfoServHandle);   
   
    if(ret!=BLE_STATUS_SUCCESS)
      goto INIT_PROFILE_FAIL;
    PROFILE_MESG_DBG (profiledbgfile, "Device Information Service is added successfully \n");
    
    PROFILE_MESG_DBG ( profiledbgfile, "Device Information Service Handle: %04X\n",
                      gMainProfileCtxt.devInfoService.devInfoServHandle);
    
    ret = Add_DIS_Charac();
    if(ret!=BLE_STATUS_SUCCESS)
      goto INIT_PROFILE_FAIL;
    PROFILE_MESG_DBG (profiledbgfile, "Device Information characteristics added successfully \n");
  }
  
  return BLE_STATUS_SUCCESS;
  
INIT_PROFILE_FAIL:
  PROFILE_MESG_ERR(profiledbgfile, "Error while initializing main profile.\n");
  return ret;
}

int Profile_Process_Q()
{
  uint8_t doNotifyStateChange = 0;
  tBleStatus cmdResult = BLE_STATUS_FAILED;
  
  if (bleProfileIintFlag == 0x00)
  {
    /* the profile is not yet initialized
    * so donot do any processing
    */ 
    return 0x01;
  }
 
  if(profileProcess)
  {
//    PROFILE_MESG_DBG(profiledbgfile,"Profile_Process_Q\n");
//    
//    PROFILE_MESG_DBG(profiledbgfile, "BLE profile MainState %02X, SubState %02X \r\n", gMainProfileCtxt.bleProfileMainState, gMainProfileCtxt.bleProfileSubState);
//    
    doNotifyStateChange = 0;
    cmdResult = BLE_STATUS_FAILED;
    
    switch (gMainProfileCtxt.bleProfileMainState)
    {
    case BLE_PROFILE_STATE_INIT_BLE:
      {
        /* Steps for controller stack Initialization 
        * 1. GATT Init
        * 2. GAP Init
        * 3. set IO capabilities
        * 4. set authentication requirement
        * 5. set event mask
        * 6. set le event mask
        * 7. set GAP event mask
        * 8. set GATT event mask 
        * 9. configure whitelist 
        */
        if (gMainProfileCtxt.bleProfileSubState != BLE_PROFILE_STATE_INITIALIZATION_ERROR)
        {  
          if(Configure_Profile() == BLE_STATUS_SUCCESS){
            
            /* notify sub profiles that main profile initialization is complete
            * so that the profile specific initialization can be done
            */ 
            gMainProfileCtxt.bleProfileMainState = BLE_PROFILE_STATE_INIT_PROFILE;
            gMainProfileCtxt.bleProfileSubState = BLE_PROFILE_INVALID;
          }
          else {
            /* the profile initialization was completed with an error
            * notify the application
            */
            gMainProfileCtxt.bleProfileMainState = BLE_PROFILE_STATE_INIT_BLE;
            gMainProfileCtxt.bleProfileSubState = BLE_PROFILE_STATE_INITIALIZATION_ERROR;
          }
          doNotifyStateChange = 1;
        }
        else
        {
          doNotifyStateChange = 0;
        }
      }
      break; /* BLE_PROFILE_STATE_INIT_BLE: */
    case BLE_PROFILE_STATE_INIT_PROFILE:
      {
        /* the profile task is waiting here for votes from all sub profiles to 
        * go to advertisable state. */
        /* check the advertising vote counter and change state to Advertisable 
        * if all the registered profiles have been voted */
        /* ENTER CRITICAL SECTION */
        if (gMainProfileCtxt.bleProfileCanAdvertise == gMainProfileCtxt.bleProfileNumOfRegsProfiles)
        {
          /* Set the BLE Profile task into connectable state */
          gMainProfileCtxt.bleProfileMainState = BLE_PROFILE_STATE_CONNECTABLE;
          gMainProfileCtxt.bleProfileSubState = BLE_PROFILE_STATE_CONNECTABLE_IDLE;
          
          /* Now how to notify the sub profiles about main profile state change */
          doNotifyStateChange = 1;
        }
      }
      break;
    case BLE_PROFILE_STATE_CONNECTING:
      {
        switch (gMainProfileCtxt.bleProfileSubState)
        {
        case BLE_PROFILE_STATE_PAIRING_STARTED:
          {
            PROFILE_MESG_DBG ( profiledbgfile, "pair\n");
            PROFILE_MESG_DBG ( profiledbgfile, "send pairing request\n");
#if BLE_2_X /* BLE stack FW version >= 2.x*/
            cmdResult = aci_gap_slave_security_req(gBLEProfileContext.connectionHandle);
#else
            cmdResult = aci_gap_slave_security_req(gBLEProfileContext.connectionHandle,
                                                   gBLEProfileContext.bleSecurityParam.bonding_mode,
                                                   gBLEProfileContext.bleSecurityParam.mitm_mode);
#endif
            if (cmdResult == BLE_STATUS_SUCCESS){
              PROFILE_MESG_DBG ( profiledbgfile, "Slave Security Request has been sent to master \n");
              gMainProfileCtxt.bleProfileSubState = BLE_PROFILE_STATE_SLAVE_SECURITY_REQUESTED;
            }
            else {
              /* security could not be requested */
              gMainProfileCtxt.bleProfileMainState = BLE_PROFILE_STATE_CONNECTED;
            }
          }
          break;
        case BLE_PROFILE_STATE_SLAVE_SECURITY_REQUESTED:
          break;
        case BLE_PROFILE_STATE_SLAVE_SECURITY_INITIATED:
          break;
        case BLE_PROFILE_STATE_PASS_KEY_REQUESTED:
          break;
        case BLE_PROFILE_STATE_PAIRING_COMPLETE_WAITING:
          break;
        }
      }
      break; /* BLE_PROFILE_STATE_CONNECTING */
    case BLE_PROFILE_STATE_CONNECTED:
      {
        switch (gMainProfileCtxt.bleProfileSubState)
        {
        case BLE_PROFILE_STATE_CONNECTED_IDLE:
          break;
        case BLE_PROFILE_STATE_EXCHANGE_CONFIG:
          {
            PROFILE_MESG_DBG ( profiledbgfile, "Starting Configuration Exchange Process \n");
            cmdResult = aci_gatt_exchange_config(gBLEProfileContext.connectionHandle);
            if (cmdResult == BLE_STATUS_SUCCESS)
            {
              PROFILE_MESG_DBG(profiledbgfile, "BLE profile MainState %02X, SubState %02X,aci_gatt_exchange_configuration() OK \n", gMainProfileCtxt.bleProfileMainState, gMainProfileCtxt.bleProfileSubState);
              /* change profile's sub states */
              gMainProfileCtxt.bleProfileSubState = BLE_PROFILE_STATE_EXCHANGING_CONFIG;
            }
            else
              PROFILE_MESG_DBG(profiledbgfile, "BLE profile MainState %02X, SubState %02X,aci_gatt_exchange_configuration() failed %02X \n", gMainProfileCtxt.bleProfileMainState, gMainProfileCtxt.bleProfileSubState,cmdResult);
          }
          break;
        case BLE_PROFILE_STATE_EXCHANGING_CONFIG:
          break;
        case BLE_PROFILE_STATE_DISCONNECTING:
          break;
        }
      }
      break; /* BLE_PROFILE_STATE_CONNECTED */
    }
    
    if (!doNotifyStateChange)
    {
      profileProcess = FALSE;
    }
  }
  
  return profileProcess;
}

tProfileState BLE_Profile_Get_Main_State ()
{
  return (gMainProfileCtxt.bleProfileMainState);
}

tProfileState BLE_Profile_Get_Sub_State ()
{
  return (gMainProfileCtxt.bleProfileSubState);
}

tBleStatus Is_Profile_Present(uint8_t profileID)
{
  tListNode * node = NULL;
  tProfileInterfaceContext *temp;
  
  PROFILE_MESG_DBG(profiledbgfile, "Is_Profile_Present\n");
  
  list_get_next_node (&gMainProfileCtxt.bleRegstedProfileList, &node);
  while (node != &gMainProfileCtxt.bleRegstedProfileList)
  {
    temp = (tProfileInterfaceContext*)node;
    if (temp->profileID == profileID)
    {
      PROFILE_MESG_DBG(profiledbgfile, "found profile %x\n",profileID);
      /* profile is a registered one */
      return BLE_STATUS_SUCCESS;
    }
    /* get the next node "registered profile context" in the list */
    list_get_next_node (node, &node);
  }
  PROFILE_MESG_DBG(profiledbgfile, "did not find profile %x\n",profileID);
  return BLE_STATUS_FAILED;
}


tBleStatus BLE_Profile_Register_Profile ( tProfileInterfaceContext * profileContext)
{
  if (profileContext == NULL)
  {
    return (BLE_STATUS_NULL_PARAM);
  }
 
  /* Check whether the profile is already registered */
  if ((BLE_Profile_Is_Registered_Profile (profileContext)) == BLE_STATUS_SUCCESS)
  {
    /* profile is already registered */
    return (BLE_STATUS_FAILED);
  }
  
  /* Check whether at this point the main profile can accept a new proifle */
  if ((gMainProfileCtxt.bleProfileMainState == BLE_PROFILE_STATE_INIT_BLE) && 
      (gMainProfileCtxt.bleProfileSubState != BLE_PROFILE_STATE_INITIALIZATION_ERROR))
  {
    /* BLE main profile can accept a new profile */
  }
  else
  {
    /* BLE profile is not in the state to accept a new profile at this time */
    return (BLE_STATUS_FAILED);
  }
  
  PROFILE_MESG_DBG(profiledbgfile,"[b]DISCharRequired %x\n",gMainProfileCtxt.devInfoService.deviceInfoCharToBeAdded);
  gMainProfileCtxt.devInfoService.deviceInfoCharToBeAdded |= profileContext->DISCharRequired;
  PROFILE_MESG_DBG(profiledbgfile,"[a]DISCharRequired %x\n",gMainProfileCtxt.devInfoService.deviceInfoCharToBeAdded);
  
  gMainProfileCtxt.bleProfileNumOfRegsProfiles++;
  
  /* add the new profile interface context to registered profile list */
  list_insert_tail ( (tListNode *)&gMainProfileCtxt.bleRegstedProfileList, (tListNode *)profileContext );
  
  return (BLE_STATUS_SUCCESS);
}


tBleStatus BLE_Profile_Unregister_Profile ( tProfileInterfaceContext * profileContext)
{
  if (profileContext == NULL)
  {
    return (BLE_STATUS_NULL_PARAM);
  }
  
  /* first verify that the requesting profile is actually registered with BLE main profile */
  if ((BLE_Profile_Is_Registered_Profile (profileContext)) != BLE_STATUS_SUCCESS)
  {
    /* profile not registered */
    return (BLE_STATUS_FAILED);
  }
  
  /* here the profile context cant be removed directly from the registered list, because at 
  * the moment the profile call this function, the same list is being used for notification 
  * processing. hence removing current profile context will cause problem. */
  list_remove_node ((tListNode *)profileContext);
  gMainProfileCtxt.bleProfileNumOfRegsProfiles--;
  
  return (BLE_STATUS_SUCCESS);
}

tBleStatus BLE_Profile_Vote_For_Advertisable_State(tProfileInterfaceContext* profileContext)
{
  /* verify that the requesting profile is registered with BLE main profile */
  if ((BLE_Profile_Is_Registered_Profile (profileContext)) != BLE_STATUS_SUCCESS)
  {
    /* profile not registered */
    return (BLE_STATUS_FAILED);
  }
  
  /* check that requesting profile has not voted before for Advertisable state */
  if (profileContext->voteForAdvertisableState != 0)
  {
    /* the profile has already voted for advertisable state */
    return (BLE_STATUS_FAILED);
  }
  
  /* The host can command the controller to go into advertising state only when all
  * the profiles have completed their initialization in the scenario where there are
  * multiple profiles running. So each profile will notify the main profile that
  * it has completed its initialization. The main profile will allow the advertising
  * command only when all registered profiles notify that they have completed their
  * profile specific initialization
  */
  gMainProfileCtxt.bleProfileCanAdvertise++;
  
  profileContext->voteForAdvertisableState = 1;
  
  profileProcess = 0x01;
  
  return (BLE_STATUS_SUCCESS);    
}

void BLE_Profile_Change_Advertise_Sub_State(tProfileInterfaceContext*  profileContext, tProfileState nextState)
{
  /* if the main state of the ble profile is connectable and
  * the profile requesting for a state change is a registered
  * profile, then change the state
  */ 
  if((gMainProfileCtxt.bleProfileMainState == BLE_PROFILE_STATE_CONNECTABLE) &&
     (BLE_Profile_Is_Registered_Profile(profileContext) == BLE_STATUS_SUCCESS))
  {
    gMainProfileCtxt.bleProfileSubState = nextState;
  }
}

/**
* BLE_Profile_Set_State
* 
* @param[in] state : state to be set
* 
* sets the substate of the BLE profile
*/ 
void BLE_Profile_Set_State(tProfileState state)
{
  gMainProfileCtxt.bleProfileSubState = state;
}

tBleStatus BLE_Profile_Disconnect()
{
  PROFILE_MESG_DBG(profiledbgfile,"BLE_Profile_Disconnect\n");
  
  if((gMainProfileCtxt.bleProfileMainState == BLE_PROFILE_STATE_CONNECTED) ||
     (gMainProfileCtxt.bleProfileMainState == BLE_PROFILE_STATE_CONNECTING))
  {
    PROFILE_MESG_DBG(profiledbgfile,"Sending disconnect command\n");
    
    if(aci_gap_terminate(gBLEProfileContext.connectionHandle, 0x13) == BLE_STATUS_SUCCESS)
    {  
      PROFILE_MESG_DBG ( profiledbgfile, "Disconnection Processing !! \n");
      gMainProfileCtxt.bleProfileSubState = BLE_PROFILE_STATE_DISCONNECTING;
      
      return BLE_STATUS_SUCCESS;
    }
    else
    {
      PROFILE_MESG_DBG ( profiledbgfile, "Disconnection Error !! \n");
    }
  }
  return BLE_STATUS_FAILED;
}

tBleStatus BLE_Profile_Send_Pass_Key(uint32_t passkey)
{
  tBleStatus retval = BLE_STATUS_FAILED;
  
  PROFILE_MESG_DBG(profiledbgfile,"BLE_Profile_Send_Pass_Key\n");
  if(((gMainProfileCtxt.bleProfileMainState == BLE_PROFILE_STATE_CONNECTING) ||
      (gMainProfileCtxt.bleProfileMainState == BLE_PROFILE_STATE_CONNECTED)) && 
     (gMainProfileCtxt.bleProfileSubState == BLE_PROFILE_STATE_PASS_KEY_REQUESTED))
  {
    retval = aci_gap_pass_key_resp(gBLEProfileContext.connectionHandle, passkey);
    
    if (retval == BLE_STATUS_SUCCESS)
    {
      PROFILE_MESG_DBG ( profiledbgfile, "Pass Key Response Accepted \n");
      
      /* as pass key has been send to master as authentication parameter, the only
      * thing remained to the pairing complete event. hence wait for pairing complete. 
      */
      gMainProfileCtxt.bleProfileSubState = BLE_PROFILE_STATE_PAIRING_COMPLETE_WAITING;
    }
    else
    {
      PROFILE_MESG_DBG ( profiledbgfile, "Pass Key Response Declined \n");
      gMainProfileCtxt.bleProfileSubState = BLE_PROFILE_STATE_PAIRING_STARTED;
    }
  }
  
  return (retval);
}

tBleStatus BLE_Profile_Start_Configuration_Exchange(void)
{
  PROFILE_MESG_DBG ( profiledbgfile, "BLE_Profile_Start_Configuration_Exchange: %02X, %02X \n", gMainProfileCtxt.bleProfileMainState, gMainProfileCtxt.bleProfileSubState);
  
  if ( (gMainProfileCtxt.bleProfileMainState == BLE_PROFILE_STATE_CONNECTED) && 
      (gMainProfileCtxt.bleProfileSubState == BLE_PROFILE_STATE_CONNECTED_IDLE) )
  {
    gMainProfileCtxt.bleProfileSubState = BLE_PROFILE_STATE_EXCHANGE_CONFIG;
  }
  
  return (BLE_STATUS_SUCCESS);
}

void BLE_Profile_Add_Advertisment_Service_UUID(uint16_t servUUID)
{
  uint8_t indx = gBLEProfileContext.advtServUUIDlen;
  
  gBLEProfileContext.advtServUUID[indx] = (uint8_t)(servUUID & 0xFF);
  indx++;
  gBLEProfileContext.advtServUUID[indx] = (uint8_t)(servUUID >> 8) & 0xFF;
  indx++;
  gBLEProfileContext.advtServUUIDlen = indx;
  PROFILE_MESG_DBG(profiledbgfile,"[add UUID]%x ,%d\n",servUUID,gBLEProfileContext.advtServUUIDlen);
}

tBleStatus BLE_Profile_Remove_Advertisment_Service_UUID(uint16_t servUUID)
{
  tBleStatus retval = BLE_STATUS_FAILED;
  uint8_t indx = 1;
  uint16_t uuid;
  while (indx < gBLEProfileContext.advtServUUIDlen)
  {
    uuid = (uint16_t)(gBLEProfileContext.advtServUUID[indx] >> 8);
    indx++;
    uuid |= (uint16_t)(gBLEProfileContext.advtServUUID[indx]);
    indx++;
    if (uuid == servUUID)
    {
      /* this is the UUID we need to remove from list */
      indx -= 2;
      for (; indx < gBLEProfileContext.advtServUUIDlen ; indx++ )
      {
        gBLEProfileContext.advtServUUID[indx] = gBLEProfileContext.advtServUUID[indx + 1];
      }
      gBLEProfileContext.advtServUUIDlen -= 2;
      retval = BLE_STATUS_SUCCESS;
      break;
    }
  }
  
  return (retval);
}


/**
* Function called by hci_disconnection_complete_event of relative profile 
*/
void profile_disconnection_complete_event(uint8_t Status,
                                          uint16_t Connection_Handle,
                                          uint8_t Reason)
{
  uint8_t notifyApp = 0x00;
  uint8_t appNotifEvt;
  uint8_t appNotifStatus = BLE_STATUS_SUCCESS;
  
  profileProcess = TRUE;
  
  PROFILE_MESG_DBG(profiledbgfile, "Received event hci_disconnection_complete_event: %2x\n", Status);
  
  if ((gMainProfileCtxt.bleProfileMainState == BLE_PROFILE_STATE_CONNECTING) || 
      (gMainProfileCtxt.bleProfileMainState == BLE_PROFILE_STATE_CONNECTED) )
  {
    /* change state */
    if (Connection_Handle == gBLEProfileContext.connectionHandle)
    {
      if (Status == BLE_STATUS_SUCCESS)
      {
        gMainProfileCtxt.bleProfileMainState = BLE_PROFILE_STATE_CONNECTABLE;
        gMainProfileCtxt.bleProfileSubState = BLE_PROFILE_STATE_CONNECTABLE_IDLE;
      }
      notifyApp = 0x01;
      appNotifEvt = EVT_MP_DISCONNECTION_COMPLETE;
      appNotifStatus = Status;
    }
  }
  
  if(notifyApp)
  {
    //PROFILE_MESG_DBG(profiledbgfile, "notifying application\n");
    gMainProfileCtxt.bleProfileApplNotifyFunc(appNotifEvt,1,&appNotifStatus);
  }
}

/**
* Function called by hci_read_remote_version_information_complete_event of relative profile 
*/ 
void profile_le_connection_complete_event(uint8_t Status,
                                          uint16_t Connection_Handle,
                                          uint8_t Role,
                                          uint8_t Peer_Address_Type,
                                          uint8_t Peer_Address[6],
                                          uint16_t Conn_Interval,
                                          uint16_t Conn_Latency,
                                          uint16_t Supervision_Timeout,
                                          uint8_t Master_Clock_Accuracy)
{
  uint8_t notifyApp = 0x00;
  uint8_t appNotifEvt;
  uint8_t appNotifStatus = BLE_STATUS_SUCCESS;
  
  profileProcess = TRUE;
  
  //PROFILE_MESG_DBG ( profiledbgfile, "Received event hci_le_connection_complete_event %2x\n", Status);
  /* check task states */
  if(gMainProfileCtxt.bleProfileMainState == BLE_PROFILE_STATE_CONNECTABLE) 
  {
    /* check comand status */
    if (Status == BLE_STATUS_SUCCESS)
    {
      /* save connection handle */
      gBLEProfileContext.connectionHandle = Connection_Handle;
      
      if(gBLEProfileContext.bleSecurityParam.initiateSecurity == SLAVE_INITIATE_SECURITY)
      {
        gMainProfileCtxt.bleProfileMainState = BLE_PROFILE_STATE_CONNECTING;
        gMainProfileCtxt.bleProfileSubState = BLE_PROFILE_STATE_PAIRING_STARTED;
      }
      else if(gBLEProfileContext.bleSecurityParam.initiateSecurity == WAIT_FOR_PAIRING_CMPLT)
      {
        gMainProfileCtxt.bleProfileMainState = BLE_PROFILE_STATE_CONNECTING;
        gMainProfileCtxt.bleProfileSubState = BLE_PROFILE_STATE_WAIT_FOR_PAIRING;
      }
      else
      {
        gMainProfileCtxt.bleProfileMainState = BLE_PROFILE_STATE_CONNECTED;
        gMainProfileCtxt.bleProfileSubState = BLE_PROFILE_STATE_CONNECTED_IDLE;
      }
    }
    else
    {
      /* connection was not successful, then we need to wait for
      * the application to put the BLE in discoverable mode again
      */
      gMainProfileCtxt.bleProfileMainState = BLE_PROFILE_STATE_CONNECTABLE;
      gMainProfileCtxt.bleProfileSubState = BLE_PROFILE_STATE_CONNECTABLE_IDLE;
    }
    notifyApp = 0x01;
    appNotifEvt = EVT_MP_CONNECTION_COMPLETE;
    appNotifStatus = Status;
    
    /* Stop if any timer is running */
  }
  
  if(notifyApp)
  {
    //PROFILE_MESG_DBG(profiledbgfile, "notifying application\n");
    gMainProfileCtxt.bleProfileApplNotifyFunc(appNotifEvt,1,&appNotifStatus);
  }
}

#if BLE_2_X /* BLE stack FW version >= 2.x: BLE Security v4.2 is supported */
/**
 * Function called by aci_gap_pairing_complete_event of relative profile 
 */ 
void profile_gap_pairing_complete_event(uint16_t Connection_Handle,
                                        uint8_t Status,
                                        uint8_t Reason)
#else
void profile_gap_pairing_complete_event(uint16_t Connection_Handle,
                                        uint8_t Status)
#endif
{
  uint8_t notifyApp = 0x00;
  uint8_t appNotifEvt;
  uint8_t appNotifStatus = BLE_STATUS_SUCCESS;
  
  profileProcess = TRUE;
  PROFILE_MESG_DBG(profiledbgfile, "Received event profile_gap_pairing_complete_event: %2x\n", Status);
  
  if ( (gMainProfileCtxt.bleProfileMainState == BLE_PROFILE_STATE_CONNECTING) ||
      (gMainProfileCtxt.bleProfileMainState == BLE_PROFILE_STATE_CONNECTED) )
  {
    if (Status == BLE_STATUS_SUCCESS)
    {
      /* Pairing successful. Now the device is paired with another BT device. */
      gMainProfileCtxt.bleProfileMainState = BLE_PROFILE_STATE_CONNECTED;
      gMainProfileCtxt.bleProfileSubState = BLE_PROFILE_STATE_CONNECTED_IDLE;
    }
    else
    {
      if(gBLEProfileContext.bleSecurityParam.initiateSecurity == SLAVE_INITIATE_SECURITY)
      {
        gMainProfileCtxt.bleProfileMainState = BLE_PROFILE_STATE_CONNECTING;
        gMainProfileCtxt.bleProfileSubState = BLE_PROFILE_STATE_PAIRING_STARTED;
      }
      else if(gBLEProfileContext.bleSecurityParam.initiateSecurity == WAIT_FOR_PAIRING_CMPLT)
      {
        gMainProfileCtxt.bleProfileMainState = BLE_PROFILE_STATE_CONNECTING;
        gMainProfileCtxt.bleProfileSubState = BLE_PROFILE_STATE_WAIT_FOR_PAIRING;
      }
      else
      {
        gMainProfileCtxt.bleProfileMainState = BLE_PROFILE_STATE_CONNECTED;
        gMainProfileCtxt.bleProfileSubState = BLE_PROFILE_STATE_CONNECTED_IDLE;
      }
    }
    
    notifyApp = 0x01;
    appNotifStatus = Status;
    appNotifEvt = EVT_MP_PAIRING_COMPLETE;
  }
  
  if(notifyApp)
  {
    //PROFILE_MESG_DBG(profiledbgfile, "notifying application\n");
    gMainProfileCtxt.bleProfileApplNotifyFunc(appNotifEvt,1,&appNotifStatus);
  }
}

/**
* Function called by aci_gap_pass_key_req_event of relative profile 
*/ 
void profile_gap_pass_key_req_event(uint16_t Connection_Handle)
{
  profileProcess = TRUE;
  
  PROFILE_MESG_DBG ( profiledbgfile, "Received event aci_gap_pass_key_req_event \n");
  /* check state */
  if ((gMainProfileCtxt.bleProfileMainState == BLE_PROFILE_STATE_CONNECTING) ||
      (gMainProfileCtxt.bleProfileMainState == BLE_PROFILE_STATE_CONNECTED))
  {
    PROFILE_MESG_DBG ( profiledbgfile, "Requesting Authentication key from application \n");
    gMainProfileCtxt.bleProfileSubState = BLE_PROFILE_STATE_PASS_KEY_REQUESTED;
    /* pass event to application for user provided pass key 
     * if using fixed pin then we would not have
     * got this event at all. So no need to check for
     * fixed pin option here
     */
    gMainProfileCtxt.bleProfileApplNotifyFunc(EVT_MP_PASSKEY_REQUEST,0,NULL);
  }
}

/**
* Function called by aci_gap_authorization_req_event of relative profile 
*/
void profile_gap_authorization_req_event(uint16_t Connection_Handle)
{
  profileProcess = TRUE;
  
  PROFILE_MESG_DBG ( profiledbgfile, "Received event aci_gap_authorization_req_event \n");
  /* send the authorization response.
  * currently we donot have a profile 
  * which requires authorization
  */ 
  aci_gap_authorization_resp(gBLEProfileContext.connectionHandle, CONNECTION_AUTHORIZED);
}

/**
* Function called by aci_gap_slave_security_initiated_event of relative profile 
*/
void profile_gap_slave_security_initiated_event(void)
{
  profileProcess = TRUE;
  
  PROFILE_MESG_DBG ( profiledbgfile, "Received event aci_gap_slave_security_initiated_event \n");          
  if ((gMainProfileCtxt.bleProfileMainState == BLE_PROFILE_STATE_CONNECTING) && 
      (gMainProfileCtxt.bleProfileSubState == BLE_PROFILE_STATE_SLAVE_SECURITY_REQUESTED))
  {
    if (gBLEProfileContext.bleSecurityParam.mitm_mode)
    {
      /* man in the middle is required to exchange pass key for authentication process.
      * hence wait in the same state for pass key request from master side */
      PROFILE_MESG_DBG ( profiledbgfile, "Man in the middle is required for authentication \n");
      gMainProfileCtxt.bleProfileSubState = BLE_PROFILE_STATE_SLAVE_SECURITY_INITIATED;
    }
    else
    {
      /* if mitm is not required, we are only left for pairing complete event */
      PROFILE_MESG_DBG ( profiledbgfile, "Man in the middle is not required for authentication \n");
      gMainProfileCtxt.bleProfileSubState = BLE_PROFILE_STATE_PAIRING_COMPLETE_WAITING;
    }
  }
}

/**
* Function called by aci_gatt_proc_timeout_event of relative profile 
*/
void profile_gatt_proc_timeout_event(uint16_t Connection_Handle)
{
  profileProcess = TRUE;
  PROFILE_MESG_DBG ( profiledbgfile, "Received event aci_gatt_proc_timeout_event \n");
  /* when a gatt procedure timeout occurs no more gatt pdus can be exchanged
  * since profiles are hugely dependant on GATT we can as well start a 
  * disconnection here
  */ 
  BLE_Profile_Disconnect(); 
}

/**
* Function called by aci_gatt_proc_complete_event of relative profile 
*/
void profile_gatt_proc_complete_event(uint16_t Connection_Handle,
                                      uint8_t Error_Code)
{
  uint16_t gattProcedureOpcode = 0x00;
  
  profileProcess = TRUE;
  
  PROFILE_MESG_DBG ( profiledbgfile, "Received event aci_gatt_proc_complete_event \n");
  gattProcedureOpcode = BLE_Profile_Get_Current_GATT_Procedure_Opcode();
  BLE_Profile_GATT_Procedure_Complete_EvtHandler (gattProcedureOpcode);

  if(gattProcedureOpcode)
  {
//    PROFILE_MESG_DBG(profiledbgfile, "gattProcedureOpcode\n");
    BLE_Profile_Reset_GATT_Procedure_Status ();
    //HCI_Enable_GATT_Cmd(gattProcedureOpcode); TODO: Add procedure commands in a queue?
  }
}

/**
* Function called by aci_gap_limited_discoverable_event of relative profile 
*/
void profile_gap_limited_discoverable_event(void)
{
  profileProcess = TRUE;
//  PROFILE_MESG_DBG(profiledbgfile, "Received event aci_gap_limited_discoverable_event \n");
}

/**
* Function called by aci_gatt_write_permit_req_event of relative profile 
*/
void profile_gatt_write_permit_req_event(uint16_t Connection_Handle,
                                         uint16_t Attribute_Handle,
                                         uint8_t Data_Length,
                                         uint8_t Data[])
{
  profileProcess = TRUE;
//  PROFILE_MESG_DBG ( profiledbgfile, "Received event aci_gatt_write_permit_req_event \n");
}      

/**
* Function called by aci_gatt_attribute_modified_event of relative profile 
*/
#if BLE_2_X /* BLE stack FW version >= 2.x*/
void profile_gatt_attribute_modified_event(uint16_t Connection_Handle,
                                           uint16_t Attr_Handle,
                                           uint16_t Offset,
                                           uint16_t Attr_Data_Length,
                                           uint8_t Attr_Data[])
#else
void profile_gatt_attribute_modified_event(uint16_t Connection_Handle,
                                           uint16_t Attr_Handle,
                                           uint16_t Offset,
                                           uint8_t Attr_Data_Length,
                                           uint8_t Attr_Data[])
#endif
{
  profileProcess = TRUE;
//  PROFILE_MESG_DBG(profiledbgfile, "Received event aci_gatt_attribute_modified_event \n");
}

/**
* Function called by hci_encryption_change_event of relative profile 
*/ 
void profile_hci_encryption_change_event(uint8_t Status,
                                         uint16_t Connection_Handle,
                                         uint8_t Encryption_Enabled)
{
  profileProcess = TRUE;
  PROFILE_MESG_DBG(profiledbgfile, "Received event hci_encryption_change_event \n");
}

/**
* Function called by hci_read_remote_version_information_complete_event of relative profile 
*/ 
void profile_hci_read_remote_version_information_complete_event(uint8_t Status,
                                                            uint16_t Connection_Handle,
                                                            uint8_t Version,
                                                            uint16_t Manufacturer_Name,
                                                            uint16_t Subversion)
{
  profileProcess = TRUE;
  PROFILE_MESG_DBG(profiledbgfile, "Received event hci_read_remote_version_information_complete_event \n");
}

/**
* Function called by hci_hardware_error_event of relative profile 
*/ 
void profile_hci_hardware_error_event(uint8_t Hardware_Code)
{
  profileProcess = TRUE;
  PROFILE_MESG_DBG ( profiledbgfile, "Received event hci_hardware_error_event \n");
}

/**
* Function called by hci_encryption_key_refresh_complete_event of relative profile 
*/
void profile_hci_encryption_key_refresh_complete_event(uint8_t Status,
                                                       uint16_t Connection_Handle)
{
  profileProcess = TRUE;
  PROFILE_MESG_DBG(profiledbgfile, "Received event hci_encryption_key_refresh_complete_event \n");
}

/**
* Function called by hci_le_connection_update_complete_event of relative profile 
*/
void profile_hci_le_connection_update_complete_event(uint8_t Status,
                                                     uint16_t Connection_Handle,
                                                     uint16_t Conn_Interval,
                                                     uint16_t Conn_Latency,
                                                     uint16_t Supervision_Timeout)
{
  profileProcess = TRUE;
}

/**
* Function called by aci_l2cap_connection_update_resp_event of relative profile 
*/
void profile_aci_l2cap_connection_update_resp_event(uint16_t Connection_Handle,
                                                    uint16_t Result)
{
  profileProcess = TRUE;
  PROFILE_MESG_DBG (profiledbgfile, "Received event aci_l2cap_connection_update_resp_event \n");
}

/**
* Function called by aci_l2cap_proc_timeout_event of relative profile 
*/
void profile_aci_l2cap_proc_timeout_event(uint16_t Connection_Handle,
                                          uint8_t Data_Length,
                                          uint8_t Data[])
{
  profileProcess = TRUE;
  PROFILE_MESG_DBG (profiledbgfile, "Received event aci_l2cap_proc_timeout_event \n");\
}  

///**
//* Function called by aci_gatt_attribute_modified_event of relative profile 
//*/
//void profile_aci_gatt_attribute_modified_event(uint16_t Connection_Handle,
//                                               uint16_t Attr_Handle,
//                                               uint16_t Offset,
//                                               uint16_t Attr_Data_Length,
//                                               uint8_t Attr_Data[])
//{
//  profileProcess = TRUE;
//  PROFILE_MESG_DBG(profiledbgfile, "Received event aci_gatt_attribute_modified_event \n");
//}

/**
* Function called by aci_att_exchange_mtu_resp_event of relative profile 
*/
void profile_aci_att_exchange_mtu_resp_event(uint16_t Connection_Handle,
                                             uint16_t Server_RX_MTU)
{
  profileProcess = TRUE;
  PROFILE_MESG_DBG ( profiledbgfile, "Received event aci_att_exchange_mtu_resp_event \n");
}
  
/**
* Function called by aci_att_find_info_resp_event of relative profile 
*/
void profile_aci_att_find_info_resp_event(uint16_t Connection_Handle,
                                          uint8_t Format,
                                          uint8_t Event_Data_Length,
                                          uint8_t Handle_UUID_Pair[])
{
  profileProcess = TRUE;
  PROFILE_MESG_DBG ( profiledbgfile, "Received event aci_att_find_info_resp_event \n");
}

/**
* Function called by aci_att_find_by_type_value_resp_event of relative profile 
*/
void profile_aci_att_find_by_type_value_resp_event(uint16_t Connection_Handle,
                                                   uint8_t Num_of_Handle_Pair,
                                                   Attribute_Group_Handle_Pair_t Attribute_Group_Handle_Pair[])
{
  profileProcess = TRUE;
  PROFILE_MESG_DBG ( profiledbgfile, "Received event aci_att_find_by_type_value_resp_event \n");
}

/**
* Function called by aci_att_read_by_type_resp_event of relative profile 
*/
void profile_aci_att_read_by_type_resp_event(uint16_t Connection_Handle,
                                             uint8_t Handle_Value_Pair_Length,
                                             uint8_t Data_Length,
                                             uint8_t Handle_Value_Pair_Data[])
{
  profileProcess = TRUE;
  PROFILE_MESG_DBG ( profiledbgfile, "Received event aci_att_read_by_type_resp_event \n");
}

/**
* Function called by aci_att_read_resp_event of relative profile 
*/
void profile_aci_att_read_resp_event(uint16_t Connection_Handle,
                                     uint8_t Event_Data_Length,
                                     uint8_t Attribute_Value[])
{
  profileProcess = TRUE;
  PROFILE_MESG_DBG ( profiledbgfile, "Received event aci_att_read_resp_event \n");
}

/**
* Function called by aci_att_read_blob_resp_event of relative profile 
*/
void profile_aci_att_read_blob_resp_event(uint16_t Connection_Handle,
                                          uint8_t Event_Data_Length,
                                          uint8_t Attribute_Value[])
{
  profileProcess = TRUE;
  PROFILE_MESG_DBG ( profiledbgfile, "Received event aci_att_read_blob_resp_event \n");
}

/**
* Function called by aci_att_read_multiple_resp_event of relative profile 
*/
void profile_aci_att_read_multiple_resp_event(uint16_t Connection_Handle,
                                              uint8_t Event_Data_Length,
                                              uint8_t Set_Of_Values[])
{
  profileProcess = TRUE;
  PROFILE_MESG_DBG ( profiledbgfile, "Received event aci_att_read_multiple_resp_event \n");
}

/**
* Function called by aci_att_read_by_group_type_resp_event of relative profile 
*/
void profile_aci_att_read_by_group_type_resp_event(uint16_t Connection_Handle,
                                                   uint8_t Attribute_Data_Length,
                                                   uint8_t Data_Length,
                                                   uint8_t Attribute_Data_List[])
{
  profileProcess = TRUE;
 // PROFILE_MESG_DBG ( profiledbgfile, "Received event aci_att_read_by_group_type_resp_event \n");
}

/**
* Function called by aci_att_prepare_write_resp_event of relative profile 
*/
void profile_aci_att_prepare_write_resp_event(uint16_t Connection_Handle,
                                              uint16_t Attribute_Handle,
                                              uint16_t Offset,
                                              uint8_t Part_Attribute_Value_Length,
                                              uint8_t Part_Attribute_Value[])
{
  profileProcess = TRUE;
  PROFILE_MESG_DBG ( profiledbgfile, "Received event aci_att_prepare_write_resp_event \n");
}

/**
* Function called by aci_att_exec_write_resp_event of relative profile 
*/
void profile_aci_att_exec_write_resp_event(uint16_t Connection_Handle)
{
  profileProcess = TRUE;
  PROFILE_MESG_DBG ( profiledbgfile, "Received event aci_att_exec_write_resp_event \n");
}

/**
* Function called by aci_gatt_indication_event of relative profile 
*/
void profile_aci_gatt_indication_event(uint16_t Connection_Handle,
                                       uint16_t Attribute_Handle,
                                       uint8_t Attribute_Value_Length,
                                       uint8_t Attribute_Value[])
{
  profileProcess = TRUE;
  PROFILE_MESG_DBG ( profiledbgfile, "Received event aci_gatt_indication_event \n");
}

/**
* Function called by aci_gatt_notification_event of relative profile 
*/
void profile_aci_gatt_notification_event(uint16_t Connection_Handle,
                                         uint16_t Attribute_Handle,
                                         uint8_t Attribute_Value_Length,
                                         uint8_t Attribute_Value[])
{
  profileProcess = TRUE;
  PROFILE_MESG_DBG ( profiledbgfile, "Received event aci_gatt_notification_event \n");
}

/**
* Function called by aci_gatt_error_resp_event of relative profile 
*/
void profile_aci_gatt_error_resp_event(uint16_t Connection_Handle,
                                       uint8_t Req_Opcode,
                                       uint16_t Attribute_Handle,
                                       uint8_t Error_Code)
{
  profileProcess = TRUE;
  PROFILE_MESG_DBG ( profiledbgfile, "Received event aci_gatt_error_resp_event (Error_Code: 0x%02x, Attr_Handle: 0x%04x) \n", Error_Code, Attribute_Handle);
}

/**
* Function called by aci_gatt_disc_read_char_by_uuid_resp_event of relative profile 
*/
void profile_aci_gatt_disc_read_char_by_uuid_resp_event(uint16_t Connection_Handle,
                                                        uint16_t Attribute_Handle,
                                                        uint8_t Attribute_Value_Length,
                                                        uint8_t Attribute_Value[])
{
  profileProcess = TRUE;
  PROFILE_MESG_DBG ( profiledbgfile, "Received event aci_gatt_disc_read_char_by_uuid_resp_event \n");
}

/**
* Function called by aci_gatt_write_permit_req_event of relative profile 
*/
void profile_aci_gatt_write_permit_req_event(uint16_t Connection_Handle,
                                             uint16_t Attribute_Handle,
                                             uint8_t Data_Length,
                                             uint8_t Data[])
{
  profileProcess = TRUE;
  PROFILE_MESG_DBG ( profiledbgfile, "Received event aci_gatt_write_permit_req_event \n");
}

/**
* Function called by aci_gatt_read_permit_req_event of relative profile 
*/
void profile_aci_gatt_read_permit_req_event(uint16_t Connection_Handle,
                                            uint16_t Attribute_Handle,
                                            uint16_t Offset)
{
  profileProcess = TRUE;
  PROFILE_MESG_DBG ( profiledbgfile, "Received event aci_gatt_read_permit_req_event \n");
}

/**
* Function called by aci_gatt_read_multi_permit_req_event of relative profile 
*/
void profile_aci_gatt_read_multi_permit_req_event(uint16_t Connection_Handle,
                                                  uint8_t Number_of_Handles,
                                                  Handle_Item_t Handle_Item[])
{
  profileProcess = TRUE;
  PROFILE_MESG_DBG ( profiledbgfile, "Received event aci_gatt_read_multi_permit_req_event \n");
}




/*******************************************************************************
* Local Functions
*******************************************************************************/
tBleStatus Add_DIS_Charac(void)
{
  tBleStatus ret;
  Char_UUID_t uuid;
  
  if(gMainProfileCtxt.devInfoService.deviceInfoCharToBeAdded & SYSTEM_ID_CHAR_MASK)
  {  
    HOST_TO_LE_16((uint8_t *) &uuid, SYSTEM_ID_UUID);
    
    ret = aci_gatt_add_char(gMainProfileCtxt.devInfoService.devInfoServHandle,
                            UUID_TYPE_16,
                            &uuid,
                            SYSTEM_ID_LEN_MAX,
                            CHAR_PROP_READ,  
                            ATTR_PERMISSION_NONE,
                            GATT_DONT_NOTIFY_EVENTS,
                            10,
                            CHAR_VALUE_LEN_CONSTANT,
                            &gMainProfileCtxt.devInfoService.systemIDCharHandle);
    if(ret != BLE_STATUS_SUCCESS)
      goto ADD_DIS_CHARAC_FAIL;
    PROFILE_MESG_DBG ( profiledbgfile, "[DIS] system ID Handle: %04X !!\n",
                      gMainProfileCtxt.devInfoService.systemIDCharHandle);
  }
  if(gMainProfileCtxt.devInfoService.deviceInfoCharToBeAdded & MODEL_NUMBER_STRING_CHAR_MASK)
  {
    HOST_TO_LE_16((uint8_t *) &uuid, MODEL_NUMBER_UUID);
    
    ret = aci_gatt_add_char(gMainProfileCtxt.devInfoService.devInfoServHandle,
                            UUID_TYPE_16,
                            &uuid,
                            MODEL_NUMBER_STRING_LEN_MAX,
                            CHAR_PROP_READ,  
                            ATTR_PERMISSION_NONE,
                            GATT_DONT_NOTIFY_EVENTS,
                            10,
                            CHAR_VALUE_LEN_VARIABLE,
                            &gMainProfileCtxt.devInfoService.modelNumCharHandle);
    if(ret != BLE_STATUS_SUCCESS)
      goto ADD_DIS_CHARAC_FAIL;
    
    PROFILE_MESG_DBG ( profiledbgfile, "[DIS] model number Handle: %04X !!\n",
                      gMainProfileCtxt.devInfoService.modelNumCharHandle);
    
  }
  if(gMainProfileCtxt.devInfoService.deviceInfoCharToBeAdded & SERIAL_NUMBER_STRING_CHAR_MASK)
  {
    HOST_TO_LE_16((uint8_t *) &uuid, SERIAL_NUMBER_UUID);
    
    ret = aci_gatt_add_char(gMainProfileCtxt.devInfoService.devInfoServHandle,
                            UUID_TYPE_16,
                            &uuid,
                            SERIAL_NUMBER_STRING_LEN_MAX,
                            CHAR_PROP_READ,  
                            ATTR_PERMISSION_NONE,
                            GATT_DONT_NOTIFY_EVENTS,
                            10,
                            CHAR_VALUE_LEN_VARIABLE,
                            &gMainProfileCtxt.devInfoService.serialNumCharHandle);
    if(ret != BLE_STATUS_SUCCESS)
      goto ADD_DIS_CHARAC_FAIL;
    
	PROFILE_MESG_DBG(profiledbgfile, "[DIS] serial number Handle: %04X !!\n",
                     gMainProfileCtxt.devInfoService.serialNumCharHandle);
  }
  if(gMainProfileCtxt.devInfoService.deviceInfoCharToBeAdded & FIRMWARE_REVISION_STRING_CHAR_MASK)
  {
    HOST_TO_LE_16((uint8_t *) &uuid, FIRMWARE_REVISION_UUID);
    
    ret = aci_gatt_add_char(gMainProfileCtxt.devInfoService.devInfoServHandle,
                            UUID_TYPE_16,
                            &uuid,
                            FIRMWARE_REVISION_STRING_LEN_MAX,
                            CHAR_PROP_READ,  
                            ATTR_PERMISSION_NONE,
                            GATT_DONT_NOTIFY_EVENTS,
                            10,
                            CHAR_VALUE_LEN_VARIABLE,
                            &gMainProfileCtxt.devInfoService.firmwareRevisionCharHandle);
    if(ret != BLE_STATUS_SUCCESS)
      goto ADD_DIS_CHARAC_FAIL;
    
	PROFILE_MESG_DBG(profiledbgfile, "[DIS] firmware revision Handle: %04X !!\n",
                     gMainProfileCtxt.devInfoService.firmwareRevisionCharHandle);
  }
  if(gMainProfileCtxt.devInfoService.deviceInfoCharToBeAdded & HARDWARE_REVISION_STRING_CHAR_MASK)
  {
    HOST_TO_LE_16((uint8_t *) &uuid, HARDWARE_REVISION_UUID);
    
    ret = aci_gatt_add_char(gMainProfileCtxt.devInfoService.devInfoServHandle,
                            UUID_TYPE_16,
                            &uuid,
                            HARDWARE_REVISION_STRING_LEN_MAX,
                            CHAR_PROP_READ,  
                            ATTR_PERMISSION_NONE,
                            GATT_DONT_NOTIFY_EVENTS,
                            10,
                            CHAR_VALUE_LEN_VARIABLE,
                            &gMainProfileCtxt.devInfoService.hardwareRevisionCharHandle);
    if(ret != BLE_STATUS_SUCCESS)
      goto ADD_DIS_CHARAC_FAIL;
    
	PROFILE_MESG_DBG(profiledbgfile, "[DIS] hardware revision Handle: %04X !!\n",
                     gMainProfileCtxt.devInfoService.hardwareRevisionCharHandle);
  }
  if(gMainProfileCtxt.devInfoService.deviceInfoCharToBeAdded & SOFTWARE_REVISION_STRING_CHAR_MASK)
  {
    HOST_TO_LE_16((uint8_t *) &uuid, SOFTWARE_REVISION_UUID);
    
    ret = aci_gatt_add_char(gMainProfileCtxt.devInfoService.devInfoServHandle,
                            UUID_TYPE_16,
                            &uuid,
                            SOFTWARE_REVISION_STRING_LEN_MAX,
                            CHAR_PROP_READ,  
                            ATTR_PERMISSION_NONE,
                            GATT_DONT_NOTIFY_EVENTS,
                            10,
                            CHAR_VALUE_LEN_VARIABLE,
                            &gMainProfileCtxt.devInfoService.softwareRevisionCharHandle);
    if(ret != BLE_STATUS_SUCCESS)
      goto ADD_DIS_CHARAC_FAIL;
    
	PROFILE_MESG_DBG(profiledbgfile, "[DIS] software revision Handle: %04X !!\n",
                     gMainProfileCtxt.devInfoService.softwareRevisionCharHandle);
  }
  if(gMainProfileCtxt.devInfoService.deviceInfoCharToBeAdded & MANUFACTURER_NAME_STRING_CHAR_MASK)
  {
    HOST_TO_LE_16((uint8_t *) &uuid, MANUFACTURER_NAME_UUID);
    
    ret = aci_gatt_add_char(gMainProfileCtxt.devInfoService.devInfoServHandle,
                            UUID_TYPE_16,
                            &uuid,
                            MANUFACTURER_NAME_STRING_LEN_MAX,
                            CHAR_PROP_READ,  
                            ATTR_PERMISSION_NONE,
                            GATT_DONT_NOTIFY_EVENTS,
                            10,
                            CHAR_VALUE_LEN_VARIABLE,
                            &gMainProfileCtxt.devInfoService.manufacturerNameCharHandle);
    if(ret != BLE_STATUS_SUCCESS)
      goto ADD_DIS_CHARAC_FAIL;
    
	PROFILE_MESG_DBG(profiledbgfile, "[DIS] manufacturer name Handle: %04X !!\n",
                     gMainProfileCtxt.devInfoService.manufacturerNameCharHandle);
  }
  if(gMainProfileCtxt.devInfoService.deviceInfoCharToBeAdded & IEEE_CERTIFICATION_CHAR_MASK)
  {
    HOST_TO_LE_16((uint8_t *) &uuid, IEEE_CERTIFICATION_UUID);
    
    ret = aci_gatt_add_char(gMainProfileCtxt.devInfoService.devInfoServHandle,
                            UUID_TYPE_16,
                            &uuid,
                            IEEE_CERTIFICATION_LEN_MAX,
                            CHAR_PROP_READ,  
                            ATTR_PERMISSION_NONE,
                            GATT_DONT_NOTIFY_EVENTS,
                            10,
                            CHAR_VALUE_LEN_VARIABLE,
                            &gMainProfileCtxt.devInfoService.ieeeCretificationCharHandle);
    if(ret != BLE_STATUS_SUCCESS)
      goto ADD_DIS_CHARAC_FAIL;
    
	PROFILE_MESG_DBG(profiledbgfile, "[DIS] IEE certification Handle: %04X !!\n",
                     gMainProfileCtxt.devInfoService.ieeeCretificationCharHandle);
  }
  if(gMainProfileCtxt.devInfoService.deviceInfoCharToBeAdded & PNP_ID_CHAR_MASK)
  {
    HOST_TO_LE_16((uint8_t *) &uuid, PNP_ID_UUID);
    
    ret = aci_gatt_add_char(gMainProfileCtxt.devInfoService.devInfoServHandle,
                            UUID_TYPE_16,
                            &uuid,
                            PNP_ID_LEN_MAX,
                            CHAR_PROP_READ,  
                            ATTR_PERMISSION_NONE,
                            GATT_DONT_NOTIFY_EVENTS,
                            10,
                            CHAR_VALUE_LEN_CONSTANT,
                            & gMainProfileCtxt.devInfoService.pnpIDCharHandle);
    if(ret != BLE_STATUS_SUCCESS)
      goto ADD_DIS_CHARAC_FAIL;
    
	PROFILE_MESG_DBG(profiledbgfile, "[DIS] PNP ID Handle: %04X !!\n",
                     gMainProfileCtxt.devInfoService.pnpIDCharHandle);
  }
  
  return BLE_STATUS_SUCCESS;
  
ADD_DIS_CHARAC_FAIL:
  PROFILE_MESG_ERR(profiledbgfile, "[DIS] Error while adding characteristics.\n");
  return ret;
}

tBleStatus BLE_Profile_Update_DIS_SystemID(uint8_t len,uint8_t *SystemId)
{
  tBleStatus ret = BLE_STATUS_INVALID_PARAMS;
  
  PROFILE_MESG_DBG(profiledbgfile,"BLE_Profile_Update_DIS_SystemID %d\n",len);
  if(len <= SYSTEM_ID_LEN_MAX)
  {	
    ret = aci_gatt_update_char_value(gMainProfileCtxt.devInfoService.devInfoServHandle, 
                                     gMainProfileCtxt.devInfoService.systemIDCharHandle,
                                     0,
                                     len,
                                     SystemId);
    
    if(ret == BLE_STATUS_SUCCESS)
    {
      gMainProfileCtxt.devInfoService.deviceInfoCharToBeAdded  |= SYSTEM_ID_CHAR_MASK;
    }
  }
  return ret;
}

tBleStatus BLE_Profile_Update_DIS_ModelNum(uint8_t len,uint8_t *modelNum)
{
  tBleStatus ret = BLE_STATUS_INVALID_PARAMS;
  
  PROFILE_MESG_DBG(profiledbgfile,"BLE_Profile_Update_DIS_ModelNum %d\n",len);
  if(len <= MODEL_NUMBER_STRING_LEN_MAX)
  {	
    ret = aci_gatt_update_char_value(gMainProfileCtxt.devInfoService.devInfoServHandle, 
                                     gMainProfileCtxt.devInfoService.modelNumCharHandle, 
                                     0,
                                     len, 
                                     modelNum);
    if(ret == BLE_STATUS_SUCCESS)
    {
      gMainProfileCtxt.devInfoService.deviceInfoCharToBeAdded  |= MODEL_NUMBER_STRING_CHAR_MASK;
    }
  }
  return ret;
}

tBleStatus BLE_Profile_Update_DIS_SerialNum(uint8_t len,uint8_t *serialNum)
{
  tBleStatus ret = BLE_STATUS_INVALID_PARAMS;
  
  PROFILE_MESG_DBG(profiledbgfile,"BLE_Profile_Update_DIS_SerialNum %d\n",len);
  if(len <= SERIAL_NUMBER_STRING_LEN_MAX)
  {	
    ret = aci_gatt_update_char_value(gMainProfileCtxt.devInfoService.devInfoServHandle, 
                                     gMainProfileCtxt.devInfoService.serialNumCharHandle, 
                                     0,
                                     len, 
                                     serialNum);
    if(ret == BLE_STATUS_SUCCESS)
    {
      gMainProfileCtxt.devInfoService.deviceInfoCharToBeAdded  |= SERIAL_NUMBER_STRING_CHAR_MASK;
    }
  }
  return ret;
}

tBleStatus BLE_Profile_Update_DIS_FirmwareRev(uint8_t len,uint8_t *firmwareRev)
{
  tBleStatus ret = BLE_STATUS_INVALID_PARAMS;
  
  PROFILE_MESG_DBG(profiledbgfile,"BLE_Profile_Update_DIS_FirmwareRev %d\n",len);
  if(len <= FIRMWARE_REVISION_STRING_LEN_MAX)
  {	
    ret = aci_gatt_update_char_value(gMainProfileCtxt.devInfoService.devInfoServHandle, 
                                     gMainProfileCtxt.devInfoService.firmwareRevisionCharHandle, 
                                     0,
                                     len, 
                                     firmwareRev);
    if(ret == BLE_STATUS_SUCCESS)
    {
      gMainProfileCtxt.devInfoService.deviceInfoCharToBeAdded  |= FIRMWARE_REVISION_STRING_CHAR_MASK;
    }
  }
  return ret;
}

tBleStatus BLE_Profile_Update_DIS_HardwareRev(uint8_t len,uint8_t *hardwareRev)
{
  tBleStatus ret = BLE_STATUS_INVALID_PARAMS;
  
  PROFILE_MESG_DBG(profiledbgfile,"BLE_Profile_Update_DIS_HardwareRev %d\n",len);
  if(len <= HARDWARE_REVISION_STRING_LEN_MAX)
  {	
    ret = aci_gatt_update_char_value(gMainProfileCtxt.devInfoService.devInfoServHandle, 
                                     gMainProfileCtxt.devInfoService.hardwareRevisionCharHandle, 
                                     0,
                                     len, 
                                     hardwareRev);
    if(ret == BLE_STATUS_SUCCESS)
    {
      gMainProfileCtxt.devInfoService.deviceInfoCharToBeAdded  |= HARDWARE_REVISION_STRING_CHAR_MASK;
    }
  }
  return ret;
}

tBleStatus BLE_Profile_Update_DIS_SoftwareRev(uint8_t len,uint8_t *softwareRev)
{
  tBleStatus ret = BLE_STATUS_INVALID_PARAMS;
  
  PROFILE_MESG_DBG(profiledbgfile,"BLE_Profile_Update_DIS_SoftwareRev %d\n",len);
  if(len <= SOFTWARE_REVISION_STRING_LEN_MAX)
  {	
    ret = aci_gatt_update_char_value(gMainProfileCtxt.devInfoService.devInfoServHandle, 
                                     gMainProfileCtxt.devInfoService.softwareRevisionCharHandle, 
                                     0,
                                     len, 
                                     softwareRev);
    if(ret == BLE_STATUS_SUCCESS)
    {
      gMainProfileCtxt.devInfoService.deviceInfoCharToBeAdded  |= SOFTWARE_REVISION_STRING_CHAR_MASK;
    }
  }
  return ret;
}

tBleStatus BLE_Profile_Update_DIS_manufacturerName(uint8_t len,uint8_t *name)
{
  tBleStatus ret = BLE_STATUS_INVALID_PARAMS;
  
  PROFILE_MESG_DBG(profiledbgfile,"BLE_Profile_Update_DIS_manufacturerName %d\n",len);
  if(len <= MANUFACTURER_NAME_STRING_LEN_MAX)
  {	
    ret = aci_gatt_update_char_value(gMainProfileCtxt.devInfoService.devInfoServHandle, 
                                     gMainProfileCtxt.devInfoService.manufacturerNameCharHandle, 
                                     0,
                                     len, 
                                     name);
    if(ret == BLE_STATUS_SUCCESS)
    {
      gMainProfileCtxt.devInfoService.deviceInfoCharToBeAdded  |= MANUFACTURER_NAME_STRING_CHAR_MASK;
    }
  }
  return ret;
}

tBleStatus BLE_Profile_Update_DIS_IEEECertification(uint8_t len,uint8_t *ieeeCert)
{
  tBleStatus ret = BLE_STATUS_INVALID_PARAMS;
  
  PROFILE_MESG_DBG(profiledbgfile,"BLE_Profile_Update_DIS_manufacturerName %d\n",len);
  if(len <= IEEE_CERTIFICATION_LEN_MAX)
  {	
    ret = aci_gatt_update_char_value(gMainProfileCtxt.devInfoService.devInfoServHandle, 
                                     gMainProfileCtxt.devInfoService.ieeeCretificationCharHandle, 
                                     0,
                                     len, 
                                     ieeeCert);
    if(ret == BLE_STATUS_SUCCESS)
    {
      gMainProfileCtxt.devInfoService.deviceInfoCharToBeAdded  |= IEEE_CERTIFICATION_CHAR_MASK;
    }
  }
  return ret;
}

tBleStatus BLE_Profile_Update_DIS_pnpId(uint8_t len,uint8_t *pnpId)
{
  tBleStatus ret = BLE_STATUS_INVALID_PARAMS;
  
  PROFILE_MESG_DBG(profiledbgfile,"BLE_Profile_Update_DIS_pnpId %d\n",len);
  if(len <= PNP_ID_LEN_MAX)
  {	
    ret = aci_gatt_update_char_value(gMainProfileCtxt.devInfoService.devInfoServHandle, 
                                     gMainProfileCtxt.devInfoService.pnpIDCharHandle, 
                                     0,
                                     len, 
                                     pnpId);
    if(ret == BLE_STATUS_SUCCESS)
    {
      gMainProfileCtxt.devInfoService.deviceInfoCharToBeAdded  |= PNP_ID_CHAR_MASK;
    }
  }
  return ret;
}

/**
* BLE_Profile_GATT_Procedure_Complete_EvtHandler
* 
* @param[in] procedureCode : opcode of the gatt procedure that
*           has been completed
*/ 
tBleStatus BLE_Profile_GATT_Procedure_Complete_EvtHandler(uint16_t procedureCode)
{
  PROFILE_MESG_DBG ( profiledbgfile, "BLE_Profile_GATT_Procedure_Complete_EvtHandler: opcode %04X \n", procedureCode);
  switch (gMainProfileCtxt.bleProfileMainState)
  {
  case BLE_PROFILE_STATE_CONNECTED:
    {
      switch (gMainProfileCtxt.bleProfileSubState)
      {
      case BLE_PROFILE_STATE_EXCHANGING_CONFIG:
        {
         // TBR???? 
//          if (procedureCode == OCF_GATT_EXCHANGE_CONFIG)
//          {
//            gMainProfileCtxt.bleProfileSubState = BLE_PROFILE_STATE_CONNECTED_IDLE;
//          }
        }
        break;
      }
    }
    break;
  }
  return (BLE_STATUS_SUCCESS);
}

/**
* BLE_Profile_Is_Registered_Profile
* 
* @param[in] profileContext : pointer to the profile interface context
*            which has to be checked for its registration with the main profile
* 
* searches through the list of registered profiles to find the requested
* profile
* 
* @return returns BLE_STATUS_SUCCESS if the desired profile is found else
* returns BLE_STATUS_FAILED
*/ 
tBleStatus BLE_Profile_Is_Registered_Profile(tProfileInterfaceContext *profileContext)
{
  tListNode * node = NULL;
  
  list_get_next_node (&gMainProfileCtxt.bleRegstedProfileList, &node);
  while (node != &gMainProfileCtxt.bleRegstedProfileList)
  {
    if (node == (tListNode *)profileContext)
    {
      /* profile is a registered one */
      return BLE_STATUS_SUCCESS;
    }
    
    /* get the next node "registered profile context" in the list */
    list_get_next_node (node, &node);
  }
  
  return BLE_STATUS_FAILED;
}

static uint16_t runningGATTProcedureOpcode = 0 ;

/*******************************************************************************
* Functions to handle ATT sequential protocol
*******************************************************************************/

void BLE_Profile_Set_GATT_Procedure_Status(uint16_t opcode)
{
  runningGATTProcedureOpcode = opcode;
}

void BLE_Profile_Reset_GATT_Procedure_Status(void)
{
  runningGATTProcedureOpcode = 0;
}

uint16_t BLE_Profile_Get_Current_GATT_Procedure_Opcode ( void )
{
  return (runningGATTProcedureOpcode);
}

/* New function added for directly calling the aci_gap_slave_security_request()
   from GAP peripheral device. It used for some specific PTS tests. */
void  BLE_Profile_Send_Security_Request() 
{ 
  tBleStatus cmdResult = BLE_STATUS_INVALID_PARAMS;
  
  PROFILE_MESG_DBG ( profiledbgfile, "BLE_Profile_Send_Security_Request(): state %02X, substate %02X\n", gMainProfileCtxt.bleProfileMainState,gMainProfileCtxt.bleProfileSubState);
  
  PROFILE_MESG_DBG ( profiledbgfile, "BLE_Profile_Send_Security_Request(): send slave security request\n");
#if BLE_2_X /* BLE stack FW version >= 2.x*/   
  cmdResult = aci_gap_slave_security_req(gBLEProfileContext.connectionHandle);
#else
  cmdResult = aci_gap_slave_security_req(gBLEProfileContext.connectionHandle,
                                         gBLEProfileContext.bleSecurityParam.bonding_mode,
                                         gBLEProfileContext.bleSecurityParam.mitm_mode);
#endif
  if (cmdResult == BLE_STATUS_SUCCESS){
    PROFILE_MESG_DBG ( profiledbgfile, "BLE_Profile_Send_Security_Request(): Slave Security Request has been sent to master \n");
    gMainProfileCtxt.bleProfileSubState = BLE_PROFILE_STATE_SLAVE_SECURITY_REQUESTED;
  }
  else {
    /* security could not be requested */
    PROFILE_MESG_DBG ( profiledbgfile, "BLE_Profile_Send_Security_Request: aci_gap_slave_security_request() failure %02X  \n", cmdResult);
    gMainProfileCtxt.bleProfileMainState = BLE_PROFILE_STATE_CONNECTED;
  }
  
}/* end BLE_Profile_Send_Security_Request() */

/* New function to set Peripheral public address */
void BLE_Set_Peripheral_Public_Address(uint8_t *bdaddr)
{
  tBleStatus cmdResult = BLE_STATUS_INVALID_PARAMS;
  cmdResult = aci_hal_write_config_data(CONFIG_DATA_PUBADDR_OFFSET, CONFIG_DATA_PUBADDR_LEN,
                                        bdaddr);
  if(cmdResult)
  {
     PROFILE_MESG_DBG ( profiledbgfile, "BLE_Set_Peripheral_Public_Address: aci_hal_write_config_data() failure %02X  \n", cmdResult);
  }
}

/* Set profileProcess and state of ble  */
void setProfileProcess()
{
  profileProcess = TRUE;
  if(gMainProfileCtxt.bleProfileMainState == BLE_PROFILE_STATE_INIT_BLE)
  {
    gMainProfileCtxt.bleProfileSubState = BLE_PROFILE_STATE_INITIALIZING_GATT;
  }
}  
