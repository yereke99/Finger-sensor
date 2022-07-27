from aiogram import types # aiogram 
from load import dp, bot  # боттың денесінен диспетчермен боттты импорттау
from keyborad import*     # кнопкаларды импорттау
from send_req import*     # запростарды импорттау
from send_req import*     #   

btn = Btn()               # кнопкаларға жалпы бір обьект құру

# /start командасын басқан кезде орындалатын handler
@dp.message_handler(commands=['start'])
async def start_handler(message: types.Message):
    print(message.from_user.id)
    """
    /start командасын басқан кезде бот юзерге хабарлама жібереді
    await бұл синтаксис асинхронды функцияларды бұғаттамай шақыру үшін жазады
    """
    await bot.send_message(
        message.from_user.id,
        text="Керекті функцияны таңдаңыз",
        reply_markup=btn.start_btn()
    )

# текстік хаттарды өңдеу handler-і
@dp.message_handler(content_types=['text'])
async def text_handler(message: types.Message):
    msg = message.text #  хабарламаны обьект ретінде алу
    req = Request()    #  запрос үшін обьект жасау
    
    # Құлыптау үшін хабарламаға жауап
    if msg == "🔒 Құлыптау":
        req.Close()
        await bot.send_message(
            message.from_user.id,
            text="Есік құлыпталды 🔒",
            reply_markup=btn.start_btn()
        )
    # Ашу үшін забарламаға жауап
    if msg == "🔓 Ашу":
        req.Open()
        await bot.send_message(
            message.from_user.id,
            text="Есік ашылды 🔓",
            reply_markup=btn.start_btn()
        )
    # Жаңа саусақ үшін хабарламаға жауап
    if msg == "👆🏻 Жаңа саусақ ізі":
        req.NewFinger()   # Жаңа саусақ ізін енгізу туралы микробақылауыш веб-серверіне запрос
        await bot.send_message(
            message.from_user.id,
            text="Жаңа 👆🏻саусақ таңба енгізу үшін Serial port-қа ID енгізіңіз!",
            reply_markup=btn.start_btn()
        )    