# http  

## **http功能介绍**

‌HTTP协议‌，全称为Hypertext Transfer Protocol，即超文本传输协议，是互联网上应用最为广泛的一种网络传输协议。它定义了客户端（如Web浏览器）与服务器之间的通信规范，主要用于传输超文本（如HTML文档）以及其他类型的数据（如图片、视频、文件等）。

## **实现功能**
本APP实现http/https的post/get操作示例
1. 使用http(s) POST在onenet物联网开放平台创建设备；
2. 使用http(s) GET在onenet物联网开放平台查询设备详情。


## **APP执行流程**

1. 进入http测试线程，然后等待模组pdp激活；
2. 进入cm_http_post_onenet_reg_dev函数，实现在onenet物联网平台产品下新增设备：首先计算鉴权信息，然后根据http类型创建http实例，配置http参数，设置http header，使用imei作为新增设备的名称封装http body内容，再发起http同步请求，最后将http返回的结果解析打印出来。
3. 进入cm_http_post_onenet_inquire_dev函数，实现查询onenet物联网平台产品设备：首先计算鉴权信息，然后根据http类型创建http实例，配置http参数，设置http header，封装http url中的参数信息，同样使用imei作为设备名称，再发起http同步请求，最后将http返回的结果解析打印出来。

## **使用说明**
- 支持的模组（子）型号：ML307R-DC
- 支持的SDK版本：ML307R OpenCPU SDK 2.0.0版本及其后续版本
- 是否需要外设支撑：无
- 使用注意事项：  
    1. 修改HTTP_TEST_TYPE宏定义可测试http或者https;
    2. 测试本示例前请在onenet物联网开放平台上创建产品，并修改示例中ONENET_PRODUCT_ID宏定义为产品id、ONENET_PRODUCT_ACCESSKEY为产品access_key；
    3. onenet物联网开放平台鉴权信息计算、新增设备、查询设备详细信息和返回结果的code码含义等的信息具体请查看onenet文档：https://open.iot.10086.cn/doc/v5/fuse/detail/1464。

## **FAQ（非必要，视客户/FAE咨询情况增列）**

- 无

## **版本更新说明**

### **1.0.0版本**
- 发布时间：2024/11/15 10:01
- 修改记录：
  1. 初版


--------------------------------------------------------------------------------