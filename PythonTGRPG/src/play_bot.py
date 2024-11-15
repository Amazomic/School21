import asyncio
from aiogram import Bot, Dispatcher, types
from aiogram.exceptions import TelegramUnauthorizedError, TelegramNetworkError
from aiogram.types import ReplyKeyboardMarkup, KeyboardButton, FSInputFile
from aiogram.filters import Command
from aiogram.utils.keyboard import ReplyKeyboardBuilder
from aiogram.client.session.aiohttp import AiohttpSession
from aiohttp import ClientConnectorError

from engine import protagonist
from engine.item import Item
from engine.direction import Direction

# Установите свой API_TOKEN
API_TOKEN = ''

# Создаем сессию и инициализируем бота
session = AiohttpSession()
bot = Bot(token=API_TOKEN, session=session)
dp = Dispatcher()

protagonists = {}

# Меню персонажей
characters_menu_builder = ReplyKeyboardBuilder()
characters_menu_builder.row(KeyboardButton(text="Рыцарь"), KeyboardButton(text="Лучник"))
characters_menu_builder.row(KeyboardButton(text="Маг"), KeyboardButton(text="Эльф"))
characters_menu_keyboard = characters_menu_builder.as_markup(resize_keyboard=True)

# Меню предметов (будет показано после выбора персонажа)
items_menu_builder = ReplyKeyboardBuilder()
items_menu_builder.row(KeyboardButton(text="Меч"), KeyboardButton(text="Лук"))
items_menu_builder.row(KeyboardButton(text="Посох"), KeyboardButton(text="Копьё"))
items_menu_keyboard = items_menu_builder.as_markup(resize_keyboard=True)

# Меню для старта игры или изменения выбора
confirm_menu_builder = ReplyKeyboardBuilder()
confirm_menu_builder.row(KeyboardButton(text="Начать игру"))
confirm_menu_builder.row(KeyboardButton(text="Изменить выбор"))
confirm_menu_keyboard = confirm_menu_builder.as_markup(resize_keyboard=True)

# Меню с информацией о персонаже (открывается по кнопке "Персонаж")
character_menu_builder = ReplyKeyboardBuilder()
character_menu_builder.row(KeyboardButton(text="Карта"), KeyboardButton(text="Задание"))
character_menu_builder.row(KeyboardButton(text="Экипировка"), KeyboardButton(text="Назад"))
character_menu_keyboard = character_menu_builder.as_markup(resize_keyboard=True)

# Главное меню с кнопкой "Персонаж"
main_menu_builder = ReplyKeyboardBuilder()
main_menu_builder.row(KeyboardButton(text="Персонаж"))
main_menu_keyboard = main_menu_builder.as_markup(resize_keyboard=True)

# Меню в случае смерти
get_death_menu  = ReplyKeyboardBuilder()
get_death_menu.row(KeyboardButton(text="Начать заново"))
get_death_menu.row(KeyboardButton(text="Поплакать"))
death_menu_keyboard = get_death_menu.as_markup(resize_keyboard=True)

# Меню в случае выйгрыша
get_win_menu = ReplyKeyboardBuilder()
get_win_menu.row(KeyboardButton(text="Начать заново"))
get_win_menu.row(KeyboardButton(text="Порадоваться"))
win_menu_keyboard = get_win_menu.as_markup(resize_keyboard=True)


@dp.message(Command("start"))
async def start(message: types.Message):
    """
    Обрабатывает команду /start и предлагает выбрать персонажа.
    """
    await message.answer(
        " Добро пожаловать в нашу супер пупер игру \n\nЭта история об одном смелом войне который оказался в Магической стране полной опасности и приключений\n\nТебе предстоит сражаться со злодеями, захватывать замки и путешествовать по нашему удивительномму миру \n\nДля начала выбери персонажа, за которого будешь играть:",
        reply_markup=characters_menu_keyboard)


@dp.message(lambda message: message.text in ["Рыцарь", "Лучник", "Маг", "Эльф"])
async def selected_character_handler(message: types.Message):
    """
    Обрабатывает выбор персонажа и предлагает выбрать предмет.
    """
    user_id = message.from_user.id
    if user_id not in protagonists:
        protagonists[user_id] = {}
    protagonists[user_id]['character'] = message.text
    await message.answer(f"Выбран персонаж: {message.text}. Теперь выбери начальное оружие.",
                         reply_markup=items_menu_keyboard)


@dp.message(lambda message: message.text in ["Меч", "Лук", "Посох", "Копьё"])
async def selected_item_handler(message: types.Message):
    """
    Обрабатывает выбор предмета и предлагает подтвердить выбор или изменить его.
    """
    user_id = message.from_user.id
    protagonists[user_id]['item'] = message.text
    selected_character = protagonists[user_id]['character']
    await message.answer(
        f"Ты выбрал персонажа: {selected_character} и предмет: {message.text}. Если всё верно, нажми 'Начать игру', или измени свой выбор.",
        reply_markup=confirm_menu_keyboard)


@dp.message(lambda message: message.text == "Изменить выбор")
async def change_selection_handler(message: types.Message):
    """
    Обрабатывает команду 'Изменить выбор' и возвращает пользователя к выбору персонажа или предмета.
    """
    await message.answer("Выбери нового персонажа или предмет:", reply_markup=characters_menu_keyboard)


@dp.message(lambda message: message.text == "Начать игру")
async def start_game_handler(message: types.Message):
    """
    Начало игры после выбора персонажа и предмета. Создание объекта Protagonist.
    """
    user_id = message.from_user.id
    player_name = message.from_user.username or f"Игрок_{user_id}"
    item = protagonists[user_id]['item']

    protagonists[user_id]['protagonist'] = protagonist.Protagonist(name=player_name, id=str(user_id), current_location=1)
    protagonists[user_id]['protagonist'].equip_item(Item(name=item, item_type='weapon', attack_bonus=1, defense_bonus=0))
    await send_photo(message, "locations", 1)
    await message.answer(f"Игра началась! {protagonists[user_id]['protagonist'].whereami()}",
                         reply_markup=await get_movement_menu(user_id))


@dp.message(lambda message: message.text in ["⬆️ Север", "⬇️ Юг", "⬅️ Запад", "➡️ Восток"])
async def move_handler(message: types.Message):
    """
    Обрабатывает команды перемещения по карте.
    """
    user_id = message.from_user.id
    direction = message.text.split(" ")[1].capitalize()

    if user_id in protagonists and 'protagonist' in protagonists[user_id]:
        protagonist = protagonists[user_id]['protagonist']
        direction_obj = Direction(direction)
        result = protagonist.go(direction_obj)

        await send_photo(message, "locations", protagonist.current_location)
        await message.answer(result, reply_markup=await get_movement_menu(user_id))
    else:
        await message.answer("Ошибка: персонаж не найден.")


@dp.message(lambda message: message.text == "Персонаж")
async def character_menu_handler(message: types.Message):
    """
    Открывает меню персонажа с опциями "Карта", "Задание", "Экипировка" и "Назад".
    """
    user_id = message.from_user.id
    protagonist = protagonists[user_id]['protagonist']
    stats = protagonist.get_stats()
    await message.answer(f"Характеристики персонажа:\n{stats}", reply_markup=character_menu_keyboard)


@dp.message(lambda message: message.text == "Назад")
async def back_to_main_menu_handler(message: types.Message):
    """
    Возвращает пользователя в основное меню с кнопкой "Персонаж".
    """
    user_id = message.from_user.id
    await message.answer("Вы вернулись в основное меню.", reply_markup=await get_movement_menu(user_id))


@dp.message(lambda message: message.text == "Карта")
async def show_map_handler(message: types.Message):
    """
    Обработчик для отправки карты
    """
    user_id = message.from_user.id
    photo_path = "./img/map.jpg"
    photo = FSInputFile(photo_path)

    protagonist = protagonists[user_id]['protagonist']
    if not protagonist:
        await message.answer("Ошибка: персонаж не найден. Пожалуйста, начните игру.")
        return

    location_id = protagonist.current_location
    caption = (f"Вот твоя карта.\n"
               f"Ты на локации: {location_id}")
    await message.answer_photo(photo, caption=caption)


async def get_movement_menu(user_id: int) -> ReplyKeyboardMarkup:
    """
    Возвращает клавиатуру с доступными направлениями для текущей локации
    и кнопками "Говорить", "Драться", "Захватить базу", при этом
    кнопка "Персонаж" всегда будет располагаться внизу.
    """
    protagonist = protagonists[user_id]['protagonist']
    available_directions = protagonist.get_available_directions()
    movement_menu_builder = ReplyKeyboardBuilder()

    direction_emojis = {
        "Север": "⬆️ Север",
        "Юг": "⬇️ Юг",
        "Запад": "⬅️ Запад",
        "Восток": "➡️ Восток"
    }

    for direction in available_directions:
        movement_menu_builder.add(KeyboardButton(text=direction_emojis.get(direction, direction)))

    if int(protagonist.current_location) in [2, 9, 11, 17, 21]:
        movement_menu_builder.row(KeyboardButton(text="Говорить"))

    if int(protagonist.current_location) in [4, 6, 12, 16, 22, 23]:
        movement_menu_builder.row(KeyboardButton(text="Драться"))

    if int(protagonist.current_location) in [5, 7, 10, 13, 15, 19, 24, 25]:
        movement_menu_builder.row(KeyboardButton(text="Захватить базу"))

    movement_menu_builder.row(KeyboardButton(text="Персонаж"))

    return movement_menu_builder.as_markup(resize_keyboard=True)


@dp.message(lambda message: message.text == "Задание")
async def show_mission_handler(message: types.Message):
    """
    Показывает текущие активные задания персонажа и их статус.
    """
    user_id = message.from_user.id
    protagonist = protagonists[user_id]['protagonist']

    active_quests = protagonist.get_active_quests()

    if not active_quests:
        await message.answer("У вас нет активных заданий.")
        return

    response_text = "Ваши текущие активные задания:\n\n"

    for quest in active_quests:
        if not quest.is_completed:
            required_locations = quest.requirements.get("locations", [])
            current_locations_visited = len(quest.progress["locations_visited"])
            response_text += f"{quest.description}: {current_locations_visited}/{len(required_locations)}\n"

    await message.answer(response_text)


@dp.message(lambda message: message.text == "Экипировка")
async def show_inventory_handler(message: types.Message):
    """
    Показывает Экипировка персонажа.
    """
    user_id = message.from_user.id
    if user_id in protagonists and 'protagonist' in protagonists[user_id]:
        protagonist = protagonists[user_id]['protagonist']
        inventory = protagonist.get_equipped_items()
        if inventory:
            inventory_list = "\n".join(inventory)
            await message.answer(f"На тебе экипировка:\n{inventory_list}")
        else:
            await message.answer("Твоя экипировка пуст.")
    else:
        await message.answer("Твоя экипировка пуста.")


@dp.message(lambda message: message.text == "Говорить")
async def talk_handler(message: types.Message):
    """
    Обрабатывает нажатие кнопки 'Говорить', вызывает диалог с NPC.
    """
    user_id = message.from_user.id
    protagonist = protagonists.get(user_id, {}).get('protagonist')

    if not protagonist:
        await message.answer("Ошибка: персонаж не найден. Пожалуйста, начните игру.")
        return

    if not protagonist.is_npc_present():
        await message.answer("Здесь нет никого, с кем можно поговорить.")
        return

    await send_photo(message, "npcs", protagonist.current_location)
    await message.answer(protagonist.talk_to(), reply_markup=await get_movement_menu(user_id))


@dp.message(lambda message: message.text == "Драться")
async def fight_handler(message: types.Message):
    """
    Обрабатывает нажатие кнопки 'Драться', отправляет фото врага и начинает сражение.
    """
    user_id = message.from_user.id
    protagonist = protagonists.get(user_id, {}).get('protagonist')

    if not protagonist:
        await message.answer("Ошибка: персонаж не найден.")
        return

    if not protagonist.is_enemy_present():
        await message.answer("Здесь нет врагов, с которыми можно сражаться.")
        return

    await send_photo(message, "enemies", protagonist.current_location)
    battle_result = protagonist.fight_to("fight")

    if protagonist.hp <= 0:
        await handle_death(message)
    else:
        await message.answer(battle_result, reply_markup=await get_movement_menu(user_id))

        if protagonist.check_game_completion():
            await handle_win(message)


@dp.message(lambda message: message.text == "Захватить базу")
async def capture_base_handler(message: types.Message):
    """
    Обрабатывает нажатие кнопки 'Захватить базу'.
    """
    user_id = message.from_user.id
    protagonist = protagonists.get(user_id, {}).get('protagonist')

    if not protagonist:
        await message.answer("Ошибка: персонаж не найден.")
        return

    battle_result = protagonist.fight_to("capture")

    if protagonist.hp <= 0:
        await handle_death(message)
    else:
        await message.answer(battle_result, reply_markup=await get_movement_menu(user_id))

        if protagonist.check_game_completion():
            await handle_win(message)        

async def handle_win(message: types.Message):
    """
    Обрабатывает выигрыш персонажа и выводит соответствующее сообщение.
    """
    user_id = message.from_user.id
    protagonist = protagonists.get(user_id, {}).get('protagonist')

    if not protagonist:
        await message.answer("Ошибка: персонаж не найден.")
        return

    await message.answer("Поздравлем! Вы прошли нашу супер-пупер игру!", reply_markup=win_menu_keyboard)


async def handle_death(message: types.Message):
    """
    Обрабатывает смерть персонажа и выводит соответствующее сообщение.
    """
    user_id = message.from_user.id
    protagonist = protagonists.get(user_id, {}).get('protagonist')

    if not protagonist:
        await message.answer("Ошибка: персонаж не найден.")
        return

    await message.answer("Вы погибли. Тут и сказочки конец.", reply_markup=death_menu_keyboard)


async def send_photo(message: types.Message, folder: str, filename: str):
    """
    Функция для отправки фото.
    
    :param message: Сообщение от пользователя для получения chat_id.
    :param folder: Папка, в которой находится фото.
    :param filename: Имя файла (без расширения).
    """
    photo_path = f"./img/{folder}/{filename}.png"
    photo = FSInputFile(photo_path)
    await message.answer_photo(photo)

@dp.message(lambda message: message.text == "Начать заново")
async def start(message: types.Message):
    """
    Обрабатывает команду /start и предлагает выбрать персонажа.
    """
    user_id = message.from_user.id
    protagonist = protagonists.get(user_id, {}).get('protagonist')

    if protagonist and protagonist.hp <= 0:
        await message.answer(
            "Это загробный мир или как?! Наверное, нужно чаще смотреть на карту.",
            reply_markup=characters_menu_keyboard)
    elif protagonist and protagonist.check_game_completion():
        await message.answer(
            "Это что параллельная вселенная?! Где-то я это уже видел.",
            reply_markup=characters_menu_keyboard)

    protagonists[user_id] = {}


@dp.message(lambda message: message.text == "Порадоваться")
async def rejoice(message: types.Message):
    """
    Обрабатывает команду /rejoice и выводит сообщение.
    """
    await message.answer("Ура! Ай да я, ай да молодец!")

@dp.message(lambda message: message.text == "Поплакать")
async def cry(message: types.Message):
    """
    Обрабатывает команду /cry и выводит сообщение.
    """
    await message.answer("Плак, плак. Горе то какое...")

async def main():
    """
    Запускает процесс опроса обновлений от Telegram.
    """
    try:
        print('Bot started and running...')
        await dp.start_polling(bot)
    except TelegramUnauthorizedError as e:
        print(f"Error: Unauthorized. Check your API token. {e}")
    except TelegramNetworkError as e:
        print(f"Error: Cannot connect to Telegram API. Network error occurred: {e}")
    except ClientConnectorError as e:
        print(f"Error: Cannot connect to Telegram API. Please check your internet connection. {e}")
    except Exception as e:
        print(f"An unexpected error occurred: {e}")


if __name__ == "__main__":
    print('Starting bot...')
    asyncio.run(main())
