import logging

from aiogram.dispatcher.filters import state
from aiogram.dispatcher.filters.state import State, StatesGroup
from aiogram.dispatcher import FSMContext
from load import bot, dp
from aiogram.dispatcher.filters import Text
from aiogram import types
from keyborad import*
from text import*
from config import*


class Auth(StatesGroup):
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


@dp.message_handler(state=Auth.s1)
async def s1_handler(message: types.Message, state: FSMContext):
    await Auth.next()

@dp.message_handler(state=Auth.s2)
async def s1_handler(message: types.Message, state: FSMContext):
    
    await Auth.next()


@dp.message_handler(state=Auth.s3)
async def s1_handler(message: types.Message, state: FSMContext):
    await Auth.next()


@dp.message_handler(state=Auth.s4)
async def s1_handler(message: types.Message, state: FSMContext):
    await Auth.next()

@dp.message_handler(state=Auth.s5)
async def s1_handler(message: types.Message, state: FSMContext):
    await Auth.next()


@dp.message_handler(state=Auth.s6)
async def s1_handler(message: types.Message, state: FSMContext):
    await Auth.next()


@dp.message_handler(state=Auth.s7)
async def s1_handler(message: types.Message, state: FSMContext):
    await state.finish()



