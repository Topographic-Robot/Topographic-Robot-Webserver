import dash.dash
import dash
from dash import dcc, html
from dash.dependencies import Input,Output
import pandas as pd
import plotly.express as px
import json

#dash dashboard
app= dash.Dash(__name__)
app.layout= html.Div([
    html.H1("analyzed data"),
    dcc.Interval(id='update-interval',interval=5000,n_intervals=0),
    dcc.Graph(id='temp-graph'),
    dcc.Graph(id='gyro-graph'),
])

# websocket cliuent
sensor_data = []  # List to store real-time data

async def websocket_listener():
    uri = "ws://localhost:8000/ws"
    async with websockets.connect(uri) as websocket:
        while True:
            message = await websocket.recv()
            data = json.loads(message)
            sensor_data.append(data)  # Store received data

# Listener
def start_websocket_client():
    loop = asyncio.new_event_loop()
    asyncio.set_event_loop(loop)
    loop.run_until_complete(websocket_listener())

# Run WebSocket listener in background
threading.Thread(target=start_websocket_client, daemon=True).start()

#callback function for dynamical update
@app.callback(
        [
            Output('temp-graph','figure'),
            Output('gyro-graph','figure')

        ],
        [Input('update-interval', 'n_intervals')]
    )
def update_graphs(n):
    temp_fig = px.line(temp_sensor, y="timestamp", x="temperature_c", title="Temperature Over Time")
    gyro_fig = px.line(gyro_sensor, x="timestamp", y="gyro_x", title="Gyro X-Axis Over Time")
    return temp_fig, gyro_fig
# Run app
if __name__ == '__main__':
    app.run_server(debug=True)
