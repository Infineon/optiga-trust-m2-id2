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
* \file example_optiga_se_adapter.c
*
* \brief   This file provides an example of ali id2 usecase with irot hal APIs from optiga
*
* \ingroup grUseCases
*
* @{
*/

#include "chip_config.h"
#include "irot_hal.h"
#include "optiga_example.h"
#include "optiga/pal/pal_os_memory.h"
#include "optiga/common/optiga_lib_return_codes.h"

#define PLAIN_TEXT_SIZE             (0x20)    
#if ((CHIP_CRYPTO_TYPE_CONFIG == CHIP_CRYPTO_TYPE_AES) || (CHIP_CRYPTO_TYPE_CONFIG == CHIP_CRYPTO_TYPE_3DES))
//lint --e{526} suppress "symbol not defined"
void example_optiga_crypt_symmetric_generate_key(void);
#endif
#if (CHIP_CRYPTO_TYPE_CONFIG == CHIP_CRYPTO_TYPE_RSA)
void example_optiga_util_protected_update(void);
#endif
/**
 * The below example demonstrates usage of IROT HAL APIs for Ali ID2 client usecase
 */
#if ((CHIP_CRYPTO_TYPE_CONFIG == CHIP_CRYPTO_TYPE_AES) || (CHIP_CRYPTO_TYPE_CONFIG == CHIP_CRYPTO_TYPE_3DES))
void example_optiga_se_adapter_aes(void)
{ 
    
    irot_result_t return_status = IROT_ERROR_GENERIC;
    uint8_t device_id_buffer[12];
    uint32_t device_id_buffer_length = sizeof(device_id_buffer);

    const uint8_t plain_text[PLAIN_TEXT_SIZE] = {
                                    0x6c, 0x69, 0x6e, 0x65, 0x31, 0x20, 0x3a, 0x20, 
                                    0x54, 0x68, 0x69, 0x73, 0x20, 0x69, 0x73, 0x20, 
                                    0x61, 0x20, 0x73, 0x61, 0x6d, 0x70, 0x6c, 0x65, 
                                    0x20, 0x70, 0x61, 0x79, 0x6c, 0x6f, 0x61, 0x64
                                };
    
    uint8_t cipher_text[PLAIN_TEXT_SIZE];
    uint32_t cipher_text_length = sizeof(cipher_text);
    uint8_t decipher_text[PLAIN_TEXT_SIZE];    
    uint32_t decipher_text_length = sizeof(decipher_text);
    sym_crypto_param_t sym_crypto_param;
    
    pal_os_memset(cipher_text,0x00,cipher_text_length);    
    pal_os_memset(decipher_text,0x00,decipher_text_length);

    OPTIGA_EXAMPLE_LOG_MESSAGE(__FUNCTION__);
                                
    do
    {
        /**
         * 1. Initialize IROT HAL layer and power on OPTIGA.
         */
        return_status = irot_hal_init();
        if (IROT_SUCCESS != return_status)
        {
            break;
        }

        /**
         * 2. Obtain Device ID
         */
        return_status = irot_hal_get_id2(device_id_buffer,&device_id_buffer_length);
        if (IROT_SUCCESS != return_status)
        {
            break;
        }

        /**
         * 3. Update the symmetric key in 0xE200
         */        
        example_optiga_crypt_symmetric_generate_key();
        
        /**
         * 4. Encrypt the plain text using ECB mode of operation
         */
        sym_crypto_param.block_mode = BLOCK_MODE_ECB;
        sym_crypto_param.mode = MODE_ENCRYPT;
        sym_crypto_param.padding_type = SYM_PADDING_NOPADDING;
        
        return_status = irot_hal_sym_crypto(NULL,
                                            0, 
                                            NULL,
                                            0,
                                            plain_text,
                                            sizeof(plain_text),
                                            cipher_text, 
                                            &cipher_text_length, 
                                            &sym_crypto_param);
        if (IROT_SUCCESS != return_status)
        {
            break;
        }
        
        /**
         * 5. Decrypt the cipher text using ECB mode of operation
         */
        sym_crypto_param.block_mode = BLOCK_MODE_ECB;
        sym_crypto_param.mode = MODE_DECRYPT;
        sym_crypto_param.padding_type = SYM_PADDING_NOPADDING;

        return_status = irot_hal_sym_crypto(NULL,
                                            0, 
                                            NULL,
                                            0,
                                            cipher_text,
                                            cipher_text_length,
                                            decipher_text, 
                                            &decipher_text_length, 
                                            &sym_crypto_param);        
        if (IROT_SUCCESS != return_status)
        {
            break;
        }
        
    } while (FALSE);
    OPTIGA_EXAMPLE_LOG_STATUS((optiga_lib_status_t)(int)return_status);

    /**
     * 6. Power down OPTIGA and de-initialize IROTHAL layer
     */
    return_status = irot_hal_cleanup();
    if(IROT_SUCCESS != return_status)
    {
        //lint --e{774} suppress This is a generic macro
        OPTIGA_EXAMPLE_LOG_STATUS((optiga_lib_status_t)(int)return_status);
    }

}
#endif

#if (CHIP_CRYPTO_TYPE_CONFIG == CHIP_CRYPTO_TYPE_RSA)
void example_optiga_se_adapter_rsa(void)
{ 
    irot_result_t return_status = IROT_ERROR_GENERIC;
    uint8_t device_id_buffer[12];
    uint32_t device_id_buffer_length = sizeof(device_id_buffer);

    uint8_t cipher_text[LENGTH_RSA_1024] = {
        0xBD, 0x6B, 0x42, 0xE3, 0x4E, 0x62, 0x77, 0x25, 0x89, 0x62, 0x8C, 0x32, 0x8D, 0xF5, 0xC0, 0x07,
        0xAE, 0x05, 0x0C, 0x24, 0x60, 0x98, 0x48, 0x83, 0x6E, 0x76, 0xDB, 0x33, 0x41, 0x40, 0xDF, 0x70,
        0xFC, 0x01, 0x6A, 0x6C, 0x10, 0xF2, 0xD0, 0x34, 0xBE, 0x9F, 0x6C, 0xE9, 0xE5, 0x5A, 0x7A, 0x69,
        0xCA, 0x52, 0x60, 0xF9, 0x26, 0x5F, 0x74, 0xFB, 0x6B, 0xB4, 0x19, 0xF8, 0x3B, 0xFE, 0x40, 0xF9,
        0x0F, 0x17, 0xB4, 0x38, 0x47, 0xF3, 0x96, 0xA0, 0xFF, 0xF3, 0xE1, 0x0B, 0x71, 0xE0, 0xC8, 0x58,
        0xE3, 0x53, 0x52, 0xDB, 0xB0, 0x13, 0xC1, 0xBE, 0x17, 0xB6, 0x9E, 0xC1, 0xF1, 0xD7, 0x32, 0xBE,
        0xD4, 0xCC, 0x9B, 0x3C, 0x36, 0x8A, 0x4F, 0x55, 0xB2, 0xEB, 0xF8, 0x9A, 0x4C, 0x1C, 0x64, 0xE0,
        0x8C, 0x6F, 0x80, 0x03, 0x26, 0xD0, 0x72, 0x33, 0x4E, 0x0A, 0x66, 0x4F, 0xF8, 0x61, 0x66, 0xA4
    };
    // SHA-256 digest to be signed
    static uint8_t digest [] =
    {
        0x61, 0xC7, 0xDE, 0xF9, 0x0F, 0xD5, 0xCD, 0x7A,0x8B, 0x7A, 0x36, 0x41, 0x04, 0xE0, 0x0D, 0x82,
        0x38, 0x46, 0xBF, 0xB7, 0x70, 0xEE, 0xBF, 0x8F,0x40, 0x25, 0x2E, 0x0A, 0x21, 0x42, 0xAF, 0x9C,
    };

    uint8_t signature [200];
    uint32_t signature_length = sizeof(signature);
    uint32_t cipher_text_length = sizeof(cipher_text);
    uint8_t decipher_text[PLAIN_TEXT_SIZE];    
    uint32_t decipher_text_length = sizeof(decipher_text);
    irot_asym_padding_t padding;

    pal_os_memset(decipher_text,0x00,decipher_text_length);

    OPTIGA_EXAMPLE_LOG_MESSAGE(__FUNCTION__);
                                
    do
    {
        /**
         * 1. Initialize IROT HAL layer and power on OPTIGA.
         */
        return_status = irot_hal_init();
        if (IROT_SUCCESS != return_status)
        {
            break;
        }

        /**
         * 2. Obtain Device ID
         */
        return_status = irot_hal_get_id2(device_id_buffer,&device_id_buffer_length);
        if (IROT_SUCCESS != return_status)
        {
            break;
        }

        /**
         * 3. Update the asymmetric key in 0xE0FC. RSA key update macro should be enabled in below example
         */        
        example_optiga_util_protected_update();
        
        return_status = irot_hal_asym_priv_sign(NULL,
                                               0, 
                                               digest,
                                               sizeof(digest),
                                               signature, 
                                               &signature_length, 
                                               ASYM_TYPE_RSA_SHA256_PKCS1);
        if (IROT_SUCCESS != return_status)
        {
            break;
        }
        /**
         * 4. Decrypt the cipher text
         */
        padding = ASYM_PADDING_PKCS1;
        return_status = irot_hal_asym_priv_decrypt(NULL,
                                                   0, 
                                                   cipher_text,
                                                   cipher_text_length,
                                                   decipher_text, 
                                                   &decipher_text_length, 
                                                   padding);
        if (IROT_SUCCESS != return_status)
        {
            break;
        }
    } while (FALSE);
    OPTIGA_EXAMPLE_LOG_STATUS((optiga_lib_status_t)(int)return_status);

    /**
     * 5. Power down OPTIGA and de-initialize IROTHAL layer
     */
    return_status = irot_hal_cleanup();
    if(IROT_SUCCESS != return_status)
    {
        //lint --e{774} suppress This is a generic macro
        OPTIGA_EXAMPLE_LOG_STATUS((optiga_lib_status_t)(int)return_status);
    }

}
#endif
        
/**
 * @}
 */
