import subprocess
from flask import Flask, render_template, request


def save_query(query):
    with open('files/in', 'w', encoding='UTF-8') as file:
        file.write(query)


def run_search_engine():
    subprocess.call(['./searcher'])


def parse_results(results):
    with open('files/out', 'r', encoding='UTF-8') as file:
        while (line := file.readline()):
            title, url = line.split("\\\\")
            results[title] = url


app = Flask(__name__, template_folder="web/templates",static_folder='web/static')


@app.route('/')
def home():
    return render_template('index.html')


@app.route('/search', methods=['GET', 'POST'])
def search():
    query = request.form['query']
    save_query(query)

    run_search_engine()

    results = dict()
    parse_results(results)
    print(results)

    return render_template('search.html', query=query, results=results)


if __name__ == '__main__':
    app.run(debug=True)