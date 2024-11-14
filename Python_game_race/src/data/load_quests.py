import json
import os
from typing import List
from engine.quest import Quest


def load_quests() -> List[Quest]:
    """
    Загружает квесты из JSON файла и преобразует их в объекты класса Quest.
    """
    file_dir = os.path.dirname(__file__)
    file_path = os.path.join(file_dir, 'quests.json')

    with open(file_path, 'r', encoding='utf-8') as f:
        quests_data = json.load(f)

    quests = [
        Quest(
            description=quest_data['description'],
            requirements=quest_data['requirements'],
            rewards=quest_data['rewards'],
            is_completed=quest_data['is_completed'],
            quest_id=quest_data['id'],
            is_active=quest_data.get('is_active', False),
            loot=quest_data.get('loot', []),
            message=quest_data.get('message', '')
        )
        for quest_data in quests_data
    ]

    return quests
