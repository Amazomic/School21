from engine.item import Item

class NPC:
    """
    Класс NPC представляет неигрового персонажа с именем и списком фраз.

    Атрибуты:
        name (str): Имя NPC.
        phrases (list[str]): Список фраз, которые NPC может произносить.
        loot (list[dict]): Список предметов, которые NPC может выдать.
        is_active (bool): Флаг активности NPC.
    """

    def __init__(self, name: str, phrases: list[str], loot: list[dict], is_active: bool = True):
        self.name = name
        self.phrases = phrases
        self.loot = loot
        self.is_active = is_active

    def talk(self) -> str:
        """
        Возвращает следующую фразу из списка фраз NPC и обновляет состояние.

        Возвращает:
            str: Текущая фраза NPC или сообщение, если NPC неактивен.
        """
        if not self.is_active:
            return "Всё что нужно уже было сказано"

        phrase = self.phrases[0]
        self.is_active = False

        return phrase

    def receive(self, item: str):
        """
        Обрабатывает получение предмета NPC.

        Параметры:
            item (str): Название предмета.
        """
        pass

    def give_loot(self, protagonist):
        """
        Выдает лут игроку.

        Параметры:
            protagonist: Объект игрока, который получает лут.
        """
        loot_message = ""
        for item_data in self.loot:
            item = Item(
                name=item_data["name"],
                item_type=item_data["item_type"],
                attack_bonus=item_data["attack_bonus"],
                defense_bonus=item_data["defense_bonus"]
            )
            protagonist.equip_item(item)
            loot_message += f"Вы получили {item.name}.\n"

        return loot_message

