import json
import os


def load_map():
    """
    Загружает данные о локациях из файла locations.json.

    :return: Словарь с данными о локациях.
    """    
    file_dir = os.path.dirname(__file__)
    file_path = os.path.join(file_dir, 'locations.json')

    with open(file_path, 'r') as f:
        return json.load(f)


if __name__ == "__main__":
    locations = load_map()
    print(locations)
