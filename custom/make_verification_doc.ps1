$ErrorActionPreference = 'Stop'

Add-Type -AssemblyName System.IO.Compression.FileSystem
Add-Type -AssemblyName System.Web

$outPath = Join-Path (Get-Location) '第一阶段基础功能验证表.docx'
$tempRoot = Join-Path (Get-Location) ('docx_build_' + [Guid]::NewGuid().ToString('N'))

function XmlEscape([string]$text) {
    if ($null -eq $text) { return '' }
    return [System.Security.SecurityElement]::Escape($text)
}

function Para([string]$text, [string]$style = '') {
    $escaped = XmlEscape $text
    if ($style) {
        return "<w:p><w:pPr><w:pStyle w:val=`"$style`"/></w:pPr><w:r><w:t>$escaped</w:t></w:r></w:p>"
    }
    return "<w:p><w:r><w:t>$escaped</w:t></w:r></w:p>"
}

function Table($headers, $rows) {
    $xml = @"
<w:tbl>
<w:tblPr>
<w:tblStyle w:val="TableGrid"/>
<w:tblW w:w="0" w:type="auto"/>
<w:tblBorders>
<w:top w:val="single" w:sz="4" w:space="0" w:color="666666"/>
<w:left w:val="single" w:sz="4" w:space="0" w:color="666666"/>
<w:bottom w:val="single" w:sz="4" w:space="0" w:color="666666"/>
<w:right w:val="single" w:sz="4" w:space="0" w:color="666666"/>
<w:insideH w:val="single" w:sz="4" w:space="0" w:color="666666"/>
<w:insideV w:val="single" w:sz="4" w:space="0" w:color="666666"/>
</w:tblBorders>
</w:tblPr>
"@
    $xml += '<w:tr>'
    foreach ($h in $headers) {
        $xml += '<w:tc><w:tcPr><w:shd w:fill="D9EAF7"/></w:tcPr><w:p><w:r><w:rPr><w:b/></w:rPr><w:t>' + (XmlEscape $h) + '</w:t></w:r></w:p></w:tc>'
    }
    $xml += '</w:tr>'
    foreach ($row in $rows) {
        $xml += '<w:tr>'
        foreach ($cell in $row) {
            $xml += '<w:tc><w:p><w:r><w:t>' + (XmlEscape $cell) + '</w:t></w:r></w:p></w:tc>'
        }
        $xml += '</w:tr>'
    }
    $xml += '</w:tbl>'
    return $xml
}

$infoRows = @(
    @('固件版本', ''),
    @('源码版本/提交号', ''),
    @('SDK版本', 'ML307C-GC-CN_1.0.1'),
    @('模组型号', 'ML307C-GC-CN'),
    @('测试板/硬件版本', ''),
    @('SIM卡运营商', '中国移动'),
    @('测试人员', ''),
    @('测试日期', ''),
    @('测试地点', '')
)

$summaryRows = @(
    @('用例总数', '30'),
    @('通过', ''),
    @('不通过', ''),
    @('阻塞', ''),
    @('待验证', ''),
    @('遗留问题数', '')
)

$baseRows = @(
    @('BF-001', '强启', '有用', '需要验证', '4G/蓝牙端基础功能'),
    @('BF-002', '季节切换', '有用', '需要验证', '4G/蓝牙端基础功能'),
    @('BF-003', '加热模式', '有用', '需要验证', '4G/蓝牙端基础功能'),
    @('BF-004', '休眠模式', '有用', '需要验证', '4G/蓝牙端基础功能'),
    @('BF-005', '保温模式', '有用', '需要验证', '4G/蓝牙端基础功能'),
    @('BF-006', 'OTA', '没用', '暂不验证', '如后续恢复OTA能力，再进入工程化阶段验证'),
    @('BF-007', '锁定', '待确认', '待确认', '需确认当前项目是否继续使用'),
    @('BF-008', '清空总充总放', '有用', '需要验证', '4G/蓝牙端基础功能'),
    @('BF-009', 'GPS定位', '没用', '暂不验证', '当前项目不启用'),
    @('BF-010', '单双包切换', '待确认', '待确认', '需确认当前项目是否继续使用及切换规则')
)

$caseRows = @(
    @('FV-001','资料接收','外包源码、固件、文档完整性检查','已收到外包交付资料','核对源码、固件、编译说明、烧录工具、接口说明、历史问题记录','资料齐全，能对应当前项目版本','','',''),
    @('FV-002','环境搭建','开发环境安装验证','PC已安装编译工具链和SDK','按环境搭建说明安装工具链、配置路径和环境变量','环境安装无报错，工具链命令可正常执行','','',''),
    @('FV-003','环境搭建','工程导入验证','已完成开发环境安装','打开/导入外包工程，检查工程目录、依赖库、配置文件','工程可正常打开，无缺失关键文件或依赖','','',''),
    @('FV-004','编译','全量编译验证','工程已导入','执行一次 clean 后全量编译','编译通过，生成目标固件文件','','',''),
    @('FV-005','编译','重复编译稳定性验证','已完成一次成功编译','连续编译3次，记录编译耗时和告警','每次均可成功生成固件，告警可解释','','',''),
    @('FV-006','烧录','烧录工具连接验证','模组已连接PC，串口/下载口正常','打开烧录工具，识别端口和模组','工具能识别设备，端口状态正常','','',''),
    @('FV-007','烧录','固件烧录验证','已生成固件文件','按烧录流程下载固件到模组','烧录完成且无失败提示，模组可重新启动','','',''),
    @('FV-008','启动','模组启动日志验证','固件已烧录','上电启动，使用串口工具查看启动日志','启动日志完整，无异常复位、死机、严重错误','','',''),
    @('FV-009','串口','调试串口通信验证','串口工具已连接','配置正确波特率，观察日志或发送基础指令','串口收发正常，日志可读，无乱码','','',''),
    @('FV-010','参数','基础参数读取验证','系统已启动','读取设备号、服务器地址、端口、上报周期等关键参数','参数可读取，默认值与文档/代码一致','','',''),
    @('FV-011','SIM卡','SIM卡识别验证','已插入有效SIM卡','上电后查看SIM卡识别状态','SIM卡可识别，无卡/锁卡/异常状态可明确提示','','',''),
    @('FV-012','网络','4G网络注册验证','SIM卡正常，天线已连接','启动后观察网络注册流程和状态','模组可完成入网注册，注册状态稳定','','',''),
    @('FV-013','网络','信号强度读取验证','已完成网络注册','读取或观察信号强度值','可获取信号强度，数值处于可通信范围','','',''),
    @('FV-014','网络','PDP/数据链路建立验证','已完成网络注册','观察拨号、PDP激活或数据链路建立过程','数据链路建立成功，可进行IP通信','','',''),
    @('FV-015','通信','服务器连接验证','数据链路已建立，服务器地址已配置','启动业务连接，观察TCP/MQTT连接日志','能成功连接服务器，无持续连接失败','','',''),
    @('FV-016','通信','心跳/保活验证','服务器连接成功','观察一个或多个心跳周期','心跳按周期发送，服务器响应或连接状态正常','','',''),
    @('FV-017','数据上报','基础数据上报验证','服务器连接成功','触发或等待一次数据上报','数据能成功发送，平台/服务器侧可收到','','',''),
    @('FV-018','数据上报','连续上报验证','基础上报成功','连续运行30分钟，记录上报次数和失败次数','上报周期稳定，无明显丢包、卡死、异常重启','','',''),
    @('FV-019','恢复','断网后恢复验证','设备正在正常上报','断开天线或制造短时断网，恢复网络后观察','设备能重新注册网络并恢复连接/上报','','',''),
    @('FV-020','稳定性','基础运行稳定性验证','固件已烧录，网络和服务器正常','连续运行2小时以上，观察日志、连接、上报状态','运行期间无死机、异常复位、长时间不上报','','',''),
    @('FV-021','4G/蓝牙端','强启功能验证','设备已上电，4G/蓝牙通信正常','通过4G或蓝牙下发强启指令，观察设备执行状态和反馈','设备进入强启状态，状态反馈正确，日志无异常','','',''),
    @('FV-022','4G/蓝牙端','季节切换功能验证','设备已上电，4G/蓝牙通信正常','下发季节切换指令，分别切换到支持的季节模式','设备模式切换成功，参数/状态反馈与目标季节一致','','',''),
    @('FV-023','4G/蓝牙端','加热模式功能验证','设备已上电，具备进入加热模式条件','下发加热模式指令，观察控制状态和上报状态','设备进入加热模式，状态上报正确，无异常退出','','',''),
    @('FV-024','4G/蓝牙端','休眠模式功能验证','设备已上电，具备进入休眠条件','下发休眠模式指令，观察设备功耗/状态/通信表现','设备进入休眠模式，状态反馈正确，可按设计唤醒','','',''),
    @('FV-025','4G/蓝牙端','保温模式功能验证','设备已上电，具备进入保温条件','下发保温模式指令，观察控制状态和上报状态','设备进入保温模式，状态上报正确，控制逻辑正常','','',''),
    @('FV-026','4G/蓝牙端','清空总充总放功能验证','设备存在总充/总放累计数据','下发清空总充总放指令，读取清空后的累计值','总充/总放累计值按设计清零或重置，状态反馈正确','','',''),
    @('FV-027','4G/蓝牙端','锁定功能确认','设备已上电，4G/蓝牙通信正常','确认当前项目是否保留锁定功能；如保留，下发锁定/解锁指令验证','功能范围明确；如启用，锁定/解锁状态反馈正确','','','待确认是否有用'),
    @('FV-028','4G/蓝牙端','单双包切换功能确认','设备已上电，4G/蓝牙通信正常','确认当前项目是否保留单双包切换；如保留，按规则切换并观察状态','功能范围明确；如启用，单双包状态切换和反馈正确','','','待确认是否有用'),
    @('FV-029','4G/蓝牙端','OTA功能排除确认','当前项目不启用OTA','检查菜单/指令/配置中是否仍暴露OTA入口','当前版本不验证OTA；如入口仍存在，记录风险','','','当前标记为没用'),
    @('FV-030','4G/蓝牙端','GPS定位功能排除确认','当前项目不启用GPS定位','检查菜单/指令/配置中是否仍暴露GPS定位入口','当前版本不验证GPS定位；如入口仍存在，记录风险','','','当前标记为没用')
)

$problemRows = @(
    @('BUG-001','','','','','','',''),
    @('BUG-002','','','','','','',''),
    @('BUG-003','','','','','','','')
)

$conclusionRows = @(
    @('是否完成环境复现',''),
    @('是否完成固件编译',''),
    @('是否完成固件烧录',''),
    @('基础功能是否可运行',''),
    @('是否进入第二阶段',''),
    @('遗留风险','')
)

$body = ''
$body += Para '第一阶段基础功能验证表' 'Title'
$body += Para '项目：ML307C-GC-CN 模组工程版本接手与环境复现'
$body += Para '阶段：阶段1：项目接手与环境复现'
$body += Para '周期：2026.4.28 - 2026.5.14'
$body += Para '目标：确保系统“能跑起来 + 能看懂”'
$body += Para '一、验证信息' 'Heading1'
$body += Table @('项目','内容') $infoRows
$body += Para '二、结论汇总' 'Heading1'
$body += Table @('统计项','数量') $summaryRows
$body += Para '三、4G/蓝牙端基础功能清单' 'Heading1'
$body += Table @('编号','功能项','当前项目状态','验证处理','备注') $baseRows
$body += Para '四、功能验证明细' 'Heading1'
$body += Table @('编号','模块','验证项','前置条件','操作步骤','预期结果','实测结果','结论','问题编号/备注') $caseRows
$body += Para '五、问题记录' 'Heading1'
$body += Table @('问题编号','关联用例','问题现象','复现步骤','日志/截图','严重程度','当前状态','责任人','备注') $problemRows
$body += Para '六、结论' 'Heading1'
$body += Table @('结论项','说明') $conclusionRows

$documentXml = @"
<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
<w:document xmlns:w="http://schemas.openxmlformats.org/wordprocessingml/2006/main">
<w:body>
$body
<w:sectPr>
<w:pgSz w:w="16838" w:h="11906" w:orient="landscape"/>
<w:pgMar w:top="720" w:right="720" w:bottom="720" w:left="720" w:header="360" w:footer="360" w:gutter="0"/>
</w:sectPr>
</w:body>
</w:document>
"@

$stylesXml = @"
<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
<w:styles xmlns:w="http://schemas.openxmlformats.org/wordprocessingml/2006/main">
<w:style w:type="paragraph" w:default="1" w:styleId="Normal"><w:name w:val="Normal"/><w:rPr><w:rFonts w:ascii="Microsoft YaHei" w:eastAsia="Microsoft YaHei" w:hAnsi="Microsoft YaHei"/><w:sz w:val="18"/></w:rPr></w:style>
<w:style w:type="paragraph" w:styleId="Title"><w:name w:val="Title"/><w:pPr><w:jc w:val="center"/></w:pPr><w:rPr><w:rFonts w:ascii="Microsoft YaHei" w:eastAsia="Microsoft YaHei" w:hAnsi="Microsoft YaHei"/><w:b/><w:sz w:val="36"/></w:rPr></w:style>
<w:style w:type="paragraph" w:styleId="Heading1"><w:name w:val="heading 1"/><w:rPr><w:rFonts w:ascii="Microsoft YaHei" w:eastAsia="Microsoft YaHei" w:hAnsi="Microsoft YaHei"/><w:b/><w:sz w:val="24"/></w:rPr></w:style>
<w:style w:type="table" w:styleId="TableGrid"><w:name w:val="Table Grid"/><w:tblPr><w:tblBorders><w:top w:val="single" w:sz="4" w:space="0" w:color="666666"/><w:left w:val="single" w:sz="4" w:space="0" w:color="666666"/><w:bottom w:val="single" w:sz="4" w:space="0" w:color="666666"/><w:right w:val="single" w:sz="4" w:space="0" w:color="666666"/><w:insideH w:val="single" w:sz="4" w:space="0" w:color="666666"/><w:insideV w:val="single" w:sz="4" w:space="0" w:color="666666"/></w:tblBorders></w:tblPr></w:style>
</w:styles>
"@

$contentTypes = @"
<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
<Types xmlns="http://schemas.openxmlformats.org/package/2006/content-types">
<Default Extension="rels" ContentType="application/vnd.openxmlformats-package.relationships+xml"/>
<Default Extension="xml" ContentType="application/xml"/>
<Override PartName="/word/document.xml" ContentType="application/vnd.openxmlformats-officedocument.wordprocessingml.document.main+xml"/>
<Override PartName="/word/styles.xml" ContentType="application/vnd.openxmlformats-officedocument.wordprocessingml.styles+xml"/>
</Types>
"@

$rels = @"
<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
<Relationships xmlns="http://schemas.openxmlformats.org/package/2006/relationships">
<Relationship Id="rId1" Type="http://schemas.openxmlformats.org/officeDocument/2006/relationships/officeDocument" Target="word/document.xml"/>
</Relationships>
"@

$docRels = @"
<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
<Relationships xmlns="http://schemas.openxmlformats.org/package/2006/relationships">
<Relationship Id="rId1" Type="http://schemas.openxmlformats.org/officeDocument/2006/relationships/styles" Target="styles.xml"/>
</Relationships>
"@

New-Item -ItemType Directory -Path (Join-Path $tempRoot '_rels') | Out-Null
New-Item -ItemType Directory -Path (Join-Path $tempRoot 'word') | Out-Null
New-Item -ItemType Directory -Path (Join-Path $tempRoot 'word\_rels') | Out-Null

[System.IO.File]::WriteAllText((Join-Path $tempRoot '[Content_Types].xml'), $contentTypes, [System.Text.Encoding]::UTF8)
[System.IO.File]::WriteAllText((Join-Path $tempRoot '_rels\.rels'), $rels, [System.Text.Encoding]::UTF8)
[System.IO.File]::WriteAllText((Join-Path $tempRoot 'word\document.xml'), $documentXml, [System.Text.Encoding]::UTF8)
[System.IO.File]::WriteAllText((Join-Path $tempRoot 'word\styles.xml'), $stylesXml, [System.Text.Encoding]::UTF8)
[System.IO.File]::WriteAllText((Join-Path $tempRoot 'word\_rels\document.xml.rels'), $docRels, [System.Text.Encoding]::UTF8)

if (Test-Path -LiteralPath $outPath) {
    Remove-Item -LiteralPath $outPath -Force
}

[System.IO.Compression.ZipFile]::CreateFromDirectory($tempRoot, $outPath)
Remove-Item -LiteralPath $tempRoot -Recurse -Force

Write-Output $outPath
