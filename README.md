# Blackjack - Uni Projekt 2

## Projektdetails
[Implementationsdetails](https://gitlab.informatik.uni-bonn.de/xx_best_oose_group_xx/projektbeta/-/blob/e2ffaa9738465814f7dee422b9db3b628f47ad4c/docs/Implementation%20Details/implementation_details.drawio.png)

## Server mit Docker starten
Um den Server mit Docker zu starten, muss man Docker auf seinem Gerät konfiguriert haben. Anschließend  müssen die folgenden Commands im `/Server` Verzeichnis ausgeführt werden:


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

Anschließend muss im `/Client` Verzeichnis in der axiosConfig die baseURL auf den Server-host gestellt werden, den man nutzen möchte.

Zum Installieren der Abhängikeiten muss folgender Befehl im `/Client` Verzeichnis ausgeführt werden:

    npm install

Zum Starten des Clients muss folgender Befehl im `/Client` Verzeichnis ausgeführt werden:

    npm start

## Mitglieder

- Ibrahim Fakhir        
- Christian Goerdes    
- Sergej Zuev          

## Aufteilung 
Das Projekt wurde so realisiert, dass Client und Server unabhängig von einander sind. Zum Entwickeln wurde dabei der Server in die Spiellogik und API aufgeteilt. Die drei Bestandteile wurden im wesentlichen wie folgt aufgeteilt:
- Client - Ibo
- API - Chrisi     
- Spiellogik - Sergej
