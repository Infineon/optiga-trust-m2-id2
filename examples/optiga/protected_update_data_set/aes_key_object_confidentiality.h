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
* \file aes_key_object_confidentiality.h
*
* \brief   This file provides the data set for protected update of aes key object with confidentiality.
*
* \ingroup grOptigaExamples
*
* @{
*/

#include <stdint.h>

/**
 * Manifest for AES key update with integrity protected and confidentiality
 */
const uint8_t manifest_aes_key[] = 
{
    // COSE Sign1 Trust
    0x84,
        // Protected signed header trust
        0x47,
            // Trust Sign Algorithm
            0xA1, 
            // RSA
            0x01, 0x3A, 0x00, 0x01, 0x00, 0xA3, 
        // Unprotected signed header trust
        0xA1, 
            // Root of trust
            0x04, 0x42, 
            // Trust Anchor OID
            0xE0, 0xE3, 
        // Payload info Byte string of single byte length
        0x58, 
            // Byte string length of manifest            
            0x9C, 
            // Trust manifest, array of 6 entries
            0x86, 
            // Version(Major Type 0)
            0x01,
            // NULL
            0xF6,
            // NULL
            0xF6,
            // Resources, array of 4 entries
            0x84, 
                // Trust Payload Type
                0x22,
                // Payload Length 
                0x13,
                // Trust Payload version
                0x03, 
                // Trust Add info data 
                0x82, 
                    // key algorithm
                    0x18, 0x81,
                    // key usage
                    0x02, 
                // Trust Processors, array of 2 entries 
                0x82, 
                    // Processing step integrity, array of 2 entries
                    0x82, 
                        // Process( Major Type 1)
                        0x20, 
                        // Parameters, byte string with single byte length
                        0x58, 
                            // Byte string length 
                            0x25, 
                            // IFX Digest info, array of 2 entries
                            0x82, 
                                // Digest Algorithm
                                0x18,
                                    // SHA-256
                                    0x29, 
                                // Digest
                                0x58, 
                                    // Byte string length
                                    0x20, 
                                        // Digest data
                                        0xC7, 0xD6, 0xD4, 0xE4, 0xBF, 0xD0, 0xA7, 0x98, 
                                        0x2D, 0xC5, 0x74, 0xB2, 0x96, 0xE9, 0x04, 0xCD,
                                        0xDE, 0xB9, 0x78, 0xEF, 0x6C, 0xFD, 0x70, 0xCA,
                                        0x10, 0xF9, 0x39, 0xA8, 0x98, 0x0E, 0x7E, 0xB6,     
                    // Processing step decrypt, array of 2 entries
                    0x82,
                        // Process( Major Type 0)
                        0x01,
                        // COSE_Encrypt_Trust array of 3 entries
                        0x83,
                            // protected-encrypt-header-Trust
                            0x43,
                                // Map 
                                0xA1,
                                    // Key 
                                    0x01,
                                    // AES-CCM-16-64-128, 128-bit key, 64-bit Authentication(MAC) tag, 13-byte nonce
                                    0x0A,
                            // recipients array of 1
                            0x81,
                                // COSE_Recipient_Trust array of 2
                                0x82,
                                    // protected-recipient-header-Trust
                                    0x58,
                                        // byte str
                                        0x54,
                                            // Map of 3
                                            0xA3,
                                                // Key
                                                0x04,
                                                // Value
                                                0x42,
                                                    // Shared secret OID
                                                    0xF1, 0xD1,
                                                // Key
                                                0x01,
                                                // Value (KeyDerivationAlgorithms)
                                                0x3A, 0x00, 0x01, 0x00, 0xB7,
                                                // Key
                                                0x05,
                                                // Value array of 2
                                                0x82,
                                                    0x44,
                                                        // label(4)
                                                        0x74, 0x65, 0x73, 0x74,
                                                    0x58,
                                                        0x40,
                                                        // seed(64)
                                                        0x5E, 0xC8, 0x4C, 0x3C, 0x8E, 0xC5, 0x9D, 0xC3, 
                                                        0x66, 0x0A, 0x52, 0x5E, 0x67, 0x95, 0xD9, 0xC9,
                                                        0x1C, 0x31, 0x6A, 0x47, 0x21, 0x2F, 0xB9, 0xDB, 
                                                        0xF7, 0x3E, 0x5F, 0x84, 0xA8, 0xC4, 0x81, 0xE7, 
                                                        0x7B, 0xDA, 0xB9, 0x94, 0xF3, 0xE7, 0x04, 0x50, 
                                                        0x8A, 0x51, 0x94, 0x03, 0xA9, 0x8D, 0x5E, 0x2A,
                                                        0x09, 0x79, 0x10, 0x88, 0xB0, 0x1B, 0xC8, 0x8C, 
                                                        0x98, 0x2D, 0x53, 0x26, 0xAC, 0x34, 0xDC, 0xD7, 
                                    // ciphertext-recipient-header-Trust
                                    0xF6,
                            // AdditionalInfo
                            0xF6,
                // Trust Target
                0x82, 
                    // Component identifier(0)
                    0x40,
                    // Storage identifier
                    0x42, 
                    // Optiga target OID
                    0xE2, 0x00, 
        // Signature info, byte string of single byte length
        0x58, 
            // Byte string length for RSA 1024
            0x80, 
            // Signature data
            0xBB, 0xF9, 0x64, 0x7F, 0x56, 0x50, 0xA1, 0xAA, 0x46, 0x8D, 0xE3, 0x4D, 0xC8, 0x35, 0xB1, 0xAD, 
            0xD3, 0x74, 0x14, 0x01, 0xB0, 0x2A, 0x05, 0xEC, 0xA8, 0x16, 0x04, 0xFD, 0x79, 0x99, 0xCF, 0xB5, 
            0x37, 0xF5, 0x4C, 0x2D, 0xDE, 0x20, 0x54, 0xF3, 0x93, 0x53, 0xD3, 0x29, 0x00, 0xBC, 0x0E, 0xF5, 
            0x14, 0x2A, 0xEB, 0xE8, 0x58, 0xA0, 0xCB, 0x72, 0xE4, 0x97, 0xA3, 0x21, 0x27, 0x79, 0x81, 0x57,
            0x1F, 0x99, 0xC9, 0xDB, 0xD1, 0xA4, 0x48, 0x8A, 0xC1, 0x71, 0xF0, 0x80, 0x4E, 0xC1, 0x00, 0x54,
            0xC6, 0x74, 0x0D, 0xF2, 0x4F, 0x76, 0x44, 0xC1, 0x46, 0x26, 0xCB, 0xAC, 0x01, 0x67, 0x32, 0x2A,
            0xF4, 0xCE, 0xD7, 0xB4, 0x48, 0x9C, 0x5D, 0xA4, 0xF5, 0xAD, 0x91, 0x40, 0x30, 0x9F, 0x7C, 0x8B,
            0x08, 0x13, 0x99, 0xCD, 0xA6, 0x9D, 0x84, 0xBF, 0x68, 0x9E, 0x7A, 0xE6, 0xE6, 0xB7, 0xAA, 0x4D,

};

/**
 * Fragment array for final with 19 bytes of payload for AES key update
 */
const uint8_t aes_key_final_fragment_array[] = 
{
    // Payload of 27 bytes (16 bytes AES key,3 bytes for TL and 8 byte MAC)
    0x84, 0x38, 0xEC, 0xCC, 0x41, 0x03, 0x7D, 0xD2, 0x8D, 0x5C, 0xB6, 0xA1, 0x1F, 0xFF, 0x6F, 0x2C,
    0x1F, 0x86, 0xF3, 0x30, 0x57, 0x8A, 0xE0, 0xA0, 0x8E, 0x75, 0x3F,
};

/**
* @}
*/
