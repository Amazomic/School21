class Direction:
    """
    Класс Direction представляет направление.

    Атрибуты:
        NORTH (str): Направление на север.
        SOUTH (str): Направление на юг.
        EAST (str): Направление на восток.
        WEST (str): Направление на запад.
    """

    NORTH = "Север"
    SOUTH = "Юг"
    EAST = "Восток"
    WEST = "Запад"

    def __init__(self, direction: str):
        """
        Инициализирует направление.

        Параметры:
            direction (str): Направление. Должно быть одним из значений: NORTH, SOUTH, EAST, WEST.

        Исключения:
            ValueError: Если направление не является допустимым.
        """
        if direction not in [self.NORTH, self.SOUTH, self.EAST, self.WEST]:
            raise ValueError("Invalid direction")
        self.direction = direction
