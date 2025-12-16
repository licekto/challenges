import os, pytest


def get_data_dir():
    root_dir = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
    return os.path.join(root_dir, 'data')


def get_lines(path):
    file_path = os.path.join(get_data_dir(), *path)
    with open(file_path, 'r') as file:
        return [line for line in file]


@pytest.fixture
def get_data():
    return get_lines
