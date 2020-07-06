# OPTIGA&trade; Trust M2 ID2 Software Framework

## Quick navigation

* [Security Chip](#security-chip)
  * [Description](#description)
  * [Summary](#summary)
  * [Key Features and Benefits](#key-features-and-benefits)
* [Get Started](#Get-Started)
  * [Evaluation kit](#Evaluation-kit)
  * [Software Framework overview](#Software-Framework-overview)
  * [Developement kits](#Developement-kits)
* [Documentation](#Documentation)
  * [Usefull articles](#Usefull-articles)
  * [Board assembly recommendations](#Board-assembly-recommendations)
  * [Contributing](#Contributing)


## Security Chip

<img src="https://github.com/Infineon/Assets/raw/master/Pictures/lowres-OPTIGA%20Trust%20M2%20ID2%2010-2%20plain.tif.png" width="300" >

### Description

This repository contains a target-agnostic Software Framework for the OPTIGA™ Trust M2 ID2 security chip. It is a base for other application notes.

### Summary

The OPTIGA™ Trust M2 ID2 is a security solution based on a secure microntroller. devices might come provisioned on demand. The generic device contains a unique AES (ALI ID2 specific) symmetric key and a device ID. OPTIGA™ Trust M2 ID2 enables easy integration in Alibaba Cloud.

### Key Features and Benefits

### OPTIGA&trade; Trust M features table

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

### Software Framework overview

### Developement kits

## Documentation

### Usefull articles

### Board assembly recommendations

If you are planning to integrate OPTIGA™ Trust M in your PCB design have a look at the recommendations found [here (external, opens in the same tab)](https://www.infineon.com/dgdl/Infineon-Recommendations_for_Board_Assembly_xQFN-P-v01_00-EN.pdf?fileId=5546d462580663ef015806ab383a05bf).

## <a name="contributing"></a>Contributing

Please read [CONTRIBUTING.md](CONTRIBUTING.md) for details on our code of conduct, and the process for submitting pull requests to us.

## <a name="license"></a>License
This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details
