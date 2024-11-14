from typing import Optional, List
from data.load_all import all_data
from engine.direction import Direction
from engine.enemy import Enemy
from engine.npc import NPC
from engine.quest import Quest

import copy

class Protagonist:
    """
    Класс Protagonist представляет главного героя с характеристиками и инвентарем.

    Атрибуты:
        id (str): Идентификатор героя.
        name (str): Имя героя.
        hp (int): Здоровье героя.
        strength (int): Сила героя.
        craft (int): Уровень крафта героя.
        inventory (defaultdict[int]): Инвентарь героя.
    """

    def __init__(self, name: str, id: str, current_location: str):
        """
        Инициализация главного героя.

        :param name: Имя героя.
        :param id: Идентификатор героя.
        :param current_location: Текущая локация героя.
        """
        self.id = id
        self.name = name
        self.character = ""
        self.atk = 2
        self.dfn = 2
        self.hp = 10
        self.level = 1
        self.xp = 0
        self.xp_required = self.calculate_xp_required()
        self.current_location = str(current_location)

        self.npcs = copy.deepcopy(all_data["npcs"])
        self.enemies = copy.deepcopy(all_data["enemies"])
        self.quests = copy.deepcopy(all_data["quests"])

        self.equipped_items = {
            'weapon': None,
            'armor': None,
        }

    def get_active_quests(self) -> List[Quest]:
        """
        Возвращает список активных (незавершённых) квестов.

        :return: Список активных квестов.
        """
        active_quests = []
        for quest in self.quests:
            if isinstance(quest, Quest):
                if quest.is_active and not quest.is_completed:
                    active_quests.append(quest)
            else:
                print(f"Найден некорректный элемент в списке квестов: {quest}")
        return active_quests

    def activate_quest_by_location(self, location: int) -> str:
        """
        Активирует квест на основе идентификатора текущей локации.

        :param location: Текущий идентификатор локации.
        :return: Сообщение об активации квеста.
        """
        activated_quest_message = ""
        debug_info = []

        for quest in self.quests:
            if int(quest.quest_id) == int(location):
                quest.activate()
                activated_quest_message += f"Квест '{quest.description}' получен.\n"
                debug_info.append(activated_quest_message)

        return activated_quest_message

    def complete_quest(self, quest: Quest) -> None:
        """
        Завершает квест и выдает награды.

        :param quest: Квест, который нужно завершить.
        """
        rewards = quest.get_rewards()

        if rewards:
            self.xp += rewards.get("xp", 0)
            if self.xp >= self.xp_required:
                self.level_up()

            item = rewards.get("item")
            if item:
                self.equip_item(item)

    def check_game_completion(self) -> bool:
        """
        Проверяет, выполнены ли оба квеста (захватить базы и уничтожить всех врагов).

        :return: True, если игра завершена, иначе False.
        """
        required_descriptions = ["Захватить базы", "Уничтожить врагов"]
        completed_descriptions = [quest.description for quest in self.quests if quest.is_completed]

        return all(desc in completed_descriptions for desc in required_descriptions)

    def set_character(self, character: str):
        """
        Устанавливает характеристику героя.

        :param character: Характеристика героя.
        """
        self.character = character

    def equip_item(self, item):
        """
        Экипирует предмет.

        :param item: Предмет для экипировки.
        :return: Сообщение об экипировке предмета.
        """
        item_type = item.item_type
        if self.equipped_items[item_type]:
            self.unequip_item(item_type)
        self.equipped_items[item_type] = item
        self.atk += item.attack_bonus
        self.dfn += item.defense_bonus
        return f"Вы получили {item.name}."

    def unequip_item(self, item_type):
        """
        Снимает экипированный предмет.

        :param item_type: Тип предмета для снятия.
        """
        if self.equipped_items[item_type]:
            item = self.equipped_items[item_type]
            self.atk -= item.attack_bonus
            self.dfn -= item.defense_bonus
            self.equipped_items[item_type] = None
        else:
            print(f"Нет экипированного предмета типа {item_type}.")

    def get_equipped_items(self):
        """
        Возвращает список экипированных предметов.

        :return: Список экипированных предметов.
        """
        return [str(item) for item in self.equipped_items.values() if item]

    def calculate_xp_required(self):
        """
        Рассчитывает количество опыта, необходимое для следующего уровня.

        :return: Количество опыта, необходимое для следующего уровня.
        """
        return int(10 * (1.5 ** (self.level - 1)))

    def level_up(self):
        """
        Повышает уровень героя.

        :return: Сообщение о повышении уровня.
        """
        self.level += 1
        self.hp += 2
        self.atk += 2
        self.dfn += 2
        self.xp = 0
        self.xp_required = self.calculate_xp_required() 

        return f"Вы достигли уровня {self.level}. Ваше здоровье, атака и защита увеличились на 2."

    def gain_xp(self, amount: int):
        """
        Добавляет опыт герою.

        :param amount: Количество опыта для добавления.
        :return: Сообщение о получении опыта.
        """
        self.xp += amount
        message = f"Вы получили {amount} XP."

        if self.xp >= self.xp_required:
            level_up_message = self.level_up()
            message += f"\n\n{level_up_message}"

        return message

    def go(self, direction: 'Direction') -> str:
        """
        Перемещает героя в указанном направлении и возвращает описание локации.

        :param direction: Направление перемещения.
        :return: Описание текущей локации или ошибка перемещения.
        """
        next_location_id = self.get_next_location(direction)

        if not next_location_id:
            return "Невозможно переместиться в этом направлении."

        self.current_location = str(next_location_id)
        current_description = all_data["locations"][self.current_location]["description"]
        return current_description

    def talk_to(self) -> str:
        """
        Запускает диалог с NPC в текущей локации и активирует квест при необходимости.

        :return: Имя персонажа и его диалог.
        """
        npc_data = self.npcs.get(str(self.current_location), [{}])[0]
        is_active = npc_data.get("is_active", True)
        loot = npc_data.get("loot", [])

        npc = NPC(
            name=npc_data.get("name", "Неизвестный NPC"),
            phrases=npc_data.get("dialogs", ["Нет доступных диалогов"]),
            loot=loot,
            is_active=is_active
        )

        dialogue = npc.talk()
        npc_data["is_active"] = npc.is_active
        self.npcs[str(self.current_location)][0] = npc_data

        activated_quest_message = ""
        loot_message = ""

        if is_active:
            if loot:
                loot_message = npc.give_loot(self)
            activated_quest_message = self.activate_quest_by_location(self.current_location)

        quest_completion_message = self.update_quest_progress_after_capture(self.current_location)

        messages = [f"Вы встретили {npc.name}.", dialogue]

        if loot_message:
            messages.append(loot_message)
        if activated_quest_message:
            messages.append(activated_quest_message)
        if quest_completion_message:
            messages.append(quest_completion_message)

        return "\n\n".join(messages)

    def update_quest_progress_after_capture(self, location: int):
        """
        Обновляет прогресс квеста после захвата базы.

        :param location: Локация, где была захвачена база.
        """
        for quest in self.get_active_quests():
            if quest.is_location_valid(int(location)):
                quest.update_progress(location=int(location))
                if quest.check_completion():
                    quest.complete()
                    self.complete_quest(quest)
                    rewards = quest.get_rewards()
                    message = rewards.get('message', '')
                    xp = rewards.get('xp', 0)
                    loot = rewards.get('loot', [])

                    completion_message = f"\nПоздравляем, вы выполнили квест: {quest.description}\n"

                    if xp > 0:
                        xp_message = self.gain_xp(xp)
                        completion_message += f"{xp_message}\n"

                    if loot:
                        for item in loot:
                            equip_message = self.equip_item(item)
                            completion_message += f"{equip_message}\n"

                    if message:
                        return f"{message}\n{completion_message}"
                    else:
                        return completion_message

    def fight_to(self, action: str) -> str:
        """
        Начинает сражение с врагом или захват базы, если враг находится в локации.

        :param action: Действие, которое нужно выполнить ("fight" для сражения, "capture" для захвата базы).
        :return: Результат сражения или захвата базы.
        """
        enemy_data = self.enemies.get(self.current_location)
        if enemy_data:
            if not enemy_data.get("is_active", False):
                if action == "fight":
                    return "Вы уже победили этого врага."
                elif action == "capture":
                    return "Вы уже захватили эту базу."

            enemy = Enemy(
                name=enemy_data["name"],
                attack=enemy_data["stats"]["attack"],
                defense=enemy_data["stats"]["defense"],
                hp=enemy_data["stats"]["hp"],
                loot=enemy_data["loot"],
                is_active=enemy_data["is_active"]
            )

            if action == "fight":
                battle_result = enemy.battle_round(self)
            elif action == "capture":
                battle_result = enemy.capture_base(self)

            if not enemy.is_active:
                quest_completion_message = self.update_quest_progress_after_capture(self.current_location)
                if quest_completion_message:
                    battle_result += f"\n\n{quest_completion_message}"

            enemy_data["stats"]["hp"] = enemy.hp
            enemy_data["is_active"] = enemy.is_active

            return battle_result
        return "Здесь нет врагов для сражения."

    def give_rewards(self, quest):
        """
        Выдаёт награды за завершение квеста.
        """
        rewards = quest.get_rewards()
        for reward, amount in rewards.items():
            print(f"Вы получили {amount} {reward} за выполнение квеста '{quest.description}'")

    def get_next_location(self, direction: Direction) -> Optional[int]:
        """
        Возвращает следующую локацию в указанном направлении.

        :param direction: Направление перемещения.
        :return: Идентификатор следующей локации или None, если перемещение невозможно.
        """
        current_location_data = all_data["locations"].get(str(self.current_location))

        next_location_id = current_location_data.get(direction.direction.lower())
        return next_location_id

    def get_available_directions(self) -> list[Direction]:
        """
        Возвращает список доступных для перемещения направлений из текущей локации.

        :return: Список объектов Direction, доступных для перемещения.
        """
        current_location_data = all_data["locations"].get(str(self.current_location), {})
        directions = {
            'север': Direction.NORTH,
            'юг': Direction.SOUTH,
            'восток': Direction.EAST,
            'запад': Direction.WEST
        }
        available_directions = []

        for direction, direction_obj in directions.items():
            if direction in current_location_data:
                available_directions.append(direction_obj)

        return available_directions

    def whereami(self) -> str:
        """
        Возвращает текущее местоположение героя.

        :return: Текущее местоположение героя.
        """
        location = all_data["locations"].get(str(self.current_location), {})
        description = location.get('description', 'Unknown location')
        return f"{description}"

    def get_stats(self) -> str:
        """
        Возвращает статистику героя.

        :return: Строка с статистикой героя.
        """
        return (f"ID: {self.id}\n"
                f"Имя: {self.name}\n"
                f"Уровень: {self.level}\n"
                f"Опыт: {self.xp}/{self.xp_required}\n"
                f"Здоровье: {self.hp}\n"
                f"Атака: {self.atk}\n"
                f"Защита: {self.dfn}\n"
                f"Текущая локация: {self.current_location}")

    def is_npc_present(self) -> bool:
        """
        Проверяет, есть ли NPC в текущей локации.

        :return: True, если NPC присутствует, иначе False.
        """
        return str(self.current_location) in self.npcs and len(self.npcs[str(self.current_location)]) > 0

    def is_enemy_present(self) -> bool:
        """
        Проверяет, есть ли враг в текущей локации.

        :return: True, если враг присутствует, иначе False.
        """
        return str(self.current_location) in self.enemies and len(self.enemies[str(self.current_location)]) > 0
