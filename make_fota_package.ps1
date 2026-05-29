param(
    [string]$ConfigFile = ".\fota_versions.txt",
    [string]$SliceSize = "20000"
)

Set-StrictMode -Version Latest
$ErrorActionPreference = "Stop"

function Get-ScriptRootPath {
    if ($PSScriptRoot) {
        return $PSScriptRoot
    }

    return (Get-Location).Path
}

function Find-ArtifactInDir {
    param(
        [string]$DirectoryPath,
        [string]$FileName,
        [switch]$Required
    )

    $directPath = Join-Path -Path $DirectoryPath -ChildPath $FileName
    if (Test-Path -LiteralPath $directPath) {
        return (Resolve-Path -LiteralPath $directPath).Path
    }

    $matches = @(Get-ChildItem -LiteralPath $DirectoryPath -File -Recurse -Filter $FileName |
        Where-Object { $_.FullName -notmatch '\\package\\' } |
        Sort-Object FullName)

    if ($matches.Count -gt 0) {
        return $matches[0].FullName
    }

    if ($Required) {
        throw "Missing $FileName under directory: $DirectoryPath"
    }

    return $null
}

function Resolve-ArchiveDir {
    param(
        [string]$ArchiveRoot,
        [string]$AppVersion,
        [string]$TestVersion
    )

    $candidates = @(
        (Join-Path -Path $ArchiveRoot -ChildPath ("App_{0}\Test_{1}" -f $AppVersion, $TestVersion)),
        (Join-Path -Path $ArchiveRoot -ChildPath $AppVersion)
    )

    foreach ($candidate in $candidates) {
        if (Test-Path -LiteralPath $candidate) {
            return (Resolve-Path -LiteralPath $candidate).Path
        }
    }

    throw "Archive directory not found. Tried: $($candidates -join ', ')"
}

function Copy-RequiredFile {
    param(
        [string]$SourcePath,
        [string]$DestinationPath
    )

    if (-not (Test-Path -LiteralPath $SourcePath)) {
        throw "Source file not found: $SourcePath"
    }

    Copy-Item -LiteralPath $SourcePath -Destination $DestinationPath -Force
}

$scriptRoot = Get-ScriptRootPath
$configPath = Join-Path -Path $scriptRoot -ChildPath $ConfigFile
$archiveRoot = Join-Path -Path $scriptRoot -ChildPath "out\image\archive"
$fotaToolDir = Join-Path -Path $scriptRoot -ChildPath "tools\fota_tool"
$outputRoot = Join-Path -Path $scriptRoot -ChildPath "out\fota_packages"

if (-not (Test-Path -LiteralPath $configPath)) {
    throw "Config file not found: $configPath"
}

Invoke-Expression (Get-Content -LiteralPath $configPath -Raw)

foreach ($name in "oldApp", "oldTest", "newApp", "newTest") {
    if (-not (Get-Variable -Name $name -ErrorAction SilentlyContinue)) {
        throw "Missing variable in config: `$$name"
    }
}

$oldDir = Resolve-ArchiveDir -ArchiveRoot $archiveRoot -AppVersion $oldApp -TestVersion $oldTest
$newDir = Resolve-ArchiveDir -ArchiveRoot $archiveRoot -AppVersion $newApp -TestVersion $newTest

$oldSystemImg = Find-ArtifactInDir -DirectoryPath $oldDir -FileName "system.img" -Required
$newSystemImg = Find-ArtifactInDir -DirectoryPath $newDir -FileName "system.img" -Required
$oldUserApp = Find-ArtifactInDir -DirectoryPath $oldDir -FileName "user_app.bin" -Required
$newUserApp = Find-ArtifactInDir -DirectoryPath $newDir -FileName "user_app.bin" -Required

$oldSystemHash = (Get-FileHash -LiteralPath $oldSystemImg -Algorithm MD5).Hash
$newSystemHash = (Get-FileHash -LiteralPath $newSystemImg -Algorithm MD5).Hash
$systemChanged = $oldSystemHash -ne $newSystemHash

$packageTag = "App_{0}_Test_{1}__to__App_{2}_Test_{3}" -f $oldApp, $oldTest, $newApp, $newTest
$outputDir = Join-Path -Path $outputRoot -ChildPath $packageTag
$logPath = Join-Path -Path $outputDir -ChildPath "make_fota_package.log"
$finalPatch = Join-Path -Path $outputDir -ChildPath "system_patch.bin"
$decisionFile = Join-Path -Path $outputDir -ChildPath "decision.txt"

New-Item -ItemType Directory -Path $outputDir -Force | Out-Null

$oldSystemInput = Join-Path -Path $fotaToolDir -ChildPath "system_old.img"
$newSystemInput = Join-Path -Path $fotaToolDir -ChildPath "system_new.img"
$oldUserInput = Join-Path -Path $fotaToolDir -ChildPath "user_app_old.bin"
$newUserInput = Join-Path -Path $fotaToolDir -ChildPath "user_app_new.bin"
$patchOutput = Join-Path -Path $fotaToolDir -ChildPath "system_patch.bin"
$folderA = Join-Path -Path $fotaToolDir -ChildPath "a"
$folderB = Join-Path -Path $fotaToolDir -ChildPath "b"

Copy-RequiredFile -SourcePath $oldSystemImg -DestinationPath $oldSystemInput
Copy-RequiredFile -SourcePath $newSystemImg -DestinationPath $newSystemInput
Copy-RequiredFile -SourcePath $oldUserApp -DestinationPath $oldUserInput
Copy-RequiredFile -SourcePath $newUserApp -DestinationPath $newUserInput
Copy-RequiredFile -SourcePath $newSystemImg -DestinationPath (Join-Path -Path $folderA -ChildPath "system.img")
Copy-RequiredFile -SourcePath $newUserApp -DestinationPath (Join-Path -Path $folderA -ChildPath "user_app.bin")
Copy-RequiredFile -SourcePath $oldSystemImg -DestinationPath (Join-Path -Path $folderB -ChildPath "system.img")
Copy-RequiredFile -SourcePath $oldUserApp -DestinationPath (Join-Path -Path $folderB -ChildPath "user_app.bin")

if (Test-Path -LiteralPath $patchOutput) {
    Remove-Item -LiteralPath $patchOutput -Force
}

$toolOutput = ""
$mode = ""

Push-Location $fotaToolDir
try {
    if ($systemChanged) {
        $mode = "full-diff"
        $command = ".\adiff.exe -p system_old.img system_new.img system_patch.bin -a1 user_app user_app_old.bin user_app_new.bin -l fsall -s $SliceSize"
        $toolOutput = & powershell -NoProfile -Command $command 2>&1 | Out-String
    } else {
        $mode = "app-full-package"
        $command = ".\FBFMake_CF_V1.6-150.exe -o system_patch.bin -f config_app -a a -b a"
        $toolOutput = & powershell -NoProfile -Command $command 2>&1 | Out-String
    }
} finally {
    Pop-Location
}

if (-not (Test-Path -LiteralPath $patchOutput)) {
    throw "system_patch.bin was not generated.`n$toolOutput"
}

Copy-RequiredFile -SourcePath $patchOutput -DestinationPath $finalPatch

@(
    "mode=$mode"
    "oldApp=$oldApp"
    "oldTest=$oldTest"
    "newApp=$newApp"
    "newTest=$newTest"
    "oldSystemImg=$oldSystemImg"
    "newSystemImg=$newSystemImg"
    "oldUserApp=$oldUserApp"
    "newUserApp=$newUserApp"
    "oldSystemHash=$oldSystemHash"
    "newSystemHash=$newSystemHash"
) | Set-Content -LiteralPath $decisionFile -Encoding ASCII

$toolOutput | Set-Content -LiteralPath $logPath -Encoding ASCII

Write-Host ""
Write-Host "================ Package Build Result ================" -ForegroundColor Cyan
Write-Host "Mode            : $mode"
Write-Host "Old version     : App_$oldApp / Test_$oldTest"
Write-Host "New version     : App_$newApp / Test_$newTest"
Write-Host "Output patch    : $finalPatch"
Write-Host "Decision file   : $decisionFile"
Write-Host "Tool log        : $logPath"
Write-Host "======================================================" -ForegroundColor Cyan
Write-Host ""
