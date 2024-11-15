from engine.item import Item
import random

class Enemy:
    """
    Класс, представляющий врага в игре.

    :param name: Имя врага.
    :param attack: Сила атаки врага.
    :param defense: Сила защиты врага.
    :param hp: Здоровье врага.
    :param loot: Список предметов, которые враг может выбросить.
    :param is_active: Флаг, указывающий, активен ли враг.
    """

    def __init__(self, name: str, attack: int, defense: int, hp: int, loot: list, is_active: bool):
        """
        Инициализация врага.

        :param name: Имя врага.
        :param attack: Сила атаки врага.
        :param defense: Сила защиты врага.
        :param hp: Здоровье врага.
        :param loot: Список предметов, которые враг может выбросить.
        :param is_active: Флаг, указывающий, активен ли враг.
        """
        self.name = name
        self.attack = attack
        self.defense = defense
        self.hp = hp
        self.loot = loot
        self.is_active = is_active

    def attack_player(self) -> int:
        """
        Метод для атаки игрока.

        :return: Урон, нанесенный игроку.
        """
        return random.randint(1, 6) + self.attack

    def take_damage(self, damage: int) -> bool:
        """
        Метод для получения урона.

        :param damage: Урон, полученный врагом.
        :return: True, если враг жив, иначе False.
        """
        self.hp -= max(0, damage - self.defense)
        return self.hp > 0

    def battle_round(self, protagonist) -> str:
        """
        Метод для проведения раунда боя с игроком.

        :param protagonist: Объект игрока.
        :return: Результат раунда боя.
        """
        player_attack = random.randint(1, 6) + protagonist.atk
        enemy_attack = self.attack_player()

        player_damage = max(0, player_attack - self.defense)
        enemy_damage = max(0, enemy_attack - protagonist.dfn)

        battle_result = f"Вы атакуете врага и наносите {player_damage} урона.\n"
        battle_result += f"{self.name} атакует вас и наносит {enemy_damage} урона.\n"

        if player_damage > enemy_damage:
            self.hp -= 1
            battle_result += f"Враг теряет 1 HP. Осталось {self.hp} HP.\n"
        elif enemy_damage > player_damage:
            protagonist.hp -= 1
            battle_result += f"Вы теряете 1 HP. Осталось {protagonist.hp} HP.\n"
        else:
            battle_result += f"Ничья. Осталось {protagonist.hp} HP у вас и {self.hp} HP у врага.\n"

        if self.hp <= 0:
            battle_result += f"Вы победили {self.name}!\n\n"
            xp_message = protagonist.gain_xp(10)
            battle_result += xp_message + "\n\n"
            for item_data in self.loot:
                item = Item(
                    name=item_data["name"],
                    item_type=item_data["item_type"],
                    attack_bonus=item_data["attack_bonus"],
                    defense_bonus=item_data["defense_bonus"]
                )
                battle_result += protagonist.equip_item(item)
            self.is_active = False
            return battle_result
        return battle_result

    def capture_base(self, protagonist) -> str:
        """
        Метод для захвата базы.

        :param protagonist: Объект игрока.
        :return: Результат захвата базы.
        """
        player_attack = random.randint(1, 6) + protagonist.atk
        enemy_attack = self.attack_player()

        player_damage = max(0, player_attack - self.defense)
        enemy_damage = max(0, enemy_attack - protagonist.dfn)

        capture_result = f"Вы атакуете базу и наносите {player_damage} урона.\n"
        capture_result += f"{self.name} атакует вас и наносит {enemy_damage} урона.\n"

        if player_damage > enemy_damage:
            self.hp -= 1
            capture_result += f"База теряет 1 HP. Осталось {self.hp} HP.\n"
        elif enemy_damage > player_damage:
            protagonist.hp -= 1
            capture_result += f"Вы теряете 1 HP. Осталось {protagonist.hp} HP.\n"
        else:
            capture_result += f"Ничья. Осталось {protagonist.hp} HP у вас и {self.hp} HP у базы.\n"

        if self.hp <= 0:
            capture_result += f"Вы захватили базу {self.name}!\n\n"
            xp_message = protagonist.gain_xp(10)
            capture_result += xp_message + "\n\n"
            for item_data in self.loot:
                item = Item(
                    name=item_data["name"],
                    item_type=item_data["item_type"],
                    attack_bonus=item_data["attack_bonus"],
                    defense_bonus=item_data["defense_bonus"]
                )
                capture_result += protagonist.equip_item(item)
            self.is_active = False
            return capture_result
        return capture_result
