import os
import subprocess

def generate_sphinx_docs():
    project_dir = os.getcwd()
    docs_dir = os.path.join(project_dir, "docs")

    os.chdir(docs_dir)
    subprocess.run(["make", "html"], check=True)

def set_api_token_in_file(api_token, file_path):
    with open(file_path, 'r') as file:
        lines = file.readlines()

    # Проверяем, есть ли уже строка с api_token, и заменяем её, если есть
    for i, line in enumerate(lines):
        if line.startswith('API_TOKEN ='):
            lines[i] = f'API_TOKEN = "{api_token}"\n'
            break

    with open(file_path, 'w') as file:
        file.writelines(lines)

def run_play_bot(file_path):
    try:
        subprocess.run(["python", file_path], check=True)
    except KeyboardInterrupt:
        print("\nВыполнение скрипта прервано пользователем.")

def install_requirements():
    subprocess.run(["pip", "install", "-r", "requirements.txt"], check=True)

if __name__ == "__main__":


    install_requirements()

    generate_sphinx_docs()
    print("Документация тут: docs/_build/")

    # Запрос ввода API токена от пользователя
    api_token = input("Введите ваш API токен: ")

    play_bot_file = os.path.join(os.getcwd(), "play_bot.py")
    set_api_token_in_file(api_token, play_bot_file)

    # Запуск play_bot.py
    run_play_bot(play_bot_file)
