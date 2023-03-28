import os
import json

# Укажите путь к директории, в которой находятся JSON-файлы
directory_path = '../files/text/'

# Создаем пустой список для хранения заголовков
titles = []

# Цикл для обхода всех файлов с расширением .json в директории
for filename in os.listdir(directory_path):
    if filename.endswith('.json'):
        # Открываем файл и считываем его содержимое
        with open(directory_path + filename, 'r') as json_file:
            data = json.load(json_file)
            # Получаем значение поля "title" и добавляем его в список
            title = data.get('title')
            if title:
                titles.append(title)

# Записываем все заголовки в один файл all_titles.json
with open('all_titles.json', 'w') as output_file:
    json.dump(titles, output_file)
