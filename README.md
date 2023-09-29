# Blackjack

## Project Details
[Implementationsdetails](https://github.com/christiangoerdes/Blackjack/blob/adbfea407bc6ebaf35a4ef6756ab3f50e6e726c8/docs/Implementation%20Details/implementation_details.drawio.png)

## Server
### Starting the Server with Docker
To start the server using Docker, you need to have Docker configured on your device. Afterward, execute the following commands in the `/Server` directory:


Build the container:
```
sudo docker build -t blackjack_api .
```
Start the container:
```
sudo docker run --network host blackjack_api
```

### Starting the Server without Docker
#### Creating a Build:
To create a build, enter the following command in the `/Server` directory of the project:
    
    cmake -S . -B build && cmake --build build && cmake --install build

#### Starting the API:
To start the API, execute one of the following commands in the `/Server/api` directory:
```
    uvicorn api:api --port 8000 --reload
```
```
    python3 -m uvicorn api:api --port 8000  --reload
```
---
## Client

To use the client, you first need to install Node.js.

Next, in the axiosConfig file located in the `/Client directory`, set the baseURL to the server host you want to use.

To install dependencies, run the following command in the `/Client` directory:
```
npm install
```
To start the client, run the following command in the `/Client` directory:
```
npm start
```
