#============================================================================
# Create Package Common
#     Contains Powershell script to install chocolatey, packages, and
#     at the end of this script installs CoApp and creates a nuget
#     package given a autopkg file
#============================================================================

function Test-ChocolateyInstalled
{
   $chocolateyPath = Join-Path ${Env:ALLUSERSPROFILE} chocolatey

   if ( Test-Path $chocolateyPath )
   {
      return $true
   }

   return $false
}

function Install-Chocolatey
{
   Invoke-Expression ( (New-Object Net.WebClient).DownloadString( "https://chocolatey.org/install.ps1" ) )
}

function Run-Chocolatey
{
   $chocolateyPath = Join-Path ${Env:ALLUSERSPROFILE} "chocolatey\bin\choco.exe"

   . $chocolateyPath $args
}

function Test-PackageInstalled( $packageName )
{
   $results = ( Run-Chocolatey list -lo | Select-String $packageName | Measure-Object -Line ).Lines

   if ( $results -eq 0 )
   {
      return $false
   }

   return $true
}

function Install-Package( $packageName )
{
   if ( !( Test-PackageInstalled $packageName ) )
   {
      Run-Chocolatey install $packageName -source http://nuget.techsmith.com/nuget
   }
}

function Install-PackageGlobal( $packageName )
{
   if ( !( Test-PackageInstalled $packageName ) )
   {
      Run-Chocolatey install $packageName
   }
}

#============================================================================
# Install
#============================================================================

$isChocolateyInstalled = Test-ChocolateyInstalled

if ( $isChocolateyInstalled -eq $false )
{
   Write-Host "Chocolatey Package Manager not found. Installing..."

   Install-Chocolatey
}

# Now, Install coapp and create the nuget package based on a autopkg file

# Write-Host "autopkgFile is specified as $($args[0])"

$autopkgFile = "$($args[0])";

Install-PackageGlobal coapp

Write-NugetPackage "$autopkgFile"

popd
