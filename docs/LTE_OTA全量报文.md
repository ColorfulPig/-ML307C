# BMS OTA 全量报文
进行bms ota 从064001 升级到062002
## 日志来源

- `lteota.txt`
## 平台下发 下载模组fota升级
平台->LTE: 字段data[0]=0x01
```json原始数据
datastring=e1BaAGsBLDE1MjU3NiwxLjAuMiwxLjAsaHR0cDovL2p4dy5xbWtkLmNuL3FkYmF0dGVyeS9hcHAvZ2V0TW9kdWxlT3RhRmlsZS84LDYzMGU0OTYzODY2NGZkMGRmZDk2NTU1YjJkMGI2YzM0LFpZLBd9
```
```将datastring base解码后的数据
7B 50 5A 00 6B 01 2C 31 35 32 35 37 36 2C 31 2E 30 2E 32 2C 31 2E 30 2C 68 74 74 70 3A 2F 2F 6A 78 77 2E 71 6D 6B 64 2E 63 6E 2F 71 64 62 61 74 74 65 72 79 2F 61 70 70 2F 67 65 74 4D 6F 64 75 6C 65 4F 74 61 46 69 6C 65 2F 38 2C 36 33 30 65 34 39 36 33 38 36 36 34 66 64 30 64 66 64 39 36 35 35 35 62 32 64 30 62 36 63 33 34 2C 5A 59 2C 17 7D
```
```其中字段data内容为：
,152576,1.0.2,1.0,http://jxw.qmkd.cn/qdbattery/app/getModuleOtaFile/8,630e49638664fd0dfd96555b2d0b6c34,ZY,
```
## LTE 接收成功后 返包给平台
```
LTE上报到平台需要重新组包，其中规则为
先把7b 7d去掉，只对中间的内容base64编码完以后，重新包成：7B 31 <base64字符串> 7D；然后ascll形式就是这样了{1xxxxx...}
```
``` text
7B 50 5A 00 02 01 00 09 7D
按照规则组包后为：1UFoAAgEACQ==
```
## 开始下载升级包

## 校验固件大小、MD5