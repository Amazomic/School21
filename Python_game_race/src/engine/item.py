class Item:
    """
    Класс, представляющий предмет в игре.

    :param name: Название предмета.
    :param item_type: Тип предмета (например, "weapon", "armor").
    :param attack_bonus: Бонус к атаке, предоставляемый предметом. По умолчанию 0.
    :param defense_bonus: Бонус к защите, предоставляемый предметом. По умолчанию 0.
    """

    def __init__(self, name, item_type, attack_bonus=0, defense_bonus=0):
        """
        Инициализация предмета.

        :param name: Название предмета.
        :param item_type: Тип предмета (например, "weapon", "armor").
        :param attack_bonus: Бонус к атаке, предоставляемый предметом. По умолчанию 0.
        :param defense_bonus: Бонус к защите, предоставляемый предметом. По умолчанию 0.
        """
        self.name = name
        self.item_type = item_type
        self.attack_bonus = attack_bonus
        self.defense_bonus = defense_bonus

    def __str__(self):
        """
        Возвращает строковое представление предмета.

        :return: Строковое представление предмета.
        """
        return f"{self.name} (Тип: {self.item_type}, Атака: {self.attack_bonus}, Защита: {self.defense_bonus})"
