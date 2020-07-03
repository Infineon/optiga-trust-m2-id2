/*
 * Copyright (C) 2015-2018 Alibaba Group Holding Limited
 */

#include <stdio.h>
//lint --e{766} suppress "string.h inclusion as its part of template"
#include <string.h>
#include "chip_config.h"
#include "irot_hal.h"

#if (CHIP_APDU_CMD_ADAPTER && (CHIP_TYPE_CONFIG == CHIP_TYPE_SE_OPTIGA_CMD))
#include "optiga/optiga_util.h"
#include "optiga/optiga_crypt.h"
#include "optiga/pal/pal_os_timer.h"

/* Macro to enable the OPTIGA chip to power off when irot_cleanup is invoked. By default, this MACRO is disabled.
 * If the macro is enabled, the HOST waits until the security event counter on OPTIGA reaches 0 and then 
 * closes the application on OPTIGA.
 */
//#define OPTIGA_SE_ENABLE_POWER_DOWN
// Device id to store ID of the Ali ID2 device
#define OPTIGA_SE_DEVICE_ID_OID         (0xF1D0)

// Length of the Ali ID2 device id      
#define OPTIGA_SE_DEVICE_ID_LEN         (12U)

#if (CHIP_CRYPTO_TYPE_CONFIG == CHIP_CRYPTO_TYPE_AES)
// Symmetric key object OID
#define IROT_HAL_OPTIGA_KEY_OID         ((optiga_key_id_t)0xE200)
#elif (CHIP_CRYPTO_TYPE_CONFIG == CHIP_CRYPTO_TYPE_RSA)
// Asymmetric RSA key object OID
#define IROT_HAL_OPTIGA_KEY_OID         ((optiga_key_id_t)0xE0FC)
#else                                   
#define IROT_HAL_OPTIGA_KEY_OID         ((optiga_key_id_t)0x0000)
#endif

// Mask for comms error
#define OPTIGA_SE_LIB_ERROR_LAYER_MASK  (0xFF00)
// Comms layer error code
#define OPTIGA_SE_COMMS_LAYER_ERROR     (0x0100)
// Maximum size of uint16_t
#define OPTIGA_SE_MAX_OUTPUT_LENGTH     (0x0000FFFF)

// static variable to store util instance
_STATIC_H optiga_util_t * p_irot_hal_util_instance = NULL;
// static variable to store crypt instance
_STATIC_H optiga_crypt_t * p_irot_hal_crypt_instance = NULL;

_STATIC_H volatile optiga_lib_status_t irot_hal_util_status;
_STATIC_H volatile optiga_lib_status_t irot_hal_crypt_status;

// IROT HAL initialization status
_STATIC_H bool_t irot_hal_init_status = FALSE;

//lint --e{818} suppress "argument "context" is not used in this usecase"
_STATIC_H void irot_hal_util_callback(void * context, optiga_lib_status_t return_status)
{
    irot_hal_util_status = return_status;
    if (NULL != context)
    {
        // callback to upper layer here
    }
}

//lint --e{818} suppress "argument "context" is not used in this usecase"
_STATIC_H void irot_hal_crypt_callback(void * context, optiga_lib_status_t return_status)
{
    irot_hal_crypt_status = return_status;
    if (NULL != context)
    {
        // callback to upper layer here
    }
}

irot_result_t irot_hal_init(void)
{
    irot_result_t return_status = IROT_ERROR_GENERIC;

    do
    {
        // If instances are already created, return success
        if((NULL != p_irot_hal_util_instance) && (NULL != p_irot_hal_crypt_instance))
        {
            return_status = IROT_SUCCESS;
            irot_hal_init_status = TRUE;
            break;
        }

        // Create util instance
        p_irot_hal_util_instance = optiga_util_create(0, irot_hal_util_callback, NULL);
        if (NULL == p_irot_hal_util_instance)
        {
            break;
        }

        // Create crypt instance
        p_irot_hal_crypt_instance = optiga_crypt_create(0, irot_hal_crypt_callback, NULL);
        if (NULL == p_irot_hal_crypt_instance)
        {
            break;
        }

        // Power On OPTIGA and send Open Application command
        irot_hal_util_status = OPTIGA_LIB_BUSY;

        if (OPTIGA_LIB_SUCCESS != optiga_util_open_application(p_irot_hal_util_instance, 0))
        {
            break;
        }

        while (OPTIGA_LIB_BUSY == irot_hal_util_status)
        {
            //Wait until the optiga_util_open_application is completed
        }
        //optiga util open application failed
        if (OPTIGA_LIB_SUCCESS != irot_hal_util_status)
        {
            if (OPTIGA_SE_COMMS_LAYER_ERROR == (irot_hal_util_status & OPTIGA_SE_LIB_ERROR_LAYER_MASK))
            {
                //Error from OPTIGA Comms.
                return_status = IROT_ERROR_COMMUNICATION;
            }
            break;
        }
        return_status = IROT_SUCCESS;
        irot_hal_init_status = TRUE;
    } while (FALSE);

    if ((IROT_SUCCESS != return_status) && (NULL != p_irot_hal_crypt_instance))
    {
        (void)optiga_crypt_destroy(p_irot_hal_crypt_instance);
        p_irot_hal_crypt_instance = NULL;
    }

    if ((IROT_SUCCESS != return_status) && (NULL != p_irot_hal_util_instance))
    {
        (void)optiga_util_destroy(p_irot_hal_util_instance);
        p_irot_hal_util_instance = NULL;
    }
    return (return_status);
}

irot_result_t irot_hal_cleanup(void)
{
    irot_result_t return_status;
    
#ifdef OPTIGA_SE_ENABLE_POWER_DOWN
    uint16_t bytes_to_read = 1;
    uint8_t security_event_counter = 0;

    do
    {
        // If instances are already destroyed, return success
        if(FALSE == irot_hal_init_status)
        {
            return_status = IROT_SUCCESS;
            break;
        }
        /**
        * Before performing power down, SEC value should be 0
        */
        do
        {
            irot_hal_util_status = OPTIGA_LIB_BUSY;
            return_status = IROT_ERROR_GENERIC;
            if (OPTIGA_LIB_SUCCESS != optiga_util_read_data(p_irot_hal_util_instance,
                                                    0xE0C5,
                                                    0x0000,
                                                    &security_event_counter,
                                                    &bytes_to_read))
            {
                break;
            }

            while (OPTIGA_LIB_BUSY == irot_hal_util_status)
            {
                //Wait until the optiga_util_read_data operation is completed
            }
            //Reading the data object failed.
            if (OPTIGA_LIB_SUCCESS != irot_hal_util_status)
            {
                break;
            }

            // Provide delay of 500ms in case SEC value is not equal to 0
            if (0 != security_event_counter)
            {
                pal_os_timer_delay_in_milliseconds(500);
            }
        } while (0 != security_event_counter);

        // Send Close Application command and Power Off OPTIGA
        irot_hal_util_status = OPTIGA_LIB_BUSY;
        return_status = IROT_ERROR_GENERIC;
        if (OPTIGA_LIB_SUCCESS != optiga_util_close_application(p_irot_hal_util_instance, 0))
        {
            break;
        }

        while (irot_hal_util_status == OPTIGA_LIB_BUSY)
        {
        //Wait until the optiga_util_close_application is completed
        }

        //optiga util close application failed
        if (OPTIGA_LIB_SUCCESS != irot_hal_util_status)
        {
            if (OPTIGA_SE_COMMS_LAYER_ERROR == (irot_hal_util_status & OPTIGA_SE_LIB_ERROR_LAYER_MASK))
            {
                //Error from OPTIGA Comms.
                return_status = IROT_ERROR_COMMUNICATION;
            }
            break;
        }
        return_status = IROT_SUCCESS;
    } while (FALSE);

    if ((NULL != p_irot_hal_crypt_instance) && OPTIGA_LIB_SUCCESS != optiga_crypt_destroy(p_irot_hal_crypt_instance))
    {
        if (IROT_SUCCESS == return_status)
        {
            return_status = IROT_ERROR_GENERIC;
        }
    }
    p_irot_hal_crypt_instance = NULL;

    if ((NULL != p_irot_hal_util_instance) && OPTIGA_LIB_SUCCESS != optiga_util_destroy(p_irot_hal_util_instance))
    {
        if (IROT_SUCCESS == return_status)
        {
            return_status = IROT_ERROR_GENERIC;
        }
    }
    p_irot_hal_util_instance = NULL;
#else
    return_status = IROT_SUCCESS;
#endif
    irot_hal_init_status = FALSE;
    return (return_status);
}

irot_result_t irot_hal_get_id2(uint8_t * id2, uint32_t * len)
{
    irot_result_t return_status = IROT_ERROR_GENERIC;
    
    do
    {
        if (FALSE == irot_hal_init_status)
        {
            break;
        }

        // Check if the 'len' value passed is lesser than the minimum value Device ID length
        if (OPTIGA_SE_DEVICE_ID_LEN > *len)
        {
            return_status = IROT_ERROR_SHORT_BUFFER;
            break;
        }
        
        // Check if the 'len' value passed is greater than the max length accepted by optiga_util_read_data
        if (OPTIGA_SE_MAX_OUTPUT_LENGTH < *len)
        {
            *len = OPTIGA_SE_MAX_OUTPUT_LENGTH;
        }
        
        irot_hal_util_status = OPTIGA_LIB_BUSY;
        // Read device ID from the OID configured in OPTIGA_SE_DEVICE_ID_OID
        if (OPTIGA_LIB_SUCCESS != optiga_util_read_data(p_irot_hal_util_instance,
                                                        OPTIGA_SE_DEVICE_ID_OID,
                                                        0x0000,
                                                        id2,
                                                        ((uint16_t * )(void * )len)))
        {
            break;
        }

        while (OPTIGA_LIB_BUSY == irot_hal_util_status)
        {
            //Wait until the optiga_util_read_data operation is completed
        }

        if (OPTIGA_LIB_SUCCESS != irot_hal_util_status)
        {
            do
            {
                if (OPTIGA_DEVICE_ERROR == (irot_hal_util_status & OPTIGA_DEVICE_ERROR))
                {
                    //Reading the device ID failed.
                    return_status = IROT_ERROR_ITEM_NOT_FOUND;
                    break;
                }

                if (OPTIGA_SE_COMMS_LAYER_ERROR == (irot_hal_util_status & OPTIGA_SE_LIB_ERROR_LAYER_MASK))
                {
                    //Error from OPTIGA Comms.
                    return_status = IROT_ERROR_COMMUNICATION;
                    break;
                }
            } while (FALSE);
            break;
        }
        // Check if the device ID received matches the expected length
        if (OPTIGA_SE_DEVICE_ID_LEN != *len)
        {
            return_status = IROT_ERROR_ITEM_NOT_FOUND;
            break;
        }
        return_status = IROT_SUCCESS;
    } while (FALSE);

    if (IROT_SUCCESS != return_status)
    {
        *len = 0;
    }

    return (return_status);
}

#if ((CHIP_CRYPTO_TYPE_CONFIG == CHIP_CRYPTO_TYPE_AES) || (CHIP_CRYPTO_TYPE_CONFIG == CHIP_CRYPTO_TYPE_3DES))
//lint --e{715} suppress "argument "key_id", "iv" and "iv_len" is not used"
irot_result_t irot_hal_sym_crypto(key_object * key_obj,
                                  uint8_t key_id,
                                  const uint8_t * iv,
                                  uint32_t iv_len,
                                  const uint8_t * in,
                                  uint32_t in_len,
                                  uint8_t * out,
                                  uint32_t * out_len,
                                  sym_crypto_param_t * crypto_param)
{
    irot_result_t return_status = IROT_ERROR_GENERIC;
  
    do
    {
        // Check for crypt instance NULL 
        if (FALSE == irot_hal_init_status)
        {
            break;
        }

        // Check for key object NULL
        if (NULL != key_obj)
        {
            return_status = IROT_ERROR_NOT_SUPPORTED;
            break;
        }
        
        // Check the mode for encrypt or decrypt
        if ((MODE_DECRYPT != crypto_param->mode) && (MODE_ENCRYPT != crypto_param->mode))
        {
            return_status = IROT_ERROR_BAD_PARAMETERS;
            break;
        }
        
        // Check the mode of operation as ECB and no padding
        if ((BLOCK_MODE_ECB != crypto_param->block_mode) || (SYM_PADDING_NOPADDING != crypto_param->padding_type))
        {
            return_status = IROT_ERROR_NOT_IMPLEMENTED;
            break;
        }
                                                        
        // Check for out buffer length
        if (*out_len < in_len)
        {
            return_status = IROT_ERROR_SHORT_BUFFER;
            break;
        }
        
        /*
          Check if request is for decryption or encryption
        */
        irot_hal_crypt_status = OPTIGA_LIB_BUSY;
        if (MODE_DECRYPT == crypto_param->mode)
        {
            // Decrypt input data based on symmetric key stored in IROT_HAL_OPTIGA_SYMMETRIC_KEY_OID
            if (OPTIGA_LIB_SUCCESS != optiga_crypt_symmetric_decrypt_ecb(p_irot_hal_crypt_instance,
                                                                         IROT_HAL_OPTIGA_KEY_OID,
                                                                         in,
                                                                         in_len,
                                                                         out,
                                                                         out_len))
            {
                break;
            }
        }
        else 
        {
            // Encrypt input data based on symmetric key stored in IROT_HAL_OPTIGA_SYMMETRIC_KEY_OID
            if (OPTIGA_LIB_SUCCESS != optiga_crypt_symmetric_encrypt_ecb(p_irot_hal_crypt_instance,
                                                                         IROT_HAL_OPTIGA_KEY_OID,
                                                                         in,
                                                                         in_len,
                                                                         out,
                                                                         out_len))
            {
                break;
            }
        }



        while (OPTIGA_LIB_BUSY == irot_hal_crypt_status)
        {
            //Wait until the encrypt/decrypt operation is completed
        }

        if (OPTIGA_LIB_SUCCESS != irot_hal_crypt_status)
        {
            if (OPTIGA_SE_COMMS_LAYER_ERROR == (irot_hal_crypt_status & OPTIGA_SE_LIB_ERROR_LAYER_MASK))
            {
                //Error from OPTIGA Comms.
                return_status = IROT_ERROR_COMMUNICATION;
            }
            break;
        }
        return_status = IROT_SUCCESS;
    } while (FALSE);
    
    if (IROT_SUCCESS != return_status)
    {
        *out_len = 0;
    }
    return (return_status);
}
#endif // ((CHIP_CRYPTO_TYPE_CONFIG == CHIP_CRYPTO_TYPE_AES) || (CHIP_CRYPTO_TYPE_CONFIG == CHIP_CRYPTO_TYPE_3DES))

irot_result_t irot_hal_get_random(uint8_t *buf, uint32_t len)
{
    // chip_log function is not implemented
    
    // API returns IROT_ERROR_NOT_IMPLEMENTED as it's not implemented
    return IROT_ERROR_NOT_IMPLEMENTED;

}

irot_result_t irot_hal_hash_sum(const uint8_t *in, uint32_t in_len, uint8_t *out, uint32_t *out_len, hash_t type)
{
    // chip_log function is not implemented
    
    // API returns IROT_ERROR_NOT_IMPLEMENTED as it's not implemented
    return IROT_ERROR_NOT_IMPLEMENTED;

}

#if (CHIP_CRYPTO_TYPE_CONFIG == CHIP_CRYPTO_TYPE_RSA)
static irot_result_t irot_hal_generate_hash(const uint8_t *in_data, 
                                            uint32_t *in_data_len, 
                                            uint8_t *out_data, 
                                            uint16_t *out_data_len, 
                                            optiga_rsa_signature_scheme_t type)
{

#define OPTIGA_SE_SHA_256_HASH_OUT_LENGTH  (0x20)  
#define OPTIGA_SE_SHA_384_HASH_OUT_LENGTH  (0x30)  
    irot_result_t return_status = IROT_ERROR_GENERIC;
    hash_data_from_host_t hash_data_host;
    optiga_hash_type_t hash_type;

    do
    {
        if (OPTIGA_RSASSA_PKCS1_V15_SHA256 == type)
        {
            *out_data_len = OPTIGA_SE_SHA_256_HASH_OUT_LENGTH;
            hash_type = OPTIGA_HASH_TYPE_SHA_256;
        }  
        else if (OPTIGA_RSASSA_PKCS1_V15_SHA384 == type)
        {
            return_status = IROT_ERROR_NOT_IMPLEMENTED;
            break;
        }
        hash_data_host.buffer = in_data;
        hash_data_host.length = *in_data_len;
        irot_hal_crypt_status = OPTIGA_LIB_BUSY;  
        if (OPTIGA_LIB_SUCCESS != optiga_crypt_hash(p_irot_hal_crypt_instance, 
                                                    hash_type, 
                                                    OPTIGA_CRYPT_HOST_DATA, 
                                                    &hash_data_host, 
                                                    out_data))
        {
            break;
        }

        while (OPTIGA_LIB_BUSY == irot_hal_crypt_status)
        {
            //Wait until the optiga_crypt_hash operation is completed
        }

        if (OPTIGA_LIB_SUCCESS != irot_hal_crypt_status)
        {
            if (OPTIGA_SE_COMMS_LAYER_ERROR == (irot_hal_crypt_status & OPTIGA_SE_LIB_ERROR_LAYER_MASK))
            {
                //Error from OPTIGA Comms.
                return_status = IROT_ERROR_COMMUNICATION;
            }
            break;
        }

        return_status = IROT_SUCCESS;
    } while (FALSE);
#undef OPTIGA_SE_SHA_256_HASH_OUT_LENGTH 
#undef OPTIGA_SE_SHA_384_HASH_OUT_LENGTH
    return (return_status);
}
static irot_result_t irot_hal_get_signature_scheme(asym_sign_verify_t type, 
                                                   optiga_rsa_signature_scheme_t *signature_scheme)
{
    irot_result_t return_status = IROT_SUCCESS;
    switch(type)
    {
        case ASYM_TYPE_RSA_SHA256_PKCS1: 
        {
            *signature_scheme = OPTIGA_RSASSA_PKCS1_V15_SHA256;
        }
        break;
        case ASYM_TYPE_RSA_SHA384_PKCS1: 
        {
            *signature_scheme = OPTIGA_RSASSA_PKCS1_V15_SHA384;
        }
        break;
        default :
        {
            return_status = IROT_ERROR_NOT_SUPPORTED;
        }
        break;
    }
    return (return_status);
}

irot_result_t irot_hal_asym_priv_sign(key_object *key_obj, 
                                      uint8_t key_id, 
                                      const uint8_t *in, 
                                      uint32_t in_len,
                                      uint8_t *out, 
                                      uint32_t *out_len, 
                                      asym_sign_verify_t type)
{
    irot_result_t return_status = IROT_ERROR_GENERIC;
    optiga_rsa_signature_scheme_t signature_scheme;
    uint16_t digest_len;
#define OPTIGA_SE_DIGEST_LENGTH (0x30)

    uint8_t digest[OPTIGA_SE_DIGEST_LENGTH];
    do
    {   
        // Check for crypt instance NULL 
        if (FALSE == irot_hal_init_status)
        {
            break;
        }

        // Check for key object NULL
        if (NULL != key_obj)
        {
            return_status = IROT_ERROR_NOT_SUPPORTED;
            break;
        }

        // Check supported scheme
        if (IROT_SUCCESS != irot_hal_get_signature_scheme(type,
                                                          &signature_scheme))
        {
            return_status = IROT_ERROR_NOT_SUPPORTED;
            break;
        }
        
        //Generate hash of input data
        return_status = irot_hal_generate_hash(in, 
                                               &in_len, 
                                               digest, 
                                               &digest_len,
                                               signature_scheme);
        if (IROT_SUCCESS != return_status)
        {
            break;
        }
        return_status = IROT_ERROR_GENERIC;
        
        // Out put boundary check
        *out_len = (*out_len > OPTIGA_SE_MAX_OUTPUT_LENGTH) ? (OPTIGA_SE_MAX_OUTPUT_LENGTH) : (*out_len);
        
        irot_hal_crypt_status = OPTIGA_LIB_BUSY;        
        // Generate signature of input data based on asymmetric RSA key stored in IROT_HAL_OPTIGA_KEY_OID
        if (OPTIGA_LIB_SUCCESS != optiga_crypt_rsa_sign(p_irot_hal_crypt_instance,
                                                        signature_scheme,
                                                        digest,
                                                        (uint8_t)digest_len,
                                                        IROT_HAL_OPTIGA_KEY_OID,
                                                        out,
                                                        (uint16_t *)out_len,
                                                        0x0000))
        {
            break;
        }
        while (OPTIGA_LIB_BUSY == irot_hal_crypt_status)
        {
            //Wait until the rsa sign operation is completed
        }

        if (OPTIGA_LIB_SUCCESS != irot_hal_crypt_status)
        {
            if (OPTIGA_SE_COMMS_LAYER_ERROR == (irot_hal_crypt_status & OPTIGA_SE_LIB_ERROR_LAYER_MASK))
            {
                //Error from OPTIGA Comms.
                return_status = IROT_ERROR_COMMUNICATION;
            }
            else if (OPTIGA_CMD_ERROR_MEMORY_INSUFFICIENT & irot_hal_crypt_status)
            {
                return_status = IROT_ERROR_SHORT_BUFFER;
            }
            break;
        }
        return_status = IROT_SUCCESS;
    } while (FALSE);
    
    if (IROT_SUCCESS != return_status)
    {
        *out_len = 0;
    }    
#undef OPTIGA_SE_DIGEST_LENGTH
    return (return_status);
}

irot_result_t irot_hal_asym_priv_decrypt(key_object *key_obj,
                                         uint8_t key_id, 
                                         const uint8_t *in,
                                         uint32_t in_len,
                                         uint8_t *out, 
                                         uint32_t *out_len, 
                                         irot_asym_padding_t padding)
{
    irot_result_t return_status = IROT_ERROR_GENERIC;
  
    do
    {
        // Check for crypt instance NULL 
        if (FALSE == irot_hal_init_status)
        {
            break;
        }

        // Check for key object NULL, padding and in_len size
        if ((NULL != key_obj) || (ASYM_PADDING_PKCS1 != padding) || (in_len != LENGTH_RSA_1024))
        {
            return_status = IROT_ERROR_NOT_SUPPORTED;
            break;
        }

        *out_len = (*out_len > OPTIGA_SE_MAX_OUTPUT_LENGTH) ? (OPTIGA_SE_MAX_OUTPUT_LENGTH) : (*out_len);
        
        /*
        * Decrypt input data based on asymmetric key stored in IROT_HAL_OPTIGA_KEY_OID
        */
        irot_hal_crypt_status = OPTIGA_LIB_BUSY;
        // RSA decryption
        if (OPTIGA_LIB_SUCCESS != optiga_crypt_rsa_decrypt_and_export(p_irot_hal_crypt_instance,
                                                                      OPTIGA_RSAES_PKCS1_V15,
                                                                      in,
                                                                      in_len,
                                                                      NULL,
                                                                      0,
                                                                      IROT_HAL_OPTIGA_KEY_OID,
                                                                      out,
                                                                      (uint16_t *)out_len))
        {
            break;
        }

        while (OPTIGA_LIB_BUSY == irot_hal_crypt_status)
        {
            //Wait until the decrypt operation is completed
        }

        if (OPTIGA_LIB_SUCCESS != irot_hal_crypt_status)
        {
            if (OPTIGA_SE_COMMS_LAYER_ERROR == (irot_hal_crypt_status & OPTIGA_SE_LIB_ERROR_LAYER_MASK))
            {
                //Error from OPTIGA Comms.
                return_status = IROT_ERROR_COMMUNICATION;
            }
            else if (OPTIGA_CMD_ERROR_MEMORY_INSUFFICIENT == (OPTIGA_CMD_ERROR_MEMORY_INSUFFICIENT & irot_hal_crypt_status))
            {
                return_status = IROT_ERROR_SHORT_BUFFER;
            }
                
            break;
        }
        return_status = IROT_SUCCESS;
    } while (FALSE);
    
    if (IROT_SUCCESS != return_status)
    {
        *out_len = 0;
    }
    
    return (return_status);
}
#endif

#endif
/**
* @}
*/
