[简体中文](./README.zh_CN.md)
# OTP-Client

This is a simple OTP client that can be used to generate OTPs for a given secret key. The client supports both HOTP and TOTP algorithms.

The HOTP algorithm is based on [RFC 4226](https://tools.ietf.org/html/rfc4226) and the TOTP algorithm is based on [RFC 6238](https://tools.ietf.org/html/rfc6238).

These algorithms are used to generate OTPs for two-factor authentication. this repository is used to build a demo client for manage OTP secret key and generate OTP code.

Except RFC 4226 and RFC 6238, this repository also based on [RFC 2104](https://tools.ietf.org/html/rfc2104) for HMAC algorithm and [RFC 3174](https://tools.ietf.org/html/rfc3174) for SHA1 algorithm, you can see the detail in the source code. All of supported algorithm will listed below.

| Algorithm | Standard based on | Since Version | Release Date |
|:---------:|:-----------------:|:-------------:|:------------:|
|  SHA-1(Default)(Not Recommend)| FIPS 180-1, RFC 3174 |  v0.0.0.1-xp  |    2025-05-28  |
|  SHA-2 Family(Include SHA256, SHA-224, SHA-384, SHA-512)  |   FIPS 180-4      |    v0.0.0.1-xp| 2025-05-28      |
|  SHA-3 Family(Include SHA3-256, SHA3-224, SHA3-384, SHA3-512)| FIPS 202 | v0.0.0.1-xp | 2025-05-28 |
|  SM3       | GB/T 32905-2016 | v0.0.0.1-xp | 2025-05-28 |


This demo client also using SNTP v4 protocal to check whether the time is correct. we are using [RFC 5905](https://tools.ietf.org/html/rfc4226) to implements the SNTP v4 protocal, you can see the detail in the source code. If you will see the demo of the SNTP v4, you can see my other reposities:
  - [NettimesyncTool](https://github.com/neville-studio/nettimesynctool)
  - [NettimesyncTool-winform](https://github.com/neville-studio/nettimesynctool-winform)

This project is developing, the source code will not release yet.

Unless I have completed the code and applied the Software Copyright License, I will not release the source code.

This project was started on 2025-03-14.

# When will release the first version?
The first version has done on 2025-04-13, but I am getting the something important prosedures, this version cannot release yet. I will release after I have completed the prosedures.

# System Requirement
As the first version, The System will requires Windows XP, at least Service Pack 3.

Without any critical Bug, the first version is the last version which supports Windows XP, Service Pack 3.

Windows Software Requirement: 

|OS|Software|version|Release Date|Link|
|-|-|-|-|-|
|Windows XP Service Pack 3|Microsoft Visual C++ Redistributable 2015-2019| 16.7| 2020-08-05|[here](https://my.visualstudio.com/Downloads?q=Visual%20C++%20Redistributable%20for%20Visual%20Studio%202019%20(version%2016.7))|
|Windows Vista Service Pack 2|Microsoft Visual C++ Redistributable 2015-2019| 16.11| 2021-08-10|[here](https://my.visualstudio.com/Downloads?q=Visual%20C++%20Redistributable%20for%20Visual%20Studio%202019%20(version%2016.11))|
|Windows 7 Service Pack 1 or Later|Microsoft Visual C++ Redistributable 2015-2022|Latest| N/A|[x86](https://aka.ms/vs/17/release/vc_redist.x86.exe) [x64](https://aka.ms/vs/17/release/vc_redist.x64.exe) [arm64](https://aka.ms/vs/17/release/vc_redist.arm64.exe)|

P.S. This software is only for Windows yet. 
- Linux-based OS: No support plan.
- MacOS: No support plan.

# Support Plan && System Requirement
Microsoft has announced some of the OS has end of its life. It would be better for you to upgrade your OS to the latest version. (At OS End of life date column, the information from [Microsoft Product Lifecycle](https://learn.microsoft.com/lifecycle/products/).) But we will provide a support plan for the system requirement.

| Operating System Version | Highest Software version |OS End of life Date | Support Until |
|:-:|:-:|:-:|:-:|
|Windows XP Service Pack 3|v0.0.0.1-XP|2014/4/8| 2025-05-28 |
|Windows Vista Service Pack 2|N/A|2017/4/11|At least 2026/1/10|
|Windows 7 Service Pack 1|N/A|2020/1/14, ESU 2023/1/10|At least 2026/1/10|
|Windows 8.1 |N/A|2023/1/10|At least 2026/10/13|
|Windows 10 |N/A|2025/10/14, ESU 2028/10/11|At least 2032/1/13|
|Windows 10 LTSB 2015 |N/A|2025/10/14|At least 2025/10/14|
|Windows 10 LTSB 2016 |N/A|2027/1/12|At least 2027/1/12|
|Windows 10 LTSC 2019 |N/A|2029/1/9|At least 2029/1/9|
|Windows 10 LTSC 2021 |N/A|2032/1/13|At least 2032/1/13|
|Windows 11 |N/A|No|Not yet|
|Windows 11 LTSC 2024|N/A|2034/10/10|At lease 2034/10/10|


# Thanks

## Thanks the repositorys and projects
- [nlohmann-json](https://github.com/nlohmann/json), for using JSON Library. this library is used MIT License. [@nlohmann](https://github.com/nlohmann) this project is using version 3.12.0 to process JSON data.

## Thanks the AI Generator
- **Github Copilot**, by *Github*, for providing the code suggestion. this project is using the AI code suggestion to generate the code.
- **Qwen**, *by Aliyun*, for providing the suggestion of the project.
- **Deepseek**, by *Deepseek*, for providing the core suggestion of the project.
