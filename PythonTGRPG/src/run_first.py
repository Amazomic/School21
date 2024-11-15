import os
import subprocess

# Рекомендуемая версия Python 3.10.12

def generate_sphinx_docs():
    project_dir = os.getcwd()
    docs_dir = os.path.join(project_dir, "docs")
    
    os.chdir(docs_dir)
    subprocess.run(["make", "html"], check=True)
    
def set_api_token_in_file(api_token, file_path):
    try:
        with open(file_path, 'r') as file:
            lines = file.readlines()

        for i, line in enumerate(lines):
            if line.startswith('API_TOKEN = '):
                lines[i] = f"API_TOKEN = '{api_token}'\n"
                break

        with open(file_path, 'w') as file:
            file.writelines(lines)

    except FileNotFoundError:
        print(f"Файл не найден: {file_path}")
    except Exception as e:
        print(f"Произошла ошибка при записи в файл: {e}")

def run_play_bot(file_path):
    try:
        subprocess.run(["python", file_path], check=True)
    except KeyboardInterrupt:
        print("\nВыполнение скрипта прервано пользователем.")
    except subprocess.CalledProcessError as e:
        print(f"Ошибка при запуске скрипта: {e}")

def install_requirements():
    try:
        subprocess.run(["pip", "install", "-r", "requirements.txt"], check=True)
    except subprocess.CalledProcessError as e:
        print(f"Ошибка при установке зависимостей: {e}")

if __name__ == "__main__":
    # Установка зависимостей из requirements.txt
    install_requirements()

    # Генерация документации
    generate_sphinx_docs()
    print(f"Документация тут: docs/_build/html/index.html")

    # Сохранение оригинального каталога
    original_dir = os.getcwd() 
    api_token = input("Введите ваш API токен: ")
    
    # Установка api_token
    play_bot_path = os.path.join(original_dir, "..", "play_bot.py")  
    set_api_token_in_file(api_token, play_bot_path) 
    current_dir = os.path.dirname(os.path.abspath(__file__))
    os.chdir(current_dir)

    # Запуск бота
    run_play_bot("play_bot.py")  
