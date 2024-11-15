import random

first_names = [
    "Иван", "Алексей", "Дмитрий",
    "Сергей", "Михаил", "Андрей", "Владислав", "Сергей"
]
"""
Список мужских имен.
"""

last_names = [
    "Иванов", "Петров", "Сидоров", "Смирнов", "Кузнецов",
    "Попов", "Соколов", "Лебедев", "Козлов", "Степанов"
]
"""
Список мужских фамилий.
"""

first_names_woman = [
    "Катерина", "Ольга", "Анастасия",
    "Сюзанна", "Василиса", "Брунгильда", "Картизола", "София"
]
"""
Список женских имен.
"""

last_names_woman = [
    "Прекрасная", "Хитрющая", "Сердобольная", "Безумная", "Хитромудрая", "Добросердечная"
]
"""
Список женских фамилий.
"""

first_names_enemies = [
    "Зловещая", "Коварная", "Беспощадная", "Скверная",
    "Роковая", "Тёмная", "Ведунья", "Мстительная",
    "Леденящая", "Ядовитая", "Огненная", "Непокорная",
    "Смертоносная", "Мрачная", "Проклятая"
]
"""
Список имен врагов.
"""

last_names_enemies = [
    "Мрак", "Ненависть", "Тьма", "Смерть", "Ужас", "Боль",
    "Чумная", "Кровавая", "Гибель", "Погибель", "Терзание",
    "Проклятье", "Отчаяние", "Бездна", "Горечь"
]
"""
Список фамилий врагов.
"""

attack_item_suffixes = [
    "Меч", "Кинжал", "Лук", "Стрела", "Топор", "Секира", "Молот"
]
"""
Список суффиксов для атакующих предметов.
"""

defense_item_suffixes = [
    "Щит", "Доспех", "Шлем", "Сапоги", "Кольчуга"
]
"""
Список суффиксов для защитных предметов.
"""

def generate_name():
    """
    Генерирует случайное мужское имя и фамилию.

    :return: Сгенерированное имя и фамилия.
    """
    return f"{random.choice(first_names)} {random.choice(last_names)}"

def generate_name_woman():
    """
    Генерирует случайное женское имя и фамилию.

    :return: Сгенерированное имя и фамилия.
    """
    return f"{random.choice(first_names_woman)} {random.choice(last_names_woman)}"

def generate_name_enemies():
    """
    Генерирует случайное имя и фамилию врага.

    :return: Сгенерированное имя и фамилия врага.
    """
    return f"{random.choice(first_names_enemies)} {random.choice(last_names_enemies)}"

def generate_attack_item_name():
    """
    Генерирует случайное название атакующего предмета.

    :return: Сгенерированное название атакующего предмета.
    """
    return f"{random.choice(attack_item_suffixes)}"

def generate_defense_item_name():
    """
    Генерирует случайное название защитного предмета.

    :return: Сгенерированное название защитного предмета.
    """
    return f"{random.choice(defense_item_suffixes)}"


if __name__ == "__main__":
    generated_name = generate_name()
    generated_attack_item = generate_attack_item_name()
    generated_defense_item = generate_defense_item_name()
    generated_health_potion = generate_health_potion_name()

    print(f"Сгенерированное имя: {generated_name}")
    print(f"Сгенерированный атакующий предмет: {generated_attack_item}")
    print(f"Сгенерированный защитный предмет: {generated_defense_item}")
    print(f"Сгенерированное зелье здоровья: {generated_health_potion}")
