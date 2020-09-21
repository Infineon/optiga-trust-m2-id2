/**
* \copyright
* MIT License
*
* Copyright (c) 2020 Infineon Technologies AG
*
* Permission is hereby granted, free of charge, to any person obtaining a copy
* of this software and associated documentation files (the "Software"), to deal
* in the Software without restriction, including without limitation the rights
* to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
* copies of the Software, and to permit persons to whom the Software is
* furnished to do so, subject to the following conditions:
*
* The above copyright notice and this permission notice shall be included in all
* copies or substantial portions of the Software.
*
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
* AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
* LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
* OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
* SOFTWARE
*
* \endcopyright
*
* \author Infineon Technologies AG
*
* \file example_optiga_hibernate_restore.c
*
* \brief   This file provides an example for hibernate and restore functionalities
*
* \ingroup grUseCases
*
* @{
*/

#include "optiga/optiga_util.h"
#include "optiga/optiga_crypt.h"
#include "optiga/pal/pal_os_timer.h"
#include "optiga_example.h"
#ifdef OPTIGA_COMMS_SHIELDED_CONNECTION 
//lint --e{526} suppress "the function is defined in example_pair_host_and_optiga_using_pre_shared_secret source file"
extern optiga_lib_status_t pair_host_and_optiga_using_pre_shared_secret(void);
#endif

/**
 * Callback when optiga_util_xxxx operation is completed asynchronously
 */
static volatile optiga_lib_status_t optiga_lib_status;
//lint --e{818} suppress "argument "context" is not used in the sample provided"
static void optiga_lib_callback(void * context, optiga_lib_status_t return_status)
{
    optiga_lib_status = return_status;
    if (NULL != context)
    {
        // callback to upper layer here
    }
}

static const uint8_t label [] = "Firmware update";

static const uint8_t random_seed [] = {
    0x61, 0xC7, 0xDE, 0xF9, 0x0F, 0xD5, 0xCD, 0x7A,
    0x8B, 0x7A, 0x36, 0x41, 0x04, 0xE0, 0x0D, 0x82,
    0x38, 0x46, 0xBF, 0xB7, 0x70, 0xEE, 0xBF, 0x8F,
    0x40, 0x25, 0x2E, 0x0A, 0x21, 0x42, 0xAF, 0x9C,
};
/**
 * The below example demonstrates hibernate and restore functionalities
 *
 * Example for #optiga_util_open_application and #optiga_util_close_application
 *
 */
void example_optiga_util_hibernate_restore(void)
{
    optiga_util_t * me_util = NULL;
    optiga_crypt_t * me_crypt = NULL; 
    uint32_t time_taken = 0;
    uint16_t bytes_to_read = 1;
    optiga_lib_status_t return_status = !OPTIGA_LIB_SUCCESS;
    uint8_t security_event_counter = 0;
    const uint8_t optional_data[2] = {0x01, 0x02};
    uint16_t optional_data_length = sizeof(optional_data);
    uint8_t decryption_key [16] = {0};
    uint8_t decryption_key_cmp [16] = {0};

    OPTIGA_EXAMPLE_LOG_MESSAGE("Begin demonstrating hibernate feature...\n");    
    OPTIGA_EXAMPLE_LOG_MESSAGE(__FUNCTION__);

    do
    {
        //Create an instance of optiga_util and optiga_crypt
        me_util = optiga_util_create(0, optiga_lib_callback, NULL);
        if (NULL == me_util)
        {
            break;
        }

        me_crypt = optiga_crypt_create(0, optiga_lib_callback, NULL);
        if (NULL == me_crypt)
        {
            break;
        }
        
        /**
         * 1. Open the application on OPTIGA which is a pre-condition to perform any other operations
         *    using optiga_util_open_application
         */
        optiga_lib_status = OPTIGA_LIB_BUSY;
                
        START_PERFORMANCE_MEASUREMENT(time_taken);
        
        return_status = optiga_util_open_application(me_util, 0);

        WAIT_AND_CHECK_STATUS(return_status, optiga_lib_status);
#ifdef OPTIGA_COMMS_SHIELDED_CONNECTION 
        /**
         * 2. Pairing the Host and OPTIGA using a pre-shared secret
         */
        return_status = pair_host_and_optiga_using_pre_shared_secret();
        if(OPTIGA_LIB_SUCCESS != return_status)
        {
            //pairing of host and optiga failed
            break;
        }
        
#endif        

        /**
         * 3. Generate 48 byte RSA Pre master secret in acquired session OID
         */
        optiga_lib_status = OPTIGA_LIB_BUSY;
        return_status = optiga_crypt_rsa_generate_pre_master_secret(me_crypt,
                                                                    optional_data,
                                                                    optional_data_length,
                                                                    48);

        WAIT_AND_CHECK_STATUS(return_status, optiga_lib_status);
        /**
         * 4. Derive key (e.g. decryption key) using optiga_crypt_tls_prf_sha256 with protected I2C communication.
         *       - Use shared secret from session OID data object
         */
        optiga_lib_status = OPTIGA_LIB_BUSY;

        OPTIGA_CRYPT_SET_COMMS_PROTECTION_LEVEL(me_crypt, OPTIGA_COMMS_COMMAND_PROTECTION);
        OPTIGA_CRYPT_SET_COMMS_PROTOCOL_VERSION(me_crypt, OPTIGA_COMMS_PROTOCOL_VERSION_PRE_SHARED_SECRET);
        return_status = optiga_crypt_tls_prf_sha256(me_crypt,
                                                    (uint16_t)OPTIGA_KEY_ID_SESSION_BASED, /* Input secret OID */
                                                    label,
                                                    sizeof(label),
                                                    random_seed,
                                                    sizeof(random_seed),
                                                    sizeof(decryption_key),
                                                    TRUE,
                                                    decryption_key);

        WAIT_AND_CHECK_STATUS(return_status, optiga_lib_status);
        /**
         * - The subsequent steps will hibernate OPTIGA and save the session and optiga comms related information in OPTIGA .
         * - The session and optiga comms related information are then restored back and crypto operation are performed using these information.
         */

        /**
         * 5. To perform the hibernate, Security Event Counter(SEC) must be 0.
         *    Read SEC data object (0xE0C5) and wait until SEC = 0
         */
        do
        {
            optiga_lib_status = OPTIGA_LIB_BUSY;
            return_status = optiga_util_read_data(me_util,
                                                  0xE0C5,
                                                  0x0000,
                                                  &security_event_counter,
                                                  &bytes_to_read);

            WAIT_AND_CHECK_STATUS(return_status, optiga_lib_status);
            pal_os_timer_delay_in_milliseconds(1000);
        } while (0 != security_event_counter);

        /**
         * 6. Hibernate the application on OPTIGA
         *    using optiga_util_close_application with perform_hibernate parameter as true
         */
        optiga_lib_status = OPTIGA_LIB_BUSY;
        return_status = optiga_util_close_application(me_util, 1);

        WAIT_AND_CHECK_STATUS(return_status, optiga_lib_status);

        /**
         * 7. Restore the application on OPTIGA
         *    using optiga_util_open_application with perform_restore parameter as true
         */
        optiga_lib_status = OPTIGA_LIB_BUSY;
        return_status = optiga_util_open_application(me_util, 1);

        WAIT_AND_CHECK_STATUS(return_status, optiga_lib_status);

        /**
         * 8. Derive key (e.g. decryption key) using optiga_crypt_tls_prf_sha256 with protected I2C communication.
         *       - Use shared secret from session OID data object
         */
        optiga_lib_status = OPTIGA_LIB_BUSY;

        OPTIGA_CRYPT_SET_COMMS_PROTECTION_LEVEL(me_crypt, OPTIGA_COMMS_COMMAND_PROTECTION);
        OPTIGA_CRYPT_SET_COMMS_PROTOCOL_VERSION(me_crypt, OPTIGA_COMMS_PROTOCOL_VERSION_PRE_SHARED_SECRET);
        return_status = optiga_crypt_tls_prf_sha256(me_crypt,
                                                    (uint16_t)OPTIGA_KEY_ID_SESSION_BASED, /* Input secret OID */
                                                    label,
                                                    sizeof(label),
                                                    random_seed,
                                                    sizeof(random_seed),
                                                    sizeof(decryption_key_cmp),
                                                    TRUE,
                                                    decryption_key_cmp);

        WAIT_AND_CHECK_STATUS(return_status, optiga_lib_status);
                                                    
        /**
         * 9. Compare the output of the Derive key to validate the secret was stored while hibernate option.
         *       - Use shared secret from session OID data object
         */
        return_status = (optiga_lib_status_t)memcmp(decryption_key_cmp,decryption_key,sizeof(decryption_key_cmp));
        if (0 != return_status)
        {
            break;
        }
        /**
         * 10. Close the application on OPTIGA without hibernating
         *    using optiga_util_close_application
         */
        optiga_lib_status = OPTIGA_LIB_BUSY;
        return_status = optiga_util_close_application(me_util, 0);
        WAIT_AND_CHECK_STATUS(return_status, optiga_lib_status);
        
        READ_PERFORMANCE_MEASUREMENT(time_taken);
        
        return_status = OPTIGA_LIB_SUCCESS;
        OPTIGA_EXAMPLE_LOG_MESSAGE("Hibernate feature demonstration completed...\n");
    } while (FALSE);
    OPTIGA_EXAMPLE_LOG_STATUS(return_status);

    if (me_util)
    {
        //Destroy the instance after the completion of usecase if not required.
        return_status = optiga_util_destroy(me_util);
        if(OPTIGA_LIB_SUCCESS != return_status)
        {
            //lint --e{774} suppress This is a generic macro
            OPTIGA_EXAMPLE_LOG_STATUS(return_status);
        }
    }

    if (me_crypt)
    {
        //Destroy the instance after the completion of usecase if not required.
        return_status = optiga_crypt_destroy(me_crypt);
        if(OPTIGA_LIB_SUCCESS != return_status)
        {
            //lint --e{774} suppress This is a generic macro
            OPTIGA_EXAMPLE_LOG_STATUS(return_status);
        }
    }
    OPTIGA_EXAMPLE_LOG_PERFORMANCE_VALUE(time_taken, return_status);
}

/**
 * @}
 */
