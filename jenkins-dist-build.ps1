# set name
if ([string]::IsNullOrEmpty($Env:NAME)) {
  $Env:NAME="phreeqci"
}

# set date
if ([string]::IsNullOrEmpty($Env:DATE)) {
  $Env:DATE = date +%x
}
${Env:RELEASE_DATE} = date -d ${Env:DATE} "+%B %e, %G"

# set ver
${PPK}="${Env:PUBLIC}\rsa-key-20151119.ppk"
if ([string]::IsNullOrEmpty($Env:VER)) {
  $SRC_FILE=(plink -i ${PPK} charlton@parkplace `
            "cd ftp/phreeqci; ls -t phreeqci-*-*.msi | awk '{if (NR == 1) {print}}'")
  $v = ($SRC_FILE -replace "^phreeqci-", "" -replace "-.*msi$", "") -split "\."
  if ([string]::IsNullOrEmpty($v[2])) {
    $v[2] = 0
  }
  $v[2] = 1 + $v[2]
  $Env:ver_major = $v[0]
  $Env:ver_minor = $v[1]
  $Env:ver_patch = $v[2]
  $Env:VER = $v -join "."
}
else {
  $v = ($Env:VER) -split "\."  
  $Env:ver_major = $v[0]
  $Env:ver_minor = $v[1]
  $Env:ver_patch = $v[2]
}
# set HEAD
[string]$HEAD=(-split (svn --config-dir C:\Users\jenkins\svn-jenkins st -v phreeqci2.sln))[0]
if ([string]::IsNullOrEmpty($Env:REL) -or $Env:REL.CompareTo('HEAD') -eq 0) {
  $Env:REL = $HEAD
}

# set environmental variables
${Env:VER_TAG}="r$Env:REL"
${Env:VER_NUMTAG}="-$Env:REL"
${Env:VERSION_LONG}="$Env:ver_major.$Env:ver_minor.$Env:ver_patch.$Env:REL"
${Env:VER_UC}="$Env:ver_major.$Env:ver_minor.$Env:ver_patch.$Env:REL"
${Env:MSI_VERSION}="$Env:ver_major.$Env:ver_minor.$Env:REL"
${Env:FULLPKG}="$Env:NAME-$Env:VER-$Env:REL"
${Env:DOWNLOADS}="${Env:PUBLIC}\Downloads"

# display environmental variables
Write-Output "HEAD=$HEAD"
Write-Output "Env:DATE=$Env:DATE"
Write-Output "Env:RELEASE_DATE=$Env:RELEASE_DATE"
Write-Output "Env:ver_major=$Env:ver_major"
Write-Output "Env:ver_major=$Env:ver_minor"
Write-Output "Env:ver_major=$Env:ver_patch"
Write-Output "Env:VER=$Env:VER"
Write-Output "Env:REL=$Env:REL"
Write-Output "Env:VER_TAG=$Env:VER_TAG"
Write-Output "Env:VER_NUMTAG=$Env:VER_NUMTAG"
Write-Output "Env:VERSION_LONG=$Env:VERSION_LONG"
Write-Output "Env:MSI_VERSION=$Env:MSI_VERSION"
Write-Output "Env:FULLPKG=$Env:FULLPKG"
Write-Output "Env:DOWNLOADS=${Env:DOWNLOADS}"

# duplicate build/dist.sh (sed)
$sed_files=@('build/phreeqci_version.h', `
             'phreeqc3/doc/README.PhreeqcI.TXT', `
             'phreeqc3/doc/RELEASE.TXT')

foreach ($file in $sed_files) {
  (Get-Content $file) | Foreach-Object {
    $_ -replace "(#define *PHREEQC_VER_MAJOR\s*)[0-9]*",        "`${1}$Env:ver_major" `
       -replace "(#define *PHREEQC_VER_MINOR\s*)[0-9]*",        "`${1}$Env:ver_minor" `
       -replace "(#define *PHREEQC_VER_NUMTAG\s*)[0-9]*",       "`${1}$Env:VER_NUMTAG" `
       -replace "(#define *PHREEQC_VER_PATCH\s*)[0-9]*",        "`${1}$Env:ver_patch" `
       -replace "(#define *PHREEQC_VER_REVISION\s*)[0-9]*",     "`${1}$Env:REL" `
       -replace "(#define *PHREEQC_VER_TAG\s*)[0-9]*",          "`${1}$Env:VER_TAG" `
       -replace "(#define *PHREEQCI_VER_MAJOR\s*)[0-9]*",       "`${1}$Env:ver_major" `
       -replace "(#define *PHREEQCI_VER_MINOR\s*)[0-9]*",       "`${1}$Env:ver_minor" `
       -replace "(#define *PHREEQCI_VER_NUMTAG\s*)[0-9]*",      "`${1}$Env:VER_NUMTAG" `
       -replace "(#define *PHREEQCI_VER_PATCH\s*)[0-9]*",       "`${1}$Env:ver_patch" `
       -replace "(#define *PHREEQCI_VER_REVISION\s*)[0-9]*",    "`${1}$Env:REL" `
       -replace "(#define *PHREEQCI_VER_TAG\s*)[0-9]*",         "`${1}$Env:VER_TAG" `
       -replace "@PHREEQC_DATE@",                               "$Env:RELEASE_DATE" `
       -replace "@PHREEQC_VER@",                                "$Env:VER" `
       -replace "@REL@",                                        "$Env:REL" `
       -replace "@RELEASE_DATE@",                               "$Env:RELEASE_DATE" `
       -replace "@REVISION@",                                   "$Env:REL" `
       -replace "@VER@",                                        "$Env:VER" `
       -replace "@VERSION@",                                    "$Env:VER" `
       -replace "@VERSION_LONG@",                               "$Env:VERSION_LONG" `
       -replace "@VER_DATE@",                                   "$Env:RELEASE_DATE" `
       -replace "@VER_UC@",                                     "$Env:VER_UC" `
  } | Set-Content $file
}

# build phreeqci.exe
[string]${MsBuild}="c:\WINDOWS\Microsoft.NET\Framework\v2.0.50727\MsBuild.exe"
[string]$opts="phreeqci2.sln /t:phreeqci2 /p:Configuration=CLRRelease /p:Platform=Win32"
Invoke-Expression "$MsBuild $opts"

# touch these files
Get-ChildItem .\phreeqc3\database\*| where {!$_.PSIsContainer} | %{$_.LastWriteTime = Get-Date -Date "$Env:DATE"}
Get-ChildItem .\fs-031-02\*.pdf    | where {!$_.PSIsContainer} | %{$_.LastWriteTime = Get-Date -Date "$Env:DATE"}
Get-ChildItem .\phreeqc3\doc\*.pdf | where {!$_.PSIsContainer} | %{$_.LastWriteTime = Get-Date -Date "$Env:DATE"}
Get-ChildItem .\Help\*.chm         | where {!$_.PSIsContainer} | %{$_.LastWriteTime = Get-Date -Date "$Env:DATE"}
Get-ChildItem .\phreeqc3\examples\*| where {!$_.PSIsContainer} | %{$_.LastWriteTime = Get-Date -Date "$Env:DATE"}
Get-ChildItem .\phreeqci.eng       | where {!$_.PSIsContainer} | %{$_.LastWriteTime = Get-Date -Date "$Env:DATE"}
Get-ChildItem .\CLRRelease\*.exe   | where {!$_.PSIsContainer} | %{$_.LastWriteTime = Get-Date -Date "$Env:DATE"}

# build MSI
[string]$opts_msi="phreeqci2.sln /t:msi /p:Configuration=CLRRelease /p:Platform=Win32 /p:TargetName=$Env:FULLPKG /p:Major=$Env:ver_major /p:Minor=$Env:ver_minor /p:Patch=$Env:ver_patch /p:Build=$Env:REL /p:ExampleDir=examples"
Invoke-Expression "$MsBuild $opts_msi"
