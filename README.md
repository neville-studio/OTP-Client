# OTP-Client

This is a simple OTP client that can be used to generate OTPs for a given secret key. The client supports both HOTP and TOTP algorithms.

The HOTP algorithm is based on [RFC 4226](https://tools.ietf.org/html/rfc4226) and the TOTP algorithm is based on [RFC 6238](https://tools.ietf.org/html/rfc6238).

These algorithms are used to generate OTPs for two-factor authentication. this repository is used to build a demo client for manage OTP secret key and generate OTP code.

Except RFC 4226 and RFC 6238, this repository also based on [RFC 2104](https://tools.ietf.org/html/rfc2104) for HMAC algorithm and [RFC 3174](https://tools.ietf.org/html/rfc3174) for SHA1 algorithm, you can see the detail in the source code.

This demo client also using SNTP v4 protocal to check whether the time is correct. we are using [RFC 5905](https://tools.ietf.org/html/rfc4226) to implements the SNTP v4 protocal, you can see the detail in the source code. If you will see the demo of the SNTP v4, you can see my other reposities:
  - [NettimesyncTool](https://github.com/neville-studio/nettimesynctool)
  - [NettimesyncTool-winform](https://github.com/neville-studio/nettimesynctool-winform)

# Usage
The app is only for Windows currently.



# Thanks to the following repositories for the inspiration:

- [nlohmann-json](https://github.com/nlohmann/json), for using JSON Library. this library is used MIT License. [@nlohmann](https://github.com/nlohmann) this project is using version 3.13.3 to process JSON data.