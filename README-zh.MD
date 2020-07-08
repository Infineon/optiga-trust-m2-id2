[EN](./README.md) | 中文

# OPTIGA&trade; Trust M2 ID2 软件框架

## 快速导航

* [安全芯片](#安全芯片)
  * [描述](#描述)
  * [摘要](#摘要)
  * [关键特性及优势](#关键特性及优势)
* [快速上手](#快速上手)
  * [评估工具](#评估工具)
  * [软件框架一览](#软件框架一览)
* [文档](#文档)
  * [PCB板设计指南](#PCB板设计指南)
* [贡献代码](#贡献代码)


## 安全芯片

<img src="https://github.com/Infineon/Assets/raw/master/Pictures/lowres-OPTIGA%20Trust%20M2%20ID2%2010-2%20plain.tif.png" width="300" style="float:right">

### 描述

该资源库包含OPTIGA™Trust M2 ID2安全芯片的跨平台的软件框架。它是其它应用指南的基础。

### 摘要

OPTIGA™Trust M2 ID2是一种基于安全微控制器的安全解决方案。设备可以按不同的需求设定不同的配置项。设备中包含一个满足设备唯一性的AES对称密钥和一个设备ID(ALI ID2预分配)。OPTIGA™ Trust M2 ID2可以轻松集成到阿里云物联网应用中。

### 关键特性及优势

* High-end级安全芯片
* CC EAL6+ (high)认证的硬件
* 交匙钥式的整体解决方案
* 10kB 用户存储空间
* PG-USON-10-2 封装 (3 x 3 mm)
* 温度范围 (−40°C to +105°C)
* 带有线路加密（Shielded Connection）功能的I2C 通讯接口
* 加密算法支持: 
  * RSA® 最高2048位长度密钥
  * AES 最高256位长度密钥 , HMAC 最高可支持SHA512
  * TLS v1.2 PRF 以及 HKDF算法最高可支持 SHA512
* 加密工具包中支持 SHA-256, RSA® , AES, HMAC 和 密钥分散算法
* 支持阿里云物联网应用
* 可配置的设备安全监控器, 4 个单向计数器
* 受保护(完整性 与/或 私密性) 的数据，密钥，元组等对象的更新
* 零电流消耗的冬眠模式
* 满足面向工业自动化及基础设施应用的15至20年长效生命周期  

### OPTIGA&trade; Trust M 功能对照表

<table class="tg">
  <tr>
    <th class="tg-fymr">功能</th>
    <th class="tg-fymr">支持的曲线/算法</th>
    <th class="tg-fymr">API 指令</th>
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
    <td class="tg-c3ow" rowspan="2">受保护的数据/对象的完整性保障更新</td>
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
    <td class="tg-c3ow">受保护的数据/密钥/元组更新 (完整性 与/或 私密性)</td>
    <td class="tg-0pky">ECC NIST P256/384/521</br>ECC Brainpool P256/384/512 r1</br>RSA® 1024/2048</br>Signature scheme as ECDSA FIPS 186-3/RSA SSA PKCS#1 v1.5 without hashing</td>
    <td class="tg-0pky"></td>
    <td class="tg-0pky"> </td>
    <th class="tg-0pky">✓</th>
    <td class="tg-0pky">✓</td>
  </tr>
</table>

## 快速上手

### 评估工具

[阿里云物联网OPTIGA™ Trust M2 ID2](https://github.com/Infineon/alios-things-optiga-trust-m)应用指南说明如何开始使用评估工具包。

### 软件框架一览

![](https://github.com/Infineon/Assets/raw/master/Pictures/optiga_trust_m_system_block_diagram_v2id2.png)

## 文档

关于文档的概要描述和一些关键摘要，请参考[Wiki页面](https://github.com/Infineon/optiga-trust-m2-id2/wiki)

其余可下载的PDF文档可从如下路径中获得:
1. [OPTIGA Trust M2 ID2 V2 Datasheet v2.00](documents/OPTIGA_Trust_M2_ID2_V2_Datasheet_v2.00.pdf) (PDF)
2. [OPTIGA Trust M2 ID2 V2 Solution Reference Manual v2.00](documents/OPTIGA_Trust_M2_ID2_V2_Solution_Reference_Manual_v2.00.pdf) (PDF)
4. [Infineon I2C protocol specification v2.02](documents/Infineon_I2C_Protocol_v2.02.pdf) (PDF)

### PCB板设计指南

如果你计划集成OPTIGA™ Trust M至 PCB设计中，建议参考[点击这里(外部链接,在当前标签页中打开)](https://www.infineon.com/dgdl/Infineon-Recommendations_for_Board_Assembly_xQFN-P-v01_00-EN.pdf?fileId=5546d462580663ef015806ab383a05bf)。

## <a name="contributing"></a>贡献代码

请参阅 [CONTRIBUTING.md](CONTRIBUTING.md)以了解我们的行为准则，以及如何向我们提交“合并”申请的相关流程。

## <a name="license"></a>License
该项目遵循MIT License - 详见 [LICENSE](LICENSE) 中的相关细节
