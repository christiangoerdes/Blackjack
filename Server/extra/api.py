try:
    import blackjack
except ImportError as e:
    print(f"Importing the shared library 'hangman' did not work.")
    print(f"Is (a link to) the shared library 'hangman.____.so' in the same directory as this python script?")
    print(f"The import caused the following exception '{e}'")
    print(f"Exiting")
    exit(1)

from blackjack import BlackjackGame
import os
from fastapi import FastAPI
import uvicorn

b = BlackjackGame()

api = FastAPI

@api.get("/")
async def root():
    return {
        "json: " : b.to_json()
    }

if __name__ == "__main__":
    this_python_file = os.path.basename(__file__)[:-3]
    instance = uvicorn.run(f"{this_python_file}:api", host="127.0.0.1", port=8000, log_level="info", reload=True)
