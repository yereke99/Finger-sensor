import logging
from aiogram.dispatcher.filters import state
from aiogram.dispatcher.filters.state import State, StatesGroup
from aiogram.dispatcher import FSMContext
import urllib
from load import bot, dp
from aiogram.dispatcher.filters import Text
from aiogram import types
from keyborad import*
from text import*
from config import*

class Offer(StatesGroup):
    s1 = State()
    s2 = State()
    s3 = State()
    s4 = State()
    s5 = State()
    s6 = State()


@dp.message_handler(state='*', commands='Бас тарту🙌🏻')
@dp.message_handler(Text(equals='Бас тарту🙌🏻', ignore_case=True), state='*')
async def cancell_handler(message: types.Message, state: FSMContext):
    """
        :param message: ???????????
        :param state: ???????
        :return: finish

    """

    current_state = await state.get_state()
    if current_state is None:
        return
    logging.info('Баc тарту!')

    await state.finish()
    await message.reply('Бас тарту🙌🏻', reply_markup=start_btn)


@dp.message_handler(content_types=['document'], state=Offer.s)
async def cv_handler(message: types.Message, state: FSMContext):
    
    document_id = message.document.file_id
    file_info = await bot.get_file(document_id)

    fi = file_info.file_path
    name = message.document.file_name
    urllib.request.urlretrieve(f'https://api.telegram.org/file/bot{token_api}/{fi}', f'./{name}')
    await bot.send_message(
        message.from_user.id,
        text="Сіздің өтінішіңіз сәтті қабылданды😉",
        reply_markup=start_btn
    )
    
    await state.finish()