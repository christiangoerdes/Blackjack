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


def get_dealer():
    dealer = b.getDealer()
    dealer_obj = {
        "name": dealer._name,
        "password": dealer._password,
        "balance": dealer._balance,
        "deck": get_deck_for_player(dealer),
        "bet": dealer._bet,
        "in_round": dealer._in_round
    }
    return dealer_obj


def get_players():
    players = b.getPlayers()
    players_list = []
    for player in players:
        player_obj = {
            "name": player._name,
            "password": player._password,
            "balance": player._balance,
            "deck": get_deck_for_player(player),
            "bet": player._bet,
            "in_round": player._in_round
        }
        players_list.append(player_obj)
    return players_list


def get_deck():
    deck = b.getDeck()
    card_deck = []
    for card in deck:
        card_obj = {
            "suit": card._suit,
            "type": card._type
        }
        card_deck.append(card_obj)
    return card_deck


def get_deck_for_player(player):
    card_deck = []
    for card in player._deck:
        card_obj = {
            "suit": card._suit,
            "type": card._type
        }
        card_deck.append(card_obj)
    return card_deck


# Game State 0: Not Started

@api.get("/")
async def root():
    return {
        "initBalance": b.getInitBalance(),
        "gameState": b.getGameState(),
        "turn": b.getTurn(),
        "minBet": b.getMinBet(),
        "deck": get_deck(),
        "players": get_players(),
        "dealer": get_dealer()
    }

@api.get("/join")
async def join(name: str, password: str):
    b.join(name, password)
    return {
        "players": get_players()
    }


@api.get("/leave")
async def leave(name: str, password: str):
    b.leave(name, password)
    return {
        "players": get_players()
    }


@api.get("/start_round")
async def start_round():
    b.start_round()
    return {
        "status": b.getGameState()
    }

# Game State 1: Placing Bets


@api.get("/place_bet")
async def place_bet(name: str, password: str, bet: int):
    return {
        "status": b.place_bet(name, password, bet)
    }

# Game State 2: Drawing Cards


@api.get("/draw")
async def draw(name: str, password: str):
    return {
        "status": b.draw(name, password)
    }


@api.get("/skip")
async def skip(name: str, password: str):
    return {
        "status": b.skip(name, password)
    }


if __name__ == "__main__":
    this_python_file = os.path.basename(__file__)[:-3]
    instance = uvicorn.run(f"{this_python_file}:api", host="127.0.0.1", port=8000, log_level="info", reload=True)
