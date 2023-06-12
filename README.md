# Blackjack

## Projektdetails
[Implementationsdetails](https://gitlab.informatik.uni-bonn.de/xx_best_oose_group_xx/projektbeta/-/blob/e2ffaa9738465814f7dee422b9db3b628f47ad4c/docs/Implementation%20Details/implementation_details.drawio.png)

## Server mit Docker starten
Um den Server mit Docker zu starten, muss man Docker auf seinem Gerät configuriert haben. Anschließend  müssen die folgenden Commands im `/Server` verzeichnis ausgeführt werden:


Container bauen:

    sudo docker build -t blackjack_api .

Container starten:

    sudo docker run --network host blackjack_api


## Server ohne Docker starten
### Build erstellen:
Um einen Build zu erstellen, muss der folgende Command im `/Server` Verzeichnis des Projekts eingegeben werden:
    
    cmake -S . -B build && cmake --build build && cmake --install build

### API starten:
Um die API zu starten, muss einer der beiden folgenden Commands im `/Server/api` Verzeichnis ausgeführt werden:

    uvicorn api:api --port 8000 --reload
 ###

    python3 -m uvicorn api:api --port 8000  --reload

---
## Client

Um den Client zu nutzen, muss zuerst Node installiert werden.

Im /Client Verzeichnis muss folgender Befehl zum Installieren der Abhängikeiten ausgeführt werden:

    npm install

Im /Client Verzeichnis muss folgender Befehl zum Ausführen des Clients ausgeführt werden:

    npm run

## Mitglieder

- Ibrahim Fakhir
- Christian Goerdes
- Sergej Zuev
