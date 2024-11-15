import json
import os
from typing import Any, Dict


def load_game_data() -> Dict[str, Any]:
    """
    Загружает данные игры из файла data.json.

    :return: Словарь с данными игры.
    """

    file_dir = os.path.dirname(__file__)
    file_path = os.path.join(file_dir, 'data.json')

    with open(file_path, 'r', encoding='utf-8') as file:
        game_data = json.load(file)

    return game_data


if __name__ == "__main__":
    data = load_game_data()
