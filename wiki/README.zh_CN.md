[English](./README.md)

# 一次性密码客户端（OTP-Client）

这是一个简单的一次性密码（OTP）客户端，可用于为给定的密钥生成一次性密码。该客户端支持 HOTP 和 TOTP 两种算法。

HOTP 算法基于 [RFC 4226](https://tools.ietf.org/html/rfc4226)，TOTP 算法基于 [RFC 6238](https://tools.ietf.org/html/rfc6238)。

这些算法常用于两步验证。本仓库用于构建一个用于管理 OTP 密钥和生成 OTP 验证码的演示客户端。

除了 RFC 4226 和 RFC 6238，本仓库还基于 [RFC 2104](https://tools.ietf.org/html/rfc2104) 的 HMAC 算法和 [RFC 3174](https://tools.ietf.org/html/rfc3174) 的 SHA1 算法，详细内容可见源代码。所有支持的算法在此表中列出。

| 算法 | 基于标准 | 开始支持版本 | 发行时间 |
|:---------:|:-----------------:|:-------------:|:------------:|
|  SHA-1 (默认) (不推荐)| FIPS 180-1, RFC 3174 |  v0.0.0.1-xp  |    2025/5/28   |
|  SHA-2 家族(包括 SHA256, SHA-224, SHA-384, SHA-512)  |   FIPS 180-4      |    v0.0.0.1-xp| 2025/5/28      |
|  SHA-3 家族(包括 SHA3-256, SHA3-224, SHA3-384, SHA3-512)| FIPS 202 | v0.0.0.1-xp | 2025/5/28 |
|  SM3       | GB/T 32905-2016 | v0.0.0.1-xp | 2025/5/28  |

本演示客户端还使用 SNTP v4 协议校准时间，基于 [RFC 5905](https://tools.ietf.org/html/rfc4226) 实现 SNTP v4 协议，具体细节可见源代码。如果你想了解 SNTP v4 的演示，可以参考我的其他仓库：
  - [NettimesyncTool](https://github.com/neville-studio/nettimesynctool)
  - [NettimesyncTool-winform](https://github.com/neville-studio/nettimesynctool-winform)

本项目正在开发中，源代码暂未发布。

在我完成代码并申请软件著作权之前，源代码不会发布。

本项目开始于 2025-03-14。

# 何时发布第一个版本？
第一个版本已于 2025-04-13 完成，但由于正在办理一些重要手续，暂时无法发布。待手续完成后会发布。

# 系统要求
首个版本要求 Windows XP，至少 Service Pack 3。

如无重大 Bug，首个版本将是最后一个支持 Windows XP SP3 的版本。

Windows 软件要求：

|操作系统|软件|版本|发布日期|链接|
|-|-|-|-|-|
|Windows XP Service Pack 3|Microsoft Visual C++ Redistributable 2015-2019|16.7|2020-08-05|[点此下载](https://my.visualstudio.com/Downloads?q=Visual%20C++%20Redistributable%20for%20Visual%20Studio%202019%20(version%2016.7))|
|Windows Vista Service Pack 2|Microsoft Visual C++ Redistributable 2015-2019|16.11|2021-08-10|[点此下载](https://my.visualstudio.com/Downloads?q=Visual%20C++%20Redistributable%20for%20Visual%20Studio%202019%20(version%2016.11))|
|Windows 7 Service Pack 1 及以上|Microsoft Visual C++ Redistributable 2015-2022|最新版|N/A|[x86](https://aka.ms/vs/17/release/vc_redist.x86.exe) [x64](https://aka.ms/vs/17/release/vc_redist.x64.exe) [arm64](https://aka.ms/vs/17/release/vc_redist.arm64.exe)|

P.S. 本软件目前仅支持 Windows。
- Linux 系统：暂无支持计划。
- MacOS：暂无支持计划。

# 支持计划与系统要求
微软已宣布部分操作系统停止支持。建议您升级至最新操作系统。（操作系统生命周期信息来自 [Microsoft Product Lifecycle](https://learn.microsoft.com/lifecycle/products/)。）我们将根据系统要求提供支持计划。

|操作系统版本|最高支持软件版本|操作系统生命周期结束日期|支持截止日期|
|:-:|:-:|:-:|:-:|
|Windows XP Service Pack 3|v0.0.0.1-XP|2014/4/8|2025/5/28|
|Windows Vista Service Pack 2|N/A|2017/4/11|至少 2026/1/10|
|Windows 7 Service Pack 1|N/A|2020/1/14，ESU 2023/1/10|至少 2026/1/10|
|Windows 8.1|N/A|2023/1/10|至少 2026/10/13|
|Windows 10|N/A|2025/10/14，ESU 2028/10/11|至少 2032/1/13|
|Windows 10 LTSB 2015|N/A|2025/10/14|至少 2025/10/14|
|Windows 10 LTSB 2016|N/A|2027/1/12|至少 2027/1/12|
|Windows 10 LTSC 2019|N/A|2029/1/9|至少 2029/1/9|
|Windows 10 LTSC 2021|N/A|2032/1/13|至少 2032/1/13|
|Windows 11|N/A|无|尚未确定|
|Windows 11 LTSC 2024|N/A|2034/10/10|至少 2034/10/10|

# 鸣谢

## 感谢以下仓库和项目
- [nlohmann-json](https://github.com/nlohmann/json)，用于处理 JSON 的库，采用 MIT 许可证。[@nlohmann](https://github.com/nlohmann) 本项目使用 3.12.0 版本处理 JSON 数据。

## 感谢 AI 生成器
- **Github Copilot**，由 *Github* 提供代码建议，本项目使用 AI 代码建议生成代码。
- **Qwen**，*阿里云* 提供项目建议。
- **Deepseek**，*Deepseek* 提供项目核心建议。