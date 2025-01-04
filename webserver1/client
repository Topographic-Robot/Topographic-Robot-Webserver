import asyncio
import websockets

async def receive_data():
    uri = "ws://localhost:8000/ws"
    async with websockets.connect(uri) as websocket:
        while True:
            message = await websocket.recv()  # Receive real-time data
            print(f"Received: {message}")

asyncio.run(receive_data())
