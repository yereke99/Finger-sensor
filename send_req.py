import requests # запрос тастайтын кітапхана

# Класс жазамыз запрос тастау үшін
class Request:
    def __init__(self) -> None:
        self.open_api = "http://172.20.10.8/api/on"                    # реле қосу үшін аддресс мекен-жайы
        self.close_api = "http://172.20.10.8/api/off"                  # реле өшіру үшін аддресс мекен-жайы
        self.enter_new_finger_api = "http://172.20.10.8/api/newFinger" # жаңа саусақ ізі үшін аддресс мекен-жайы

    # ашу функциясы есікті   
    def Open(self):
        requests.get(self.open_api)
    # жабу функциясы реле үшін 
    def Close(self):
        requests.get(self.close_api)
    # Жаңа саусақ ізін енгзуге арналган запрос
    def NewFinger(self):
        requests.get(self.enter_new_finger_api)

            