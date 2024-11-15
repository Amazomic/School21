from typing import Any, Dict
from data.load_map import load_map
from data.load_data import load_game_data
from data.load_quests import load_quests

from engine import generator

all_data = {}

def load_all_data() -> Dict[str, Any]:
    """
    Загружает все данные из файлов locations.json и data.json и сохраняет их в глобальной переменной all_data.

    :return: Словарь с загруженными данными.
    """
    global all_data

    if all_data:
        return all_data

    locations = load_map()

    game_data = load_game_data()

    npcs = game_data.get("npcs", [])
    npcs_by_location = {}
    for npc in npcs:
        location_id = str(npc["location_id"])

        if "name" not in npc or not npc["name"]:
            if npc.get("gender") == "female":
                npc["name"] = generator.generate_name_woman()
            else:
                npc["name"] = generator.generate_name()

        npcs_by_location.setdefault(location_id, []).append(npc)

    enemies = game_data.get("enemies", [])
    enemies_by_location = {}
    for enemy in enemies:
        location_id = str(enemy["location_id"])

        if "name" not in enemy or not enemy["name"]:
            enemy["name"] = generator.generate_name_enemies()

        for item in enemy["loot"]:
            if "name" not in item or not item["name"]:
                if item["item_type"] == "weapon":
                    item["name"] = generator.generate_attack_item_name()
                elif item["item_type"] == "armor":
                    item["name"] = generator.generate_defense_item_name()

        enemies_by_location[location_id] = {
            "name": enemy["name"],
            "is_active": enemy["is_active"],
            "stats": {
                "attack": enemy["stats"]["attack"],
                "defense": enemy["stats"]["defense"],
                "hp": enemy["stats"]["hp"]
            },
            "loot": enemy["loot"]
        }


    quests = load_quests()

    all_data = {
        "locations": locations,
        "npcs": npcs_by_location,
        "enemies": enemies_by_location,
        "quests": quests
    }

    return all_data


load_all_data()

if __name__ == "__main__":
    enemies = all_data.get("enemies", {})
    for location_id, npc_list in enemies.items():
        for npc in npc_list:
            print(f"Location ID: {location_id}, NPC Name: {npc.get('name', 'Unknown Name')}")
