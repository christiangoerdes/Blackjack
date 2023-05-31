# Blackjack

## Build erstellen
Um einen Build zu erstellen, muss das folgende Command im Wurzelverzeichnis des Projekts eingegeben werden:
    
    cmake -S . -B build && cmake --build build && cmake --install build

## API starten
Um die API zu starten, muss einer der beiden folgenden Commands im Server/api Verzeichnis ausgeführt werden:

    uvicorn api:api --port 8000 --reload
 ---

    python3 -m uvicorn api:api --port 8000  --reload

## Client

Dependencies Commands

- sudo apt install python3
- pip install requests

## Mitglieder

- Ibrahim Fakhir
- Christian Goerdes
- Sergej Zuev

## Projektdetails
[Implementationsdetails](https://gitlab.informatik.uni-bonn.de/xx_best_oose_group_xx/projektbeta/-/blob/e2ffaa9738465814f7dee422b9db3b628f47ad4c/docs/Implementation%20Details/implementation_details.drawio.png)