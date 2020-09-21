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
* \file example_optiga_util_protected_update.c
*
* \brief   This file provides the example for util protected update operation using #optiga_util_protected_update_start,
*           #optiga_util_protected_update_continue & #optiga_util_protected_update_final.
*
* \ingroup grOptigaExamples
*
* @{
*/

#include "optiga/optiga_util.h"
#include "optiga_example.h"
#include "protected_update_data_set/example_optiga_util_protected_update.h"

#ifndef OPTIGA_INIT_DEINIT_DONE_EXCLUSIVELY
extern void example_optiga_init(void);
extern void example_optiga_deinit(void);
#endif

/**
 * Callback when optiga_util_xxxx operation is completed asynchronously
 */
static volatile optiga_lib_status_t optiga_lib_status;
//lint --e{818} suppress "argument "context" is not used in the sample provided"
static void optiga_util_callback(void * context, optiga_lib_status_t return_status)
{
    optiga_lib_status = return_status;
    if (NULL != context)
    {
        // callback to upper layer here
    }
}

/**
 * RSA 1024 Trust Anchor
 */
const uint8_t trust_anchor [] = 
{
    0x30, 0x82, 0x02, 0xD5, 0x30, 0x82, 0x01, 0xBD, 0xA0, 0x03, 0x02, 0x01, 0x02, 0x02, 0x01, 0x4D,
    0x30, 0x0D, 0x06, 0x09, 0x2A, 0x86, 0x48, 0x86, 0xF7, 0x0D, 0x01, 0x01, 0x0B, 0x05, 0x00, 0x30,
    0x3C, 0x31, 0x0B, 0x30, 0x09, 0x06, 0x03, 0x55, 0x04, 0x06, 0x13, 0x02, 0x49, 0x4E, 0x31, 0x0D,
    0x30, 0x0B, 0x06, 0x03, 0x55, 0x04, 0x0B, 0x0C, 0x04, 0x49, 0x46, 0x49, 0x4E, 0x31, 0x1E, 0x30,
    0x1C, 0x06, 0x03, 0x55, 0x04, 0x03, 0x0C, 0x15, 0x74, 0x72, 0x75, 0x73, 0x74, 0x20, 0x61, 0x6E,
    0x63, 0x68, 0x6F, 0x72, 0x20, 0x72, 0x73, 0x61, 0x20, 0x32, 0x30, 0x34, 0x38, 0x30, 0x1E, 0x17,
    0x0D, 0x31, 0x38, 0x30, 0x36, 0x31, 0x31, 0x30, 0x37, 0x32, 0x35, 0x35, 0x36, 0x5A, 0x17, 0x0D, 
    0x32, 0x38, 0x30, 0x36, 0x30, 0x38, 0x30, 0x37, 0x32, 0x35, 0x35, 0x36, 0x5A, 0x30, 0x3C, 0x31, 
    0x0B, 0x30, 0x09, 0x06, 0x03, 0x55, 0x04, 0x06, 0x13, 0x02, 0x49, 0x4E, 0x31, 0x0D, 0x30, 0x0B, 
    0x06, 0x03, 0x55, 0x04, 0x0B, 0x0C, 0x04, 0x49, 0x46, 0x49, 0x4E, 0x31, 0x1E, 0x30, 0x1C, 0x06, 
    0x03, 0x55, 0x04, 0x03, 0x0C, 0x15, 0x69, 0x6E, 0x74, 0x65, 0x72, 0x6D, 0x65, 0x64, 0x69, 0x61, 
    0x74, 0x65, 0x20, 0x72, 0x73, 0x61, 0x20, 0x31, 0x30, 0x32, 0x34, 0x30, 0x81, 0x9F, 0x30, 0x0D, 
    0x06, 0x09, 0x2A, 0x86, 0x48, 0x86, 0xF7, 0x0D, 0x01, 0x01, 0x01, 0x05, 0x00, 0x03, 0x81, 0x8D, 
    0x00, 0x30, 0x81, 0x89, 0x02, 0x81, 0x81, 0x00, 0xE1, 0x26, 0x9F, 0x49, 0x2B, 0xA0, 0xCF, 0x88, 
    0xAB, 0x10, 0x6A, 0x48, 0x64, 0xAD, 0xBF, 0xED, 0x95, 0xAE, 0x49, 0xE6, 0x85, 0x78, 0x56, 0x5F, 
    0x44, 0x0A, 0xD5, 0x3E, 0x76, 0x16, 0x70, 0xD1, 0x15, 0x04, 0x04, 0x15, 0x3A, 0xD3, 0xBE, 0xF0, 
    0xA9, 0x68, 0x1B, 0x0A, 0x8F, 0xDC, 0xE4, 0x09, 0x5F, 0xF7, 0x27, 0xED, 0x68, 0x22, 0x54, 0xCA, 
    0xB5, 0xB2, 0x7A, 0xD0, 0xBD, 0x60, 0x97, 0x5A, 0x17, 0x3B, 0x6B, 0x9D, 0xF7, 0x12, 0xD5, 0x01, 
    0x9C, 0x7C, 0x89, 0x4F, 0xE2, 0x28, 0x44, 0xC8, 0x7E, 0xC4, 0x49, 0xFB, 0xDF, 0x31, 0xBF, 0x32, 
    0xDC, 0xC6, 0x2B, 0x32, 0xDC, 0x73, 0x41, 0xB2, 0x92, 0xE9, 0x90, 0xB5, 0x93, 0xFF, 0x60, 0x10, 
    0xC3, 0x45, 0xF3, 0xE6, 0x07, 0x04, 0xF0, 0xA2, 0x67, 0x2F, 0x5F, 0x53, 0xA5, 0xFC, 0x1B, 0x30, 
    0xE5, 0x7E, 0x8E, 0x94, 0x85, 0x13, 0x13, 0x9B, 0x02, 0x03, 0x01, 0x00, 0x01, 0xA3, 0x66, 0x30, 
    0x64, 0x30, 0x1D, 0x06, 0x03, 0x55, 0x1D, 0x0E, 0x04, 0x16, 0x04, 0x14, 0x82, 0xDA, 0x0A, 0xF9, 
    0x6A, 0x37, 0x49, 0x71, 0xA1, 0x9E, 0x61, 0x4E, 0x46, 0xEB, 0xA9, 0x19, 0x9F, 0x59, 0xAB, 0x28, 
    0x30, 0x1F, 0x06, 0x03, 0x55, 0x1D, 0x23, 0x04, 0x18, 0x30, 0x16, 0x80, 0x14, 0xDF, 0xE4, 0xB1, 
    0x34, 0x53, 0x2D, 0x46, 0x50, 0xCD, 0x7D, 0xFB, 0xF8, 0x68, 0x1F, 0xCB, 0xF4, 0x29, 0xA5, 0x4C, 
    0xD1, 0x30, 0x12, 0x06, 0x03, 0x55, 0x1D, 0x13, 0x01, 0x01, 0xFF, 0x04, 0x08, 0x30, 0x06, 0x01, 
    0x01, 0xFF, 0x02, 0x01, 0x00, 0x30, 0x0E, 0x06, 0x03, 0x55, 0x1D, 0x0F, 0x01, 0x01, 0xFF, 0x04, 
    0x04, 0x03, 0x02, 0x01, 0x86, 0x30, 0x0D, 0x06, 0x09, 0x2A, 0x86, 0x48, 0x86, 0xF7, 0x0D, 0x01, 
    0x01, 0x0B, 0x05, 0x00, 0x03, 0x82, 0x01, 0x01, 0x00, 0x21, 0x4F, 0x0B, 0x00, 0x0E, 0x63, 0x77, 
    0xE2, 0x6D, 0x54, 0x25, 0xDC, 0xD6, 0x5F, 0xFF, 0xED, 0x6B, 0x9B, 0x7C, 0x28, 0xA4, 0x09, 0x63, 
    0x93, 0xCC, 0x91, 0xBB, 0xAC, 0x2A, 0xD6, 0x93, 0x56, 0x30, 0x1C, 0x11, 0x9C, 0x9D, 0xA4, 0xD9, 
    0xD5, 0x45, 0x52, 0x26, 0xDF, 0xFA, 0x65, 0xCB, 0xA1, 0xF0, 0x63, 0xA0, 0xE4, 0xB1, 0xCD, 0xFF, 
    0xB4, 0x52, 0x6B, 0x38, 0x35, 0x17, 0x29, 0x15, 0x8A, 0xCC, 0xEC, 0xDE, 0x5E, 0x87, 0x63, 0x47, 
    0xDE, 0x32, 0xBA, 0xB1, 0x90, 0xAF, 0x5C, 0xCB, 0x6F, 0xBA, 0x48, 0x29, 0x55, 0xF0, 0xED, 0xA7, 
    0xDD, 0x75, 0x7E, 0x68, 0x42, 0xD2, 0xB3, 0xC4, 0xC5, 0x2B, 0x90, 0x3B, 0xBC, 0xCF, 0xC6, 0x96, 
    0xA6, 0x5F, 0xB9, 0x70, 0xAD, 0x05, 0x67, 0xB3, 0x3A, 0xF9, 0x9D, 0x7F, 0xEF, 0x7A, 0xDE, 0x68, 
    0xCF, 0xEE, 0x1D, 0xDF, 0x2F, 0xFF, 0x3D, 0x38, 0x92, 0xF3, 0x85, 0x00, 0x11, 0x89, 0x9F, 0xBF, 
    0xDD, 0x88, 0x79, 0xAE, 0xE0, 0xE2, 0xE1, 0x56, 0x46, 0x8C, 0x32, 0x37, 0xCA, 0x6D, 0x25, 0x43, 
    0x7F, 0x90, 0xC0, 0xFB, 0xDA, 0xAC, 0xB4, 0x6D, 0xD3, 0xAF, 0x9B, 0xFD, 0x67, 0xEC, 0x3A, 0x6A, 
    0x9E, 0x8E, 0x19, 0xA3, 0xE6, 0x19, 0x63, 0x5A, 0x94, 0x7C, 0x71, 0x2F, 0x4A, 0xD3, 0x31, 0xDA, 
    0x8E, 0x00, 0x69, 0xD2, 0xD3, 0x1A, 0xAA, 0x3D, 0x24, 0xC1, 0xF3, 0xB5, 0x3C, 0x55, 0x76, 0xCB, 
    0x05, 0xB9, 0x95, 0x2F, 0x53, 0xBC, 0x98, 0x5E, 0x26, 0xC3, 0xD7, 0x1F, 0x87, 0x18, 0x24, 0x36, 
    0x9A, 0xEE, 0x8F, 0xF7, 0xF6, 0xB8, 0x06, 0xEC, 0x07, 0x7C, 0x5E, 0x96, 0x51, 0x3F, 0x13, 0x35, 
    0x14, 0x1B, 0x67, 0x10, 0xE8, 0x28, 0xB1, 0x9E, 0xBE, 0x5B, 0xBC, 0x13, 0x48, 0x9A, 0x76, 0x2F, 
    0x84, 0xAD, 0x63, 0x2C, 0x8F, 0xBA, 0x40, 0x6C, 0xA0
};

/**
 * Metadata for reset version tag :
 * Version Tag = 00.
 */
const uint8_t reset_version_tag_metadata[] = 
{
  0x20, 0x04,
        0xC1, 0x02, 0x00, 0x00
};

/**
 * Metadata for target OID :
 * Change access condition = Integrity protected using 0xE0E3.
 */
const uint8_t target_oid_metadata[] = 
{
  0x20, 0x05,
        0xD0, 0x03, 0x21, 0xE0, 0xE3,
}; 


/**
 * Metadata for target OID with confidentiality :
 * Change access condition = Integrity protected using 0xE0E3 & Confidentiality using 0xF1D1
 */
const uint8_t target_oid_metadata_with_confidentiality[] = 
{
    0x20, 0x09,
          //0xC1, 0x02, 0x00, 0x00,
          0xD0, 0x07, 0x21, 0xE0, 0xE3, 0xFD, 0x20, 0xF1, 0xD1
};

/**
 * Metadata for secure update of target OID metadata with confidentiality :
 * Change access condition = Integrity protected using 0xE0E3 & Confidentiality using 0xF1D1
 */
const uint8_t target_oid_metadata_for_secure_metadata_update[] =
{
    0x20, 0x0C,
        0xD8, 0x07, 0x21, 0xE0, 0xE3, 0xFD, 0x20, 0xF1, 0xD1, 0xF0, 0x01, 0x01
};

/**
 * Metadata for Trust Anchor :
 * Execute access condition = Always
 * Data object type  =  Trust Anchor
 */
uint8_t trust_anchor_metadata[] = 
{
    0x20, 0x06,
          0xD3, 0x01, 0x00,
          0xE8, 0x01, 0x11
};

/**
 * Metadata for target key OID :
 * Change access condition = Integrity protected using 0xE0E3
 * Execute access condition = Always
 */
const uint8_t target_key_oid_metadata[] = 
{
    0x20, 0x0C,
            0xC1, 0x02, 0x00, 0x00,
            0xD0, 0x03, 0x21, 0xE0, 0xE3,
            0xD3, 0x01, 0x00
};

/**
 * Metadata for shared secret OID :
 * Execute access condition = Always
 * Data object type  =  Protected updated secret
 */
uint8_t confidentiality_oid_metadata[] = 
{
    0x20, 0x06,
            0xD3, 0x01, 0x00,
            0xE8, 0x01, 0x23
};


/**
 * Shared secret data
 */
const unsigned char shared_secret[] = 
{
    0x49, 0xC9, 0xF4, 0x92, 0xA9, 0x92, 0xF6, 0xD4, 0xC5, 0x4F, 0x5B, 0x12, 0xC5, 0x7E, 0xDB, 0x27, 
    0xCE, 0xD2, 0x24, 0x04, 0x8F, 0x25, 0x48, 0x2A, 0xA1, 0x49, 0xC9, 0xF4, 0x92, 0xA9, 0x92, 0xF6, 
    0x49, 0xC9, 0xF4, 0x92, 0xA9, 0x92, 0xF6, 0xD4, 0xC5, 0x4F, 0x5B, 0x12, 0xC5, 0x7E, 0xDB, 0x27, 
    0xCE, 0xD2, 0x24, 0x04, 0x8F, 0x25, 0x48, 0x2A, 0xA1, 0x49, 0xC9, 0xF4, 0x92, 0xA9, 0x92, 0xF6
};


const optiga_protected_update_data_configuration_t  optiga_protected_update_data_set[] =
{
#ifdef INTEGRITY_PROTECTED
    {
        0xE0E1,
        target_oid_metadata,
        sizeof(target_oid_metadata),
        &data_integrity_configuration
    },
#endif

#ifdef CONFIDENTIALITY_PROTECTED
    {
        0xE0E1,
        target_oid_metadata_with_confidentiality,
        sizeof(target_oid_metadata_with_confidentiality),
        &data_confidentiality_configuration
    },
#endif

#ifdef AES_KEY_UPDATE
    {
        0xE200,
        target_oid_metadata,
        sizeof(target_oid_metadata),
        &data_aes_key_configuration
    },
#endif

#ifdef METADATA_UPDATE
    {
        0xE0E2,
        target_oid_metadata_for_secure_metadata_update,
        sizeof(target_oid_metadata_for_secure_metadata_update),
        &metadata_update_configuration
    },
#endif
    
#ifdef RSA_KEY_UPDATE
    {
        0xE0FC,
        target_key_oid_metadata,
        sizeof(target_key_oid_metadata),
        &data_rsa_key_configuration
    },
#endif
};

/**
 * Local functions prototype
 */
static optiga_lib_status_t write_metadata(optiga_util_t * me, uint16_t oid, uint8_t * metadata, uint8_t metadata_length)
{
    optiga_lib_status_t return_status = OPTIGA_LIB_SUCCESS;

    do
    {
        optiga_lib_status = OPTIGA_LIB_BUSY;
        return_status = optiga_util_write_metadata(me,
                                                   oid,
                                                   metadata,
                                                   metadata_length);
        if (OPTIGA_LIB_SUCCESS != return_status)
        {
            break;
        }

        while (OPTIGA_LIB_BUSY == optiga_lib_status)
        {
            //Wait until the optiga_util_write_metadata operation is completed
        }

        if (OPTIGA_LIB_SUCCESS != optiga_lib_status)
        {
            //writing metadata to a data object failed.
            return_status = optiga_lib_status;
            break;
        }
    } while (FALSE);

    return(return_status);
}

// Precondition 2
static optiga_lib_status_t write_confidentiality_oid(optiga_util_t * me, uint16_t confidentiality_oid)
{
    optiga_lib_status_t return_status = OPTIGA_UTIL_ERROR;

    do
    {
        /**
         * Precondition :
         * Metadata for 0xF1D1 :
         * Execute access condition = Always
         * Data object type  =  Protected updated secret
         */
        return_status = write_metadata(me,
                                       confidentiality_oid, 
                                       confidentiality_oid_metadata,
                                       sizeof(confidentiality_oid_metadata));
        if (OPTIGA_LIB_SUCCESS != return_status)
        {
            break;
        }


        /**
        *  Precondition :
        *  Write shared secret in OID 0xF1D1
        */
        optiga_lib_status = OPTIGA_LIB_BUSY;
        return_status = optiga_util_write_data(me,
                                                confidentiality_oid,
                                                OPTIGA_UTIL_ERASE_AND_WRITE,
                                                0,
                                                shared_secret,
                                                sizeof(shared_secret));

        if (OPTIGA_LIB_SUCCESS != return_status)
        {
            break;
        }

        while (OPTIGA_LIB_BUSY == optiga_lib_status)
        {
            //Wait until the optiga_util_write_data operation is completed
        }

        if (OPTIGA_LIB_SUCCESS != optiga_lib_status)
        {
            //writing data to a data object failed.
            return_status = optiga_lib_status;            
            break;
        }
    } while (FALSE);
    return (return_status);
}


// Precondition 1
static optiga_lib_status_t write_trust_anchor(optiga_util_t * me, uint16_t trust_anchor_oid)
{
    optiga_lib_status_t return_status = OPTIGA_LIB_SUCCESS;

    do
    {
        /**
         * Precondition :
         * Update Metadata for 0xE0E3 :
         * Execute access condition = Always
         * Data object type  =  Trust Anchor
         */
        return_status = write_metadata(me, trust_anchor_oid, trust_anchor_metadata, sizeof(trust_anchor_metadata));
        if (OPTIGA_LIB_SUCCESS != return_status)
        {
            break;
        }
        
        optiga_lib_status = OPTIGA_LIB_BUSY;
        return_status = optiga_util_write_data(me,
                                               trust_anchor_oid,
                                               OPTIGA_UTIL_ERASE_AND_WRITE,
                                               0,
                                               trust_anchor,
                                               sizeof(trust_anchor));

        if (OPTIGA_LIB_SUCCESS != return_status)
        {
            break;
        }

        while (OPTIGA_LIB_BUSY == optiga_lib_status)
        {
            //Wait until the optiga_util_write_data operation is completed
        }

        if (OPTIGA_LIB_SUCCESS != optiga_lib_status)
        {
            //writing data to a data object failed.
            return_status = optiga_lib_status;
            break;
        }
    } while (FALSE);
    return(return_status);
}

void example_optiga_util_protected_update(void)
{
    optiga_lib_status_t return_status = OPTIGA_LIB_SUCCESS;
    optiga_util_t * me = NULL;
    uint32_t time_taken = 0;
    uint16_t trust_anchor_oid = 0xE0E3;
    uint16_t confidentiality_oid = 0xF1D1;
    uint16_t data_config = 0;

    do
    {
        
#ifndef OPTIGA_INIT_DEINIT_DONE_EXCLUSIVELY
        /**
         * Open the application on OPTIGA which is a precondition to perform any other operations
         * using optiga_util_open_application
         */
        example_optiga_init();
#endif //OPTIGA_INIT_DEINIT_DONE_EXCLUSIVELY
        
        OPTIGA_EXAMPLE_LOG_MESSAGE(__FUNCTION__);
        /**
         *  Create OPTIGA util Instance
         *
         */
        me = optiga_util_create(0, optiga_util_callback, NULL);
        if (NULL == me)
        {
            break;
        }

        /**
        *  Precondition 1 :
        *  Update the metadata and trust anchor in OID 0xE0E3
        */
        return_status = write_trust_anchor(me, trust_anchor_oid);
        if (OPTIGA_LIB_SUCCESS != return_status)
        {
            break;
        }
        /**
        *  Precondition 2 :
        *  Update the metadata and secret for confidentiality in OID 0xF1D1
        */
        return_status = write_confidentiality_oid(me, confidentiality_oid);
        if (OPTIGA_LIB_SUCCESS != return_status)
        {
            break;
        }
        
        START_PERFORMANCE_MEASUREMENT(time_taken);
        
        for (data_config = 0; 
            data_config < \
            sizeof(optiga_protected_update_data_set)/sizeof(optiga_protected_update_data_configuration_t); data_config++)
        
        {
            /**
            *  Precondition 3 :
            *  Update the metadata of target OID
            */
            return_status = write_metadata(me,
                                       optiga_protected_update_data_set[data_config].target_oid,
                                       (uint8_t * )optiga_protected_update_data_set[data_config].target_oid_metadata,
                                       (uint8_t)optiga_protected_update_data_set[data_config].target_oid_metadata_length);
            if (OPTIGA_LIB_SUCCESS != return_status)
                
            {
                break;
            }
            
            
            
            /**
            *   Send the manifest using optiga_util_protected_update_start
            */
            
            optiga_lib_status = OPTIGA_LIB_BUSY;
            return_status = optiga_util_protected_update_start(me,
                                                               optiga_protected_update_data_set[data_config].data_config->manifest_version,
                                                               optiga_protected_update_data_set[data_config].data_config->manifest_data,
                                                               optiga_protected_update_data_set[data_config].data_config->manifest_length);

            WAIT_AND_CHECK_STATUS(return_status, optiga_lib_status);

            if (NULL != optiga_protected_update_data_set[data_config].data_config->continue_fragment_data)
            {
                /**
                *   Send the first fragment using optiga_util_protected_update_continue
                */
                optiga_lib_status = OPTIGA_LIB_BUSY;
                return_status = optiga_util_protected_update_continue(me,
                                                                      optiga_protected_update_data_set[data_config].data_config->continue_fragment_data,
                                                                      optiga_protected_update_data_set[data_config].data_config->continue_fragment_length);

                WAIT_AND_CHECK_STATUS(return_status, optiga_lib_status);
            }

            /**
            *   Send the last fragment using optiga_util_protected_update_final
            */
            optiga_lib_status = OPTIGA_LIB_BUSY;
            return_status = optiga_util_protected_update_final(me,
                                                               optiga_protected_update_data_set[data_config].data_config->final_fragment_data,
                                                               optiga_protected_update_data_set[data_config].data_config->final_fragment_length);

            WAIT_AND_CHECK_STATUS(return_status, optiga_lib_status);
            
            
            /**
            *  Postcondition  :
            *  Update the metadata of target OID for version tag to be 00
            */
            return_status = write_metadata(me,
                                           optiga_protected_update_data_set[data_config].target_oid,
                                           (uint8_t * )reset_version_tag_metadata,
                                           (uint8_t)sizeof(reset_version_tag_metadata));
            if (OPTIGA_LIB_SUCCESS != return_status)
            {
                break;
            }
            
        }
        READ_PERFORMANCE_MEASUREMENT(time_taken);
    } while (FALSE);
    OPTIGA_EXAMPLE_LOG_STATUS(return_status);
    
    if (me)
    {
        //Destroy the instance after the completion of usecase if not required.
        return_status = optiga_util_destroy(me);
        if (OPTIGA_LIB_SUCCESS != return_status)
        {
            //lint --e{774} suppress This is a generic macro
            OPTIGA_EXAMPLE_LOG_STATUS(return_status);
        }
    }
    
#ifndef OPTIGA_INIT_DEINIT_DONE_EXCLUSIVELY
    /**
     * Close the application on OPTIGA after all the operations are executed
     * using optiga_util_close_application
     */
    example_optiga_deinit();
#endif //OPTIGA_INIT_DEINIT_DONE_EXCLUSIVELY 
    OPTIGA_EXAMPLE_LOG_PERFORMANCE_VALUE(time_taken, return_status);
}

/**
* @}
*/
