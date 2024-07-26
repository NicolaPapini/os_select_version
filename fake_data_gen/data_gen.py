import json
import string
import random
import socket
import time

from tqdm import tqdm

IP_ADDRESS = '127.0.0.1'
PORT = 8123
NUM_ENTRIES = 30000


class Entry:
    def __init__(self, name, surname, number, operation, password):
        self.password = password
        self.name = name
        self.surname = surname
        self.number = number
        self.operation = operation

    def generate_random_entry(self):
        self.name = self.generate_random_name()
        self.surname = self.generate_random_name()
        self.number = self.generate_random_number(random.randint(7, 18))
        self.password = random.choice(['qwerty', '123456'])
        return self

    @staticmethod
    def generate_random_name(max_length=20, max_side_spaces=5, max_middle_spaces=4):
        num_middle_spaces = random.randint(0, max_middle_spaces)
        num_side_spaces = random.randint(0, max_side_spaces)
        space_positions = []
        for pos in range(num_middle_spaces):
            space_positions.append(random.randint(num_side_spaces, max_length - num_side_spaces))

        name_with_middle_spaces = ''.join(
            random.choice(string.ascii_lowercase) for i in range(max_length - num_side_spaces * 2))
        for pos in space_positions:
            name_with_middle_spaces = name_with_middle_spaces[:pos] + ' ' + name_with_middle_spaces[pos + 1:]
        return name_with_middle_spaces

    @staticmethod
    def generate_random_number(length):
        number = ''.join(random.choice(string.digits) for i in range(length))
        space_position = random.randint(0, length)  # generate random position for space
        num_spaces = random.randint(1, 3)  # generate random number of spaces
        number_with_spaces = number[:space_position] + ' ' * num_spaces + number[space_position:]
        leading_spaces = ' ' * random.randint(0, 3)  # generate random number of leading spaces
        trailing_spaces = ' ' * random.randint(0, 3)  # generate random number of trailing spaces
        return leading_spaces + number_with_spaces + trailing_spaces

    def print_entry(self):
        print(f'Name: {self.name}, Surname: {self.surname}, Phone number: {self.number}, Password: {self.password}')

    def to_json(self):
        return json.dumps(self.__dict__)


if __name__ == '__main__':
    server_address = ('127.0.0.1', 8123)
    entries = []
    for i in tqdm(range(NUM_ENTRIES)):
        client_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        client_socket.connect(server_address)
        entry = Entry('', '', '', 'ADD', '').generate_random_entry()
        entry_json = entry.to_json()
        entries.append(entry)
        client_socket.send(entry_json.encode())
        response = client_socket.recv(4096)
        client_socket.close()
    time.sleep(1)
    for entry in tqdm(entries):
        entry.name = entry.name.strip()
        entry.surname = entry.surname.strip()
        entry.number = entry.number.replace(' ', '')
        client_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        client_socket.connect(server_address)
        entry.operation = 'DEL'
        client_socket.send(entry.to_json().encode())
        response = client_socket.recv(4096)
        client_socket.close()
