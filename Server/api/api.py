# Run with
#   uvicorn api:api --port 8000 --reload
# or, if uvicorn is not in PATH, run as
#   python3 -m uvicorn api:api --port 8000  --reload

try:
    import blackjack
except ImportError as e:
    print(f"Importing the shared library 'blackjack_module' did not work.")
    print(f"Is (a link to) the shared library 'blackjack.cpython-310-x86_64-linux-gnu.so' in the 'api' directory?")
    print(f"The import caused the following exception: {e}")
    print(f"Exiting")
    exit(1)

import os
from fastapi import FastAPI
import uvicorn
from blackjack import BlackjackGame
from fastapi.middleware.cors import CORSMiddleware

# initialize game
b = BlackjackGame()

# initialize API
api = FastAPI()

# Enable access from other devices
api.add_middleware(
    CORSMiddleware,
    allow_origins="*",
    allow_credentials=True,
    allow_methods=["*"],
    allow_headers=["*"],
)


# returns the list of all players
def get_players():
    players = b.getPlayers()
    players_list = []
    for player in players:
        player_obj = {
            "name": player._name,
            "balance": player._balance,
            "deck": get_deck_for_player(player),
            "bet": player._bet,
            "in_round": player._in_round,
            "deck_value": player._deck_value
        }
        players_list.append(player_obj)
    return players_list


# returns the deck
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


# returns the deck for a certain player
def get_deck_for_player(player):
    card_deck = []
    for card in player._deck:
        card_obj = {
            "suit": card._suit,
            "type": card._type
        }
        card_deck.append(card_obj)
    return card_deck


# returns the dealer
def get_dealer():
    return {
        "deck": get_deck_for_player(b.getDealer()),
        "deck_value": b.getDealer()._deck_value
    }


# Root query to get the current game-state
@api.get("/")
async def root():
    return {
        "initBalance": b.getInitBalance(),
        "gameState": b.getGameState(),
        "turn": b.getTurn(),
        "minBet": b.getMinBet(),
        "players": get_players(),
        "dealer": get_dealer()
    }


# Game State 0: Not Started

# query to join the game
@api.get("/join")
async def join(name: str, password: str):
    return {
        "success": b.join(name, password)
    }


# query to leave the game
@api.get("/leave")
async def leave(name: str, password: str):
    return {
        "success": b.leave(name, password)
    }


# query to start a round
@api.get("/start_round")
async def start_round():
    return {
        "success": b.start_round()
    }


# Game State 1: Placing Bets

# query to place a bet
@api.get("/place_bet")
async def place_bet(name: str, password: str, bet: int):
    return {
        "success": b.place_bet(name, password, bet)
    }


# Game State 2: Drawing Cards

# query to draw a card
@api.get("/draw")
async def draw(name: str, password: str):
    return {
        "status": b.draw(name, password)
    }


# query to skip drawing a card
@api.get("/skip")
async def skip(name: str, password: str):
    return {
        "status": b.skip(name, password)
    }


if __name__ == "__main__":
    this_python_file = os.path.basename(__file__)[:-3]
    instance = uvicorn.run(f"{this_python_file}:api", host="127.0.0.1", port=8000, log_level="info", reload=True)
