from sqlite3 import connect # Мәліметтер қоры үшін SQLite кітапханасын шақыру
from database_query import* # sql запростарына арналған жеке файл

# Мәліметтер қорын жазу үшін класс ашамыз
class Database():
    def __init__(self) -> None:
        self.conn = connect('security.db') # Мәліметтер қоры үшін байланыс орнату
        self.cursor = self.conn.cursor()   # Курсор негізгі функция
        
        self.data_table = create_data_table 
        self.insert_data = insert_data_table
        
        self.new_finger_table = create_new_finger_table
        self.insert_noti_new_finger = insert_new_finger

    # Таблица құратын функция нотификацияға    
    def create_table(self):
        self.cursor.execute(self.data_table)
        print("Data table created")
    # Талица құратын функция жаңа саусақ ізіне
    def create_table_new(self):
        self.cursor.execute(self.new_finger_table)
        print("Data new table created")        
    # Нотфикация үшін мәлімет енгізу функциясы
    def insert_datas(self, id_user: int, type: str, date_time: str, check: str):
        self.cursor.execute(self.insert_data, (id_user, type, date_time, check, ))
        self.conn.commit()
    # Жаңа саусақ ізі үшін мәліметтер енгізу функциясы
    def insert_noti_new_fingers(self, id_user: int, type: str, date_time: str, check: str):
        self.cursor.execute(self.insert_noti_new_finger, (id_user, type, date_time, check, ))
        self.conn.commit()              
    # нотификацияларды асинхронды менеджер үшін шығару функциясы  
    def fetch_all_noti(self) -> int:
        count = 0
        all = []
        self.cursor.execute("SELECT type FROM data WHERE check_='false'")
        type_list = self.cursor.fetchall()
        
        # Мәліметтерді циклда жинайды
        for i in type_list:
            s = ''.join(map(str, i))
            all.append(s)
        # Цикл мәліметтерді керекті форматқа келтірді
        for t in all:
            count += 1
            self.cursor.execute("UPDATE data SET check_='true' WHERE type=?", (t, ))
            self.conn.commit()
        # санағыш санды қайтарады
        return count
    # Тура үстінгі функция ролі тек жаңа саусақ ізі үшін
    def fetch_new_finger(self) -> int:
        count = 0
        all = []
        self.cursor.execute("SELECT type FROM new WHERE check_ = 'false'")
        type_list = self.cursor.fetchall()
        
        for i in type_list:
            s = ''.join(map(str, i))
            all.append(s)
        
        for t in all:
            count += 1
            self.cursor.execute("UPDATE new SET check_='true' WHERE type=?", (t, ))
            self.conn.commit()

        return count 

    


            


if __name__ == "__main__":
    db = Database()
    db.create_table()
    db.create_table_new()
