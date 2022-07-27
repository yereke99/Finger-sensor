from aiogram import types


class Btn:
    def __init__(self) -> None:
        pass

    def start_btn(self):
        start_btn = types.ReplyKeyboardMarkup(selective=True, resize_keyboard=True)
        start_btn.add("🔒 Құлыптау")
        start_btn.add("🔓 Ашу")
        start_btn.add("👆🏻 Жаңа саусақ ізі")
        
        return start_btn

