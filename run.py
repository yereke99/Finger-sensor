import asyncio
from email import message
from load import bot, memory
import aioschedule
from database import*
from async_manager import*




async def scheduler():
    aioschedule.every(1).seconds.do(noti)
    aioschedule.every(1).seconds.do(noti_new_finger)

    while True:
        await aioschedule.run_pending()
        await asyncio.sleep(1)

async def on_start_up(dp):
    asyncio.create_task(scheduler())

    
    

async def shut_down(dp):
    await memory.close()
    await bot.close()

if __name__ == "__main__":
    from aiogram import executor
    from main import dp
    #executor.start_polling(dp, on_shutdown=shut_down)
    executor.start_polling(dp, on_startup=on_start_up, on_shutdown=shut_down)    