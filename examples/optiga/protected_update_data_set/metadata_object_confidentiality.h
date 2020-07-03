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
* \file metadata_object_confidentiality.h
*
* \brief   This file provides the data set for protected update of metadata object with confidentiality.
*
* \ingroup grOptigaExamples
*
* @{
*/

/**
 * Manifest for metadata update with integrity protected and confidentiality
 */
const uint8_t manifest_metadata[] = 
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
            0x9B, 
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
                0x21,
                // Payload Length 
                0x0D,
                // Trust Payload version
                0x03, 
                // Trust Add info data 
                0x82, 
                    // content reset
                    0x00,
                    // additional flag
                    0x00,
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
                                        0x8A, 0x7A, 0x10, 0x3A, 0xD3, 0xB4, 0xB7, 0xA9,
                                        0x48, 0x5B, 0x80, 0x1C, 0x7E, 0xD4, 0xD5, 0xAE,
                                        0xD7, 0x8C, 0xDD, 0xB2, 0x02, 0x9A, 0xA8, 0xC8,
                                        0x11, 0x90, 0xF0, 0xA8, 0x10, 0xC7, 0x14, 0xD9,
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
                                                        0x8B, 0xE5, 0xD9, 0xF4, 0xEE, 0xA5, 0x5D, 0xD6,
                                                        0xCF, 0xBE, 0x65, 0x1B, 0x38, 0x99, 0x48, 0x23,
                                                        0x22, 0xA7, 0x47, 0x95, 0x1E, 0x88, 0x3E, 0x72, 
                                                        0x3B, 0xCE, 0xB3, 0x11, 0x71, 0xCE, 0x31, 0xB6,
                                                        0x4F, 0xF0, 0xD1, 0x39, 0xEB, 0xFA, 0x79, 0x73,
                                                        0x3C, 0x18, 0x55, 0xDD, 0x18, 0x45, 0x32, 0x57,
                                                        0xB5, 0x29, 0xEE, 0xDD, 0x5A, 0x44, 0x8A, 0x12,
                                                        0xA2, 0xFB, 0xA8, 0x1A, 0xAE, 0x5B, 0x59, 0x91, 
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
                    0xE0, 0xE2, 
        // Signature info, byte string of single byte length
        0x58, 
            // Byte string length for RSA 1024
            0x80, 
            // Signature data
            0x3A, 0x9D, 0x55, 0x74, 0xD3, 0xC4, 0x8A, 0x23, 0xD2, 0x12, 0xED, 0xE8, 0xE3, 0x25, 0xC0, 0x49, 
            0x06, 0xD8, 0x3C, 0xB5, 0x97, 0x9D, 0x4A, 0xFB, 0x64, 0xAE, 0x84, 0x82, 0x90, 0x39, 0xC0, 0x62, 
            0xC5, 0xB3, 0xAD, 0x52, 0xDA, 0x01, 0x97, 0xE7, 0xE6, 0x81, 0xD8, 0x44, 0xB3, 0x56, 0x52, 0xC8, 
            0x5F, 0x2A, 0x2E, 0x76, 0x31, 0x05, 0xA6, 0x27, 0xBE, 0xCB, 0x77, 0xE8, 0xCA, 0x0E, 0xFE, 0xA7,
            0xEC, 0xF0, 0xA5, 0xD1, 0xF6, 0xAA, 0xF1, 0x14, 0x6E, 0x3C, 0x84, 0x0B, 0x3A, 0xE1, 0xB7, 0xB5,
            0xC7, 0x58, 0xD1, 0xD2, 0x7D, 0x3D, 0x94, 0x54, 0x78, 0xB7, 0xEC, 0x02, 0xF4, 0x4D, 0x10, 0x4E,
            0xAD, 0xC8, 0x78, 0x05, 0xE1, 0x42, 0xC6, 0xD4, 0xB1, 0x0B, 0x76, 0x37, 0xC2, 0x2D, 0xF6, 0x8E, 
            0x73, 0xB1, 0x00, 0x41, 0xB4, 0xE8, 0xEA, 0x0A, 0xA8, 0x19, 0xAC, 0x43, 0x4D, 0x6A, 0x5D, 0x39,

};

/**
 * Fragment array 
 */
const uint8_t metadata_final_fragment_array[] = 
{
    0x37, 0x7F, 0x65, 0xC4, 0xE8, 0x39, 0x61, 0x1D, 0xAC, 0x37, 0x67, 0x6F, 0x9E, 0x1D, 0x62, 0x50,
    0xA2, 0x7A, 0x3E, 0xF1, 0x5B,
};

/**
* @}
*/
