from typing import Dict, List, Optional, Union
from engine.item import Item

class Quest:
    """
    Класс, представляющий квест в игре.

    :param description: Описание квеста.
    :param requirements: Требования для выполнения квеста.
    :param rewards: Награды за выполнение квеста.
    :param is_completed: Флаг, указывающий, выполнен ли квест.
    :param quest_id: Уникальный идентификатор квеста.
    :param is_active: Флаг, указывающий, активен ли квест. По умолчанию False.
    :param loot: Список предметов, которые можно получить за выполнение квеста. По умолчанию None.
    :param message: Сообщение, которое выводится при выполнении квеста. По умолчанию None.
    """

    def __init__(self, description: str, requirements: Dict[str, any], rewards: Dict[str, int],
                 is_completed: bool, quest_id: int, is_active: bool = False,
                 loot: Optional[List[Dict[str, any]]] = None, message: Optional[str] = None):
        """
        Инициализация квеста.

        :param description: Описание квеста.
        :param requirements: Требования для выполнения квеста.
        :param rewards: Награды за выполнение квеста.
        :param is_completed: Флаг, указывающий, выполнен ли квест.
        :param quest_id: Уникальный идентификатор квеста.
        :param is_active: Флаг, указывающий, активен ли квест. По умолчанию False.
        :param loot: Список предметов, которые можно получить за выполнение квеста. По умолчанию None.
        :param message: Сообщение, которое выводится при выполнении квеста. По умолчанию None.
        """
        self.description = description
        self.requirements = requirements
        self.rewards = rewards
        self.is_completed = is_completed
        self.quest_id = quest_id
        self.is_active = is_active
        self.loot = loot
        self.message = message
        self.progress = {
            "locations_visited": []
        }

    def activate(self):
        """
        Активирует квест.
        """
        self.is_active = True

    def complete(self):
        """
        Завершает квест.
        """
        self.is_completed = True

    def update_progress(self, location: int):
        """
        Обновляет прогресс квеста, добавляя посещенную локацию.

        :param location: Идентификатор локации.
        """
        if location not in self.progress["locations_visited"]:
            self.progress["locations_visited"].append(location)

    def get_progress(self, requirement: str) -> int:
        """
        Возвращает прогресс квеста по заданному требованию.

        :param requirement: Требование для проверки прогресса.
        :return: Прогресс квеста по заданному требованию.
        """
        if requirement == "locations":
            return len(self.progress["locations_visited"])
        return 0

    def get_rewards(self) -> Dict[str, Union[int, List[Item], str]]:
        """
        Возвращает награды за выполнение квеста.

        :return: Словарь с наградами.
        """
        rewards = self.rewards.copy()
        if self.loot:
            rewards['loot'] = [
                Item(
                    name=item_data["name"],
                    item_type=item_data["item_type"],
                    attack_bonus=item_data["attack_bonus"],
                    defense_bonus=item_data["defense_bonus"]
                )
                for item_data in self.loot
            ]
        if self.message:
            rewards['message'] = self.message
        return rewards

    def is_location_valid(self, location: int) -> bool:
        """
        Проверяет, является ли локация допустимой для квеста.

        :param location: Идентификатор локации.
        :return: True, если локация допустима, иначе False.
        """
        locations = self.requirements.get('locations', [])
        return location in locations

    def check_completion(self) -> bool:
        """
        Проверяет, выполнены ли все требования квеста.

        :return: True, если все требования выполнены, иначе False.
        """
        required_locations = self.requirements.get("locations", [])
        if all(loc in self.progress["locations_visited"] for loc in required_locations):
            return True
        return False
