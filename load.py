import logging
from aiogram import Bot, Dispatcher, types
from aiogram.contrib.fsm_storage.memory import MemoryStorage
from config import token_api

bot = Bot(token=token_api)
memory = MemoryStorage()
dp = Dispatcher(bot=bot, storage=memory)

logging.basicConfig(format=u'%(filename)s [LINE:%(lineno)d] #%(levelname)-8s [%(asctime)s]  %(message)s', level=logging.INFO,)
