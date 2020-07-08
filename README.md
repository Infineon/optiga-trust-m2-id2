EN | [中文](./README-zh.md)

# OPTIGA&trade; Trust M2 ID2 Software Framework

## Quick navigation

* [Security Chip](#security-chip)
  * [Description](#description)
  * [Summary](#summary)
  * [Key Features and Benefits](#key-features-and-benefits)
* [Get Started](#Get-Started)
  * [Evaluation kit](#Evaluation-kit)
  * [Software Framework overview](#Software-Framework-overview)
* [Documentation](#Documentation)
  * [Board assembly recommendations](#Board-assembly-recommendations)
* [Contributing](#Contributing)


## Security Chip

<img src="https://github.com/Infineon/Assets/raw/master/Pictures/lowres-OPTIGA%20Trust%20M2%20ID2%2010-2%20plain.tif.png" width="300" style="float:right">

### Description

This repository contains a target-agnostic Software Framework for the OPTIGA™ Trust M2 ID2 security chip. It is a base for other application notes.

### Summary

The OPTIGA™ Trust M2 ID2 is a security solution based on a secure microntroller. devices might come provisioned on demand. The generic device contains a unique AES (ALI ID2 specific) symmetric key and a device ID. OPTIGA™ Trust M2 ID2 enables easy integration in Alibaba Cloud IoT.

### Key Features and Benefits

* High-end security controller
* Common Criteria Certified EAL6+ (high) hardware
* Turnkey solution
* Up to 10kB user memory
* PG-USON-10-2 package (3 x 3 mm)
* Temperature range (−40°C to +105°C)
* I2C interface with Shielded Connection (encrypted communication)
* Cryptographic support: 
  * RSA® up to 2048
  * AES key up to 256 , HMAC up to SHA512
  * TLS v1.2 PRF and HKDF up to SHA512
* Crypto ToolBox commands for SHA-256, RSA® Feature, AES, HMAC and Key derivation
* Alibaba Cloud IoT connectivity
* Configurable device security monitor, 4 Monotonic up counters
* Protected(integrity and confidentiality) update of data, key and metadata objects
* Hibernate for zero power consumption
* Lifetime for Industrial Automation and Infrastructure is 20 years and 15 years for other Application Profiles  

### OPTIGA&trade; Trust M  feature comparison table

<table class="tg">
  <tr>
    <th class="tg-fymr">Features</th>
    <th class="tg-fymr">Supported Curve/Algorithm</th>
    <th class="tg-fymr">API Command</th>
    <th class="tg-fymr">V1</th>
    <th class="tg-fymr">M2 ID2</th>
    <th class="tg-fymr">V3</th>
  </tr>
  <tr>
    <td class="tg-c3ow" rowspan="2">ECC</td>
    <td class="tg-0pky">ECC NIST P256/384 </td>
    <td class="tg-0pky"></td>
    <td class="tg-0pky">✓</td>
    <td class="tg-0pky"> </td>
    <td class="tg-0pky">✓</td>
  </tr>
  <tr>
    <td class="tg-0pky">ECC NIST P521, ECC Brainpool P256/384/512 r1</td>
    <td class="tg-0pky"></td>
    <td class="tg-0pky"> </td>
    <td class="tg-0pky"> </td>
    <td class="tg-0pky">✓</td>
  </tr>
  <tr>
    <td class="tg-c3ow">RSA</td>
    <td class="tg-0pky">RSA® 1024/2048 </td>
    <td class="tg-0pky"></td>
    <td class="tg-0pky">✓</td>
    <th class="tg-0pky">✓</th>
    <td class="tg-0pky">✓</td>
  </tr>
  <tr>
    <td class="tg-c3ow" rowspan="3">Key Derivation</td>
    <td class="tg-0pky">TLS v1.2 PRF SHA 256 </td>
    <td class="tg-0pky"></td>
    <td class="tg-0pky">✓</td>
    <th class="tg-0pky">✓</th>
    <td class="tg-0pky">✓</td>
  </tr>
  <tr>
    <td class="tg-0pky">TLS v1.2 PRF SHA 384/512 </td>
    <td class="tg-0pky"></td>
    <td class="tg-0pky"> </td>
    <th class="tg-0pky">✓</th>
    <td class="tg-0pky">✓</td>
  </tr>
  <tr>
    <td class="tg-0pky">HKDF SHA-256/384/512  </td>
    <td class="tg-0pky"></td>
    <td class="tg-0pky"> </td>
    <th class="tg-0pky">✓</th>
    <td class="tg-0pky">✓</td>
  </tr>
  <tr>
    <td class="tg-c3ow">AES</td>
    <td class="tg-0pky">Key size - 128/192/256 (ECB, CBC, CBC-MAC, CMAC)</td>
    <td class="tg-0pky"></td>
    <td class="tg-0pky"> </td>
    <th class="tg-0pky">✓</th>
    <td class="tg-0pky">✓</td>
  </tr>
  <tr>
    <td class="tg-c3ow">Random Generation</td>
    <td class="tg-0pky">TRNG, DRNG, Pre-Master secret for RSA® Key exchange</td>
    <td class="tg-0pky"></td>
    <td class="tg-0pky">✓</td>
    <th class="tg-0pky">✓</th>
    <td class="tg-0pky">✓</td>
  </tr>
  <tr>
    <td class="tg-c3ow">HMAC</td>
    <td class="tg-0pky">HMAC with SHA256/384/512 </td>
    <td class="tg-0pky"></td>
    <td class="tg-0pky"> </td>
    <th class="tg-0pky">✓</th>
    <td class="tg-0pky">✓</td>
  </tr>
  <tr>
    <td class="tg-c3ow">Hash </td>
    <td class="tg-0pky">SHA256 </td>
    <td class="tg-0pky"></td>
    <td class="tg-0pky">✓</td>
    <th class="tg-0pky">✓</th>
    <td class="tg-0pky">✓</td>
  </tr>
  <tr>
    <td class="tg-c3ow" rowspan="2">Protected data (object) update (Integrity)</td>
    <td class="tg-0pky">ECC NIST P256/384</br>RSA® 1024/2048 </br>Signature scheme as ECDSA FIPS 186-3/RSA SSA PKCS#1 v1.5 without hashing </td>
    <td class="tg-0pky"></td>
    <td class="tg-0pky">✓</td>
    <th class="tg-0pky">✓</th> 
    <td class="tg-0pky">✓</td>
  </tr>
  <tr>
    <td class="tg-c3ow">ECC NIST P521,</br>ECC Brainpool P256/384/512 r1</br>Signature scheme as ECDSA FIPS 186-3/RSA SSA PKCS#1 v1.5 without hashing</td>
    <td class="tg-0pky"></td>
    <td class="tg-0pky"> </td>
    <th class="tg-0pky">✓</th> 
    <td class="tg-0pky">✓</td>
  </tr>
  <tr>
    <td class="tg-c3ow">Protected Data/key/metadata update (Integrity and/or confidentiality)</td>
    <td class="tg-0pky">ECC NIST P256/384/521</br>ECC Brainpool P256/384/512 r1</br>RSA® 1024/2048</br>Signature scheme as ECDSA FIPS 186-3/RSA SSA PKCS#1 v1.5 without hashing</td>
    <td class="tg-0pky"></td>
    <td class="tg-0pky"> </td>
    <th class="tg-0pky">✓</th>
    <td class="tg-0pky">✓</td>
  </tr>
</table>

## Get Started

### Evaluation kit

The [Alibaba Cloud IoT with OPTIGA™ Trust M2 ID2](https://github.com/Infineon/alios-things-optiga-trust-m) Application Note shows how to start working with the evaluation kit.

### Software Framework overview

![](https://github.com/Infineon/Assets/raw/master/Pictures/optiga_trust_m_system_block_diagram_v2id2.png)

## Documentation

For high level description and some important excerpts from the documentation please refer to [Wiki page](https://github.com/Infineon/optiga-trust-m2-id2/wiki)

Other downloadable PDF documents can be found below:
1. [OPTIGA Trust M2 ID2 V2 Datasheet v2.00](documents/OPTIGA_Trust_M2_ID2_V2_Datasheet_v2.00.pdf) (PDF)
2. [OPTIGA Trust M2 ID2 V2 Solution Reference Manual v2.00](documents/OPTIGA_Trust_M2_ID2_V2_Solution_Reference_Manual_v2.00.pdf) (PDF)
4. [Infineon I2C protocol specification v2.02](documents/Infineon_I2C_Protocol_v2.02.pdf) (PDF)

### Board assembly recommendations

If you are planning to integrate OPTIGA™ Trust M in your PCB design have a look at the recommendations found [here (external, opens in the same tab)](https://www.infineon.com/dgdl/Infineon-Recommendations_for_Board_Assembly_xQFN-P-v01_00-EN.pdf?fileId=5546d462580663ef015806ab383a05bf).

## <a name="contributing"></a>Contributing

Please read [CONTRIBUTING.md](CONTRIBUTING.md) for details on our code of conduct, and the process for submitting pull requests to us.

## <a name="license"></a>License
This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details
