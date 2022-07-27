from datetime import datetime
from main import*
from database import *
import asyncio
from config import id_admin

async def noti():
    db = Database()
    result = db.fetch_all_noti()
    for i in range(result):
        await asyncio.sleep(0.1)
        await bot.send_message(
            id_admin,
            text="Саусақ 👆🏻 ізі {} аутенфикациядан сәтті өтті 👌🏻".format(datetime.now())
        )
    

async def noti_new_finger():
    db = Database()
    result = db.fetch_new_finger()
    for i in range(result):
        await asyncio.sleep(0.1)
        await bot.send_message(
            id_admin,
            text="Жаңа саусақ ізі мәліметтер қорына сәтті енгізілді 👌🏻"
        )
