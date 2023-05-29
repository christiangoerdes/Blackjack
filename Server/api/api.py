# Run with
#   uvicorn api:api --port 8000 --reload
# or, if uvicorn is not in PATH, run as
#   python3 -m uvicorn api:api --port 8000  --reload

try:
    import blackjack
except ImportError as e:
    print(f"Importing the shared library 'blackjack' did not work.")
    print(f"Is (a link to) the shared library 'blackjack.____.so' in the same directory as this python script?")
    print(f"The import caused the following exception '{e}'")
    print(f"Exiting")
    exit(1)

import os
from fastapi import FastAPI
import uvicorn
from blackjack import BlackjackGame
from fastapi.encoders import jsonable_encoder
from fastapi.responses import JSONResponse
from pydantic import BaseModel
import json


b = BlackjackGame()

api = FastAPI()


class PlayerModel(BaseModel):
    _name: str
    _password: str
    _balance: int
    _deck: list
    _bet: int
    _in_round: bool


@api.get("/")
async def root():
    dealer = b.getDealer()
    dealer_obj = {
        "name": dealer._name,
        "password": dealer._password,
        "balance": dealer._balance,
        "deck": [card.serialize() for card in dealer._deck],
        "bet": dealer._bet,
        "in_round": dealer._in_round
    }

    return {
        "initBalance": b.getInitBalance(),
        "gameState": b.getGameState(),
        "turn": b.getTurn(),
        "minBet": b.getMinBet(),
        "deck": b.getDeck(),
        "players": b.getPlayers(),
        "dealer": dealer_obj
    }

@api.get("/start_round")
async def start_round():
    b.start_round()
    return {
        "status": b.getGameState()
    }


@api.get("/foo")
async def get_player():
    player = blackjack.Player()  # Example player object
    try:
        # Convert the player object to a dictionary using the custom encoder
        player_dict = jsonable_encoder(player, by_alias=True)
        return JSONResponse(content=player_dict)
    except Exception as x:
        return JSONResponse(content={"error": str(x)})


if __name__ == "__main__":
    this_python_file = os.path.basename(__file__)[:-3]
    instance = uvicorn.run(f"{this_python_file}:api", host="127.0.0.1", port=8000, log_level="info", reload=True)
