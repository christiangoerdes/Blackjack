import asyncio
from api import Api

global api_path
PATH = "http://localhost:8000"


if __name__ == "__main__":
    global PATH

    api = Api()

    # api._get_data(api_path)
    # api._get_data(api_path + "/join?name=ibo&password=iboo")
    # api._get_data(api_path)

    api.join(PATH, "my name", "my password")
