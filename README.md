# OTP-Client

This is a simple OTP client that can be used to generate OTPs for a given secret key. The client supports both HOTP and TOTP algorithms.

The HOTP algorithm is based on [RFC 4226](https://tools.ietf.org/html/rfc4226) and the TOTP algorithm is based on [RFC 6238](https://tools.ietf.org/html/rfc6238).

These algorithms are used to generate OTPs for two-factor authentication. this repository is used to build a demo client for manage OTP secret key and generate OTP code.

Except RFC 4226 and RFC 6238, this repository also based on [RFC 2104](https://tools.ietf.org/html/rfc2104) for HMAC algorithm and [RFC 3174](https://tools.ietf.org/html/rfc3174) for SHA1 algorithm, you can see the detail in the source code.

This demo client also using SNTP v4 protocal to check whether the time is correct. we are using [RFC 5905](https://tools.ietf.org/html/rfc4226) to implements the SNTP v4 protocal, you can see the detail in the source code. If you will see the demo of the SNTP v4, you can see my other reposities:
  - [NettimesyncTool](https://github.com/neville-studio/nettimesynctool)
  - [NettimesyncTool-winform](https://github.com/neville-studio/nettimesynctool-winform)

This project is developing, the source code will not release yet.

We will using MIT Licence to share the code to the remote.

Unless I have completed the code and applied the Software Copyright License, I will not release the source code.

This project was started on 2025-03-14


# Usage
The app is only for Windows currently.



# Thanks

## Thanks the repositorys and projects
- [nlohmann-json](https://github.com/nlohmann/json), for using JSON Library. this library is used MIT License. [@nlohmann](https://github.com/nlohmann) this project is using version 3.13.3 to process JSON data.

## Thanks the AI Generator
- **Github Copilot**, by *Github*, for providing the code suggestion. this project is using the AI code suggestion to generate the code.
- **Qwen**, *by Aliyun*, for providing the suggestion of the project.
- **Deepseek**, by *Deepseek*, for providing the core suggestion of the project.