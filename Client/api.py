import requests
import json


def formatted_print(obj):
    text = json.dumps(obj, sort_keys=True, indent=4)
    print(text)


class Api:
    # public methods
    def get_game_data(self, path: str):
        self._get_data(path)


    def join(self, path: str, name: str, password: str):
        api_path = path + "/join?" + "name=" + name + "&" + "password=" + password

        self._get_data(api_path)


    # private methods
    def _get_data(self, path):
        response = requests.get(f"{path}")
        if response.status_code == 200:
            print("successfully fetched the data")
            formatted_print(response.json())
        else:
            print(f"could not fetch data")
