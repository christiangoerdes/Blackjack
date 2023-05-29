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


b = BlackjackGame()

api = FastAPI()


@api.get("/")
async def root():
    return {
        "balance": b.getInitBalance(),
        "gameState": b.getGameState(),
        "players": b.getPlayers(),
        "dealer": b.getDealer()
    }


if __name__ == "__main__":
    this_python_file = os.path.basename(__file__)[:-3]
    instance = uvicorn.run(f"{this_python_file}:api", host="127.0.0.1", port=8000, log_level="info", reload=True)
