echo Installing Chocolatey
powershell -Command "& {Set-ExecutionPolicy Bypass -Scope Process -Force; [System.Net.ServicePointManager]::SecurityProtocol = [System.Net.ServicePointManager]::SecurityProtocol -bor 3072; iex ((New-Object System.Net.WebClient).DownloadString('https://chocolatey.org/install.ps1'))}

echo Installing python3
powershell -Command "choco install python -y"

echo Installing pip
powershell -Command "choco install pip"

echo Installing PyQt
powershell -Command "pip install PyQt5"

echo Installing websocket_client
powershell -Command "pip install websocket_client"

echo Clone smart car repository
powershell -Command "git clone https://github.com/SHBnik/Autism-Car.git"