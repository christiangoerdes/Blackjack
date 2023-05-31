import requests
import asyncio
from api import Api

global api_path
PATH = "http://localhost:8000"


async def poll_game_state(loop):
    response = await loop.run_in_executor(None, requests.get, 'http://localhost:8000')

    game_state = response.json

    await asyncio.sleep(0.2)


async def game_logic():
    while True:
        await asyncio.sleep(0.1)


async def main():
    loop = asyncio.get_event_loop()

    tasks = [
        loop.create_task(poll_game_state(loop)),
        loop.create_task(game_logic())
    ]

    await asyncio.gather(*tasks)

if __name__ == "__main__":
    global PATH

    api = Api()

    api.join(PATH, "my name", "my password")
