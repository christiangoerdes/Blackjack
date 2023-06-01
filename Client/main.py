import requests
import asyncio
from api import Api

PATH = "http://localhost:8000"

game_update = {}

async def poll_game_state(loop):
    response = await loop.run_in_executor(None, requests.get, 'http://localhost:8000')

    global game_update
    game_update = response.json

    await asyncio.sleep(0.5)


def compare_game_state(current_game_update):
    global game_update

    difference = []

    for key, value in current_game_update.items():
        if value != game_update[key]:
            difference.append(key)

    return difference


async def game_logic():
    global game_update

    current_game_update = {"game_state": -2}

    while True:
        difference = compare_game_state(current_game_update)
        current_game_update = game_update

        if current_game_update["game_state"] == 0:
            game_state_0(difference)
        elif current_game_update["game_state"] == 1:
            game_state_1(difference)

        await asyncio.sleep(0.2)


async def main():
    loop = asyncio.get_event_loop()

    tasks = [
        loop.create_task(poll_game_state(loop)),
        loop.create_task(game_logic())
    ]

    await asyncio.gather(*tasks)


def game_state_0(difference):
    pass


def game_state_1(difference):
    pass


if __name__ == "__main__":
    global PATH

    api = Api()

    api.join(PATH, "my name", "my password")
